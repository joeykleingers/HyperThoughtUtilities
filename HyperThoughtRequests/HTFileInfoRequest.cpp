/* ============================================================================
 * Copyright (c) 2020-2020 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-15-D-5231
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "HTFileInfoRequest.h"

#include <QtCore/QUrlQuery>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"

// -----------------------------------------------------------------------------
HTFileInfoRequest::HTFileInfoRequest(HTConnection* connection, const HTFilePath& path, bool isAsync)
: HTAbstractRequest(connection, isAsync)
, m_Path(path)
{
}

// -----------------------------------------------------------------------------
HTFileInfoRequest::~HTFileInfoRequest() = default;

// -----------------------------------------------------------------------------
HTFilePath HTFileInfoRequest::getFilePath() const
{
  return m_Path;
}

// -----------------------------------------------------------------------------
void HTFileInfoRequest::setFilePath(const HTFilePath& filePath)
{
  m_Path = filePath;
}

// -----------------------------------------------------------------------------
void HTFileInfoRequest::exec()
{
  m_RecursiveSearch.FileTree.clear();
  m_RecursiveSearch.FilePath = getFilePath();

  requestAdditionalFileInfoItems(m_RecursiveSearch.FilePath);
}

// -----------------------------------------------------------------------------
void HTFileInfoRequest::requestAdditionalFileInfoItems(const HTFilePath& filePath)
{
  m_RecursiveSearch.RemainingItems++;
  requestFileInfo(getFileListRequest(filePath));
}

// -----------------------------------------------------------------------------
void HTFileInfoRequest::requestFileInfo(const QNetworkRequest& request)
{
  QNetworkReply* reply = getConnection()->get(request);
  connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &HTFileInfoRequest::requestFailed);
  connect(reply, &QNetworkReply::finished, this, &HTFileInfoRequest::onFileInfoResponse);

  // Required for running during execute.
  waitForResponse(reply);
}

// -----------------------------------------------------------------------------
QNetworkRequest HTFileInfoRequest::getFileListRequest(const HTFilePath& path)
{
  switch(path.getScopeType())
  {
  case HTFilePath::ScopeType::User:
    return createUserFileInfoRequest(path.getPath());
    break;
  case HTFilePath::ScopeType::Group:
    return createFileInfoRequest(path.getPath(), "group", path.getSourceId());
    break;
  case HTFilePath::ScopeType::Project:
    return createFileInfoRequest(path.getPath(), "project", path.getSourceId());
  }

  // throw std::exception("Invalid HTFilePath: Unrecognized ScopeType");
  return QNetworkRequest();
}

// -----------------------------------------------------------------------------
QNetworkRequest HTFileInfoRequest::createUserFileInfoRequest(const QString& path) const
{
  QString filePath = path.isEmpty() ? "," : path;
  QUrlQuery query;
  query.addQueryItem("path", filePath.toUtf8());
  query.addQueryItem("method", "user");

  QUrl fileApiUrl(getFilesApiUrl());
  fileApiUrl.setQuery(query);

  QNetworkRequest request = getConnection()->createDefaultNetworkRequest();
  request.setUrl(fileApiUrl);
  return request;
}

// -----------------------------------------------------------------------------
QNetworkRequest HTFileInfoRequest::createFileInfoRequest(const QString& path, const QString& method, const QString& pk)
{
  QString filePath = path.isEmpty() ? "," : path;
  QUrlQuery query;
  query.addQueryItem(method, pk.toUtf8());
  query.addQueryItem("path", filePath.toUtf8());
  query.addQueryItem("method", method);

  QUrl fileApiUrl(getFilesApiUrl());
  fileApiUrl.setQuery(query);

  QNetworkRequest request = getConnection()->createDefaultNetworkRequest();
  request.setUrl(fileApiUrl);
  return request;
}

// -----------------------------------------------------------------------------
void HTFileInfoRequest::onFileInfoResponse()
{
  QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
  if(nullptr == reply)
  {
    throw std::exception("Invalid sender. QNetworkReply required");
  }

  if(reply->error() <= 0)
  {
    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    std::vector<HTFileInfo> files = HTFileInfo::FromDocument(doc);

    // Copy files into recursive search object and check isTopLevel state.
    m_RecursiveSearch.FileTree.insert(files);

    // Recursively search folders for additional files
    for(const auto& file : files)
    {
      if(file.isDir())
      {
        QString newPath = file.getContent().path + file.getContent().pk + ",";
        m_RecursiveSearch.FilePath.setPath(newPath);
        requestAdditionalFileInfoItems(m_RecursiveSearch.FilePath);
      }
    }

    m_RecursiveSearch.RemainingItems--;

    // Only emit the fileInfoReceived signal once
    if(m_RecursiveSearch.RemainingItems == 0)
    {
      onRequestCompleted();
    }
  }
  reply->deleteLater();
}

// -----------------------------------------------------------------------------
void HTFileInfoRequest::onRequestCompleted()
{
  // Update file info cache
  HTFileInfoTree infoTree = m_RecursiveSearch.FileTree;
  infoTree.sort();
  getConnection()->getFileCacheRef().setFileInfoTree(getFilePath(), infoTree);

  // Emit the requested information
  emit infoReceived(infoTree);
}
