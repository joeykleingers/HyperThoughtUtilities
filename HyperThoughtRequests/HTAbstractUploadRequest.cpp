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

#include "HTAbstractUploadRequest.h"

#include <QtCore/QMimeDatabase>

#include "HyperThoughtUtilities/HyperThoughtRequests/HTUpdateMetaDataRequest.h"


// -----------------------------------------------------------------------------
HTAbstractUploadRequest::HTAbstractUploadRequest(HTConnection* connection, const HTFilePath& uploadPath, bool isAsync)
: HTAbstractRequest(connection, isAsync)
, m_UploadPath(uploadPath)
{
}

// -----------------------------------------------------------------------------
HTAbstractUploadRequest::~HTAbstractUploadRequest() = default;

// -----------------------------------------------------------------------------
HTFilePath HTAbstractUploadRequest::getUploadPath() const
{
  return m_UploadPath;
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::setUploadPath(const HTFilePath& path)
{
  m_UploadPath = path;
}

// -----------------------------------------------------------------------------
bool HTAbstractUploadRequest::assignsMetaData() const
{
  return m_AssignsMetaData;
}

// -----------------------------------------------------------------------------
HTMetaData HTAbstractUploadRequest::getMetaData() const
{
  return m_MetaData;
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::setMetaData(const HTMetaData& metadata)
{
  m_AssignsMetaData = true;
  m_MetaData = metadata;
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::setMetaData(bool useMetaData, const HTMetaData& metadata)
{
  m_AssignsMetaData = useMetaData;
  m_MetaData = metadata;
}

// -----------------------------------------------------------------------------
QString HTAbstractUploadRequest::getGenerateUploadUrl() const
{
  return getFilesApiUrl() + "generate-upload-url/";
}

// -----------------------------------------------------------------------------
QString HTAbstractUploadRequest::getFinalizeUploadUrl() const
{
  return getFilesApiUrl() + "temp-to-perm/";
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::initFileUpload(const HTFilePath& uploadTarget, const QString& localPath)
{
  // Extract data from the local file
  QFile file(localPath);
  if(!file.open(QIODevice::OpenModeFlag::ReadOnly))
  {
    emit cannotReadFile();
    return;
  }
  else
  {
    m_UploadData = file.readAll();
  }

  // Request the upload URL from HyperThought
  QByteArray json = getInitPayload().toJson(QJsonDocument::Compact);
  auto request = createUploadURLRequest(json);

  // Send request and wait for a reply.
  auto reply = sendUploadURLRequest(request, json);

  // If asynchronous, wait for a response before proceeding
  waitForResponse(reply);
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::initDataUpload(const QByteArray& data)
{
  m_UploadData = data;

  // Request the upload URL from HyperThought
  QByteArray json = getInitPayload().toJson(QJsonDocument::Compact);
  auto request = createUploadURLRequest(json);

  // Send request and wait for a reply.
  auto reply = sendUploadURLRequest(request, json);

  // If asynchronous, wait for a response before proceeding
  waitForResponse(reply);
}

// -----------------------------------------------------------------------------
QNetworkRequest HTAbstractUploadRequest::createUploadURLRequest(const QByteArray& json) const
{
  QByteArray jsonSize = QByteArray::number(json.size());
  QString genUploadUrl = getFilesApiUrl() + "generate-upload-url/";
  auto request = getConnection()->createDefaultNetworkRequest();
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Content-Length", jsonSize);
  request.setUrl(genUploadUrl);
  return request;
}

// -----------------------------------------------------------------------------
QNetworkReply* HTAbstractUploadRequest::sendUploadURLRequest(const QNetworkRequest& request, const QByteArray& json) const
{
  auto reply = getConnection()->post(request, json);
  connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &HTAbstractUploadRequest::uploadFailed);
  connect(reply, &QNetworkReply::finished, this, &HTAbstractUploadRequest::onInitResponse);
  connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
  return reply;
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::uploadData()
{
  auto request = getConnection()->createDefaultNetworkRequest();
  request.setUrl(m_UploadUrl);
  request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/octet-stream");
  request.setHeader(QNetworkRequest::KnownHeaders::ContentLengthHeader, m_UploadData.size());

  // Send request and wait for a reply.
  auto reply = getConnection()->put(request, m_UploadData);
  connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &HTAbstractUploadRequest::uploadFailed);
  connect(reply, &QNetworkReply::finished, this, &HTAbstractUploadRequest::onDataUploaded);
  connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);

  // If asynchronous, wait for a response before proceeding
  waitForResponse(reply);
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::finalizeUpload()
{
  QByteArray json = getFinalPayload().toJson(QJsonDocument::Compact);
  QString tempUrl = getFilesApiUrl() + "temp-to-perm/";
  auto request = getConnection()->createDefaultNetworkRequest();
  request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
  request.setHeader(QNetworkRequest::KnownHeaders::ContentLengthHeader, json.size());
  request.setUrl(tempUrl);
  
  // Send request and wait for a reply.
  auto reply = getConnection()->patch(request, json);
  connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &HTAbstractUploadRequest::uploadFailed);
  connect(reply, &QNetworkReply::finished, this, &HTAbstractUploadRequest::onUploadFinalized);
  connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);

  // If asynchronous, wait for a response before proceeding
  waitForResponse(reply);
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::patchMetaData()
{
  m_MetaDataRequest = new HTUpdateMetaDataRequest(getConnection(), m_UploadFileId, m_MetaData, isAsync());
  connect(m_MetaDataRequest, &HTUpdateMetaDataRequest::finished, this, &HTAbstractUploadRequest::onMetaDataPatched);
  connect(m_MetaDataRequest, &HTUpdateMetaDataRequest::requestFailed, this, &HTAbstractUploadRequest::onMetaDataError);
  m_MetaDataRequest->exec();
}

// -----------------------------------------------------------------------------
QNetworkRequest HTAbstractUploadRequest::createNetworkRequest() const
{
  QNetworkRequest request = getConnection()->createDefaultNetworkRequest();
  request.setRawHeader("Content-Type", "application/json");
  request.setUrl(getFilesApiUrl());
  return request;
}

// -----------------------------------------------------------------------------
QJsonDocument HTAbstractUploadRequest::getInitPayload() const
{
  QString scopeId = m_UploadPath.getSourceId();
  if(m_UploadPath.getScopeType() == HTFilePath::ScopeType::User)
  {
    scopeId = m_UploadPath.getUsername();
  }

  QJsonObject payload;
  payload["path"] = m_UploadPath.getPath();
  payload["space"] = m_UploadPath.getScopeName();
  payload["space_id"] = scopeId;
  payload["users"] = QJsonArray();
  payload["name"] = getUploadName();
  payload["contentType"] = getContentType();
  payload["size"] = m_UploadData.size();

  QJsonDocument doc;
  doc.setObject(payload);
  return doc;
}

// -----------------------------------------------------------------------------
QJsonDocument HTAbstractUploadRequest::getFinalPayload() const
{
  QJsonObject payload;
  payload["file_ids"] = m_UploadFileId;
  
  QJsonDocument doc;
  doc.setObject(payload);
  return doc;
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::onInitResponse()
{
  QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
  if(reply->error() > 0)
  {
    uploadFailed(reply->error());
    return;
  }
  
  QByteArray response = reply->readAll();
  QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
  QJsonObject json = jsonDoc.object();
  m_UploadUrl = json["url"].toString();
  m_UploadFileId = json["fileId"].toString();

  uploadData();
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::onDataUploaded()
{
  QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
  if(reply->error() > 0)
  {
    uploadFailed(reply->error());
    return;
  }

  // Finalize the data upload
  finalizeUpload();
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::onUploadFinalized()
{
  QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
  if(reply->error() > 0)
  {
    uploadFailed(reply->error());
    return;
  }

  if(m_AssignsMetaData)
  {
    patchMetaData();
  }
  else
  {
    emit uploadComplete();
  }
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::onMetaDataPatched()
{
  m_MetaDataRequest->deleteLater();
  m_MetaDataRequest = nullptr;
  emit uploadComplete();
}

// -----------------------------------------------------------------------------
void HTAbstractUploadRequest::onMetaDataError(QNetworkReply::NetworkError err)
{
  m_MetaDataRequest->deleteLater();
  m_MetaDataRequest = nullptr;

  emit uploadFailed(err);
}

// -----------------------------------------------------------------------------
QString HTAbstractUploadRequest::getContentType() const
{
  QMimeDatabase db;
  QMimeType type = db.mimeTypeForData(m_UploadData);
  return type.name();
}
