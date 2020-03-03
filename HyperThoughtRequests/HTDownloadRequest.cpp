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

#include "HTDownloadRequest.h"

#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTDownloadRequest::HTDownloadRequest(HTConnection* connection, const HTFilePath& path, bool isAsync)
: HTAbstractRequest(connection, isAsync)
, m_FilePath(path)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTDownloadRequest::~HTDownloadRequest() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath HTDownloadRequest::getFilePath() const
{
  return m_FilePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTDownloadRequest::setFilePath(const HTFilePath& path)
{
  m_FilePath = path;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTDownloadRequest::getDownloadName() const
{
  return m_DownloadName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTDownloadRequest::setDownloadName(const QString& name)
{
  m_DownloadName = name;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QDir HTDownloadRequest::getDownloadDir() const
{
  return m_DownloadDir;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTDownloadRequest::setDownloadDir(const QDir& dir)
{
  m_DownloadDir = dir;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTDownloadRequest::exec()
{
  QString filePath = m_DownloadDir.path() + "/" + getDownloadName();
  m_File = new QFile(filePath, this);
  if(!m_File->open(QIODevice::WriteOnly | QIODevice::Truncate))
  {
    return;
  }

  HTFileInfo fileInfo = getConnection()->getFileCache().getFileInfo(m_FilePath);
  QString downloadUrl = getDownloadApiUrl() + "?pk=" + fileInfo.getId();
  QNetworkRequest request = getConnection()->createDefaultNetworkRequest();
  request.setUrl(downloadUrl);

  QNetworkReply* reply = getConnection()->get(request);
  connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &HTDownloadRequest::downloadFailed);
  connect(reply, &QNetworkReply::downloadProgress, this, &HTDownloadRequest::onDownloadProgress);
  connect(reply, &QNetworkReply::finished, this, &HTDownloadRequest::onDownloadComplete);
  connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);

  waitForResponse(reply);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTDownloadRequest::onDownloadProgress(int64_t bytesReceived, int64_t bytesTotal)
{
  QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
  if(nullptr == reply)
  {
    throw std::runtime_error("Invalid sender. QNetworkReply required");
  }

  // QDataStream::operator<< stringifies the byte data.
  // Use QFile::write(buffer, size) to write bytes instead of string data.
  QByteArray bytes = reply->readAll();
  m_File->write(bytes.data(), bytes.size());

  emit downloadProgress(bytesReceived, bytesTotal);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTDownloadRequest::onDownloadComplete()
{
  m_File->close();
  m_File->deleteLater();

  emit downloadComplete();
}
