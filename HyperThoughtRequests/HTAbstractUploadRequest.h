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

#pragma once

#include "HTAbstractRequest.h"

#include <QtCore/QJsonDocument>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTFilePath.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTMetaData.h"

class HTUpdateMetaDataRequest;

/**
 * @class HTFileUploadRequest HTFileUploadRequest.h HyperThoughtUtilities/HyperThoughtConnection/HTFileUploadRequest.h
 * @brief The HTFileUploadRequest class creates and manages a network request to upload a specified file or data to HyperThought.
 */
class HyperThoughtUtilities_EXPORT HTAbstractUploadRequest : public HTAbstractRequest
{
  Q_OBJECT

public:
  /**
   * @brief Constructor
   * @param connection
   * @param uploadPath
   * @param isAsync
   */
  HTAbstractUploadRequest(HTConnection* connection, const HTFilePath& uploadPath, bool isAsync = false);

  /**
   * @brief Destructor
   */
  ~HTAbstractUploadRequest() override;

  /**
   * @brief Returns the HyperThought upload path.
   * @return
   */
  HTFilePath getUploadPath() const;

  /**
   * @brief Sets the HyperThought upload path
   * @param path
   */
  void setUploadPath(const HTFilePath& path);

  /**
   * @brief Returns the upload file name. This must be implemented in derived classes.
   * @return
   */
  virtual QString getUploadName() const = 0;

  bool assignsMetaData() const;
  HTMetaData getMetaData() const;
  void setMetaData(const HTMetaData& metadata);
  void setMetaData(bool useMetaData, const HTMetaData& metadata);

signals:
  void uploadComplete();
  void cannotReadFile();
  void uploadFailed(QNetworkReply::NetworkError err);

protected:
  /**
   * @brief Returns the URL for generating upload URLs.
   * @return
   */
  QString getGenerateUploadUrl() const;

  /**
   * @brief Returns the URL for finalizing file uploads.
   * @return
   */
  QString getFinalizeUploadUrl() const;

  QNetworkRequest createNetworkRequest() const;

  /**
   * @brief Initialize the file upload using the target upload directory and local file path.
   * Copies the local file to a QByteArray.
   * Calls onInitResponse() when completed.
   * @param uploadTarget
   * @param localPath
   */
  void initFileUpload(const HTFilePath& uploadTarget, const QString& localPath);

  /**
   * @brief Uploads the QByteArray data to the URL provided by HyperThought in the initFileUpload response.
   * Calls onDataUploaded() when completed.
   */
  void uploadData();

  /**
   * @brief Finalizes the data upload by notifying HyperThought that the file has been fully uploaded.
   * Calls onUploadFinalized() when completed.
   */
  void finalizeUpload();

  /**
   * @brief Requests HyperThought update the metadata for the uploaded file.
   */
  void patchMetaData();

  /**
   * @brief Creates the json payload for the initial upload request.
   * @retrn
   */
  QJsonDocument getInitPayload() const;

  /**
   * @brief Creates the json payload for the finalize upload request.
   * @return
   */
  QJsonDocument getFinalPayload() const;

  /**
   * @brief Called when HyperThought responds to the initial request to upload data.
   * If no network errors occured, the response contains the URL for the next part
   * of the upload request and the fileId to use for finalizing the upload.
   * Otherwise, emits uploadFailed(QNetworkReply::NetworkError).
   */
  void onInitResponse();

  /**
   * @brief Called when the data has finished uploading to HyperThought.
   * If no network errors occurred, this calls finalizeUpload() to finalize the upload.
   * Otherwise, emits uploadFailed(QNetworkReply::NetworkError).
   */
  void onDataUploaded();

  /**
   * @brief Called when the upload has been finalized.
   * If no network errors occurred, the uploadComplete() signal is emitted.
   * Otherwise, emits uploadFailed(QNetworkReply::NetworkError).
   */
  void onUploadFinalized();

  /**
   * @brief Called when the uploaded HyperThought file has had its metadata patched.
   * Deletes the update request and returns uploadComplete().
   */
  void onMetaDataPatched();

  /**
   * @brief Called when the uploaded HyperThought file failed to be patched.
   * @param err
   */
  void onMetaDataError(QNetworkReply::NetworkError err);

  /**
   * @brief Returns the content type for the data to be uploaded.
   * @return
   */
  QString getContentType() const;

private:
  HTFilePath m_UploadPath;
  QByteArray m_UploadData;
  QString m_UploadFileId;
  QString m_UploadUrl;
  bool m_AssignsMetaData;
  HTMetaData m_MetaData;
  HTUpdateMetaDataRequest* m_MetaDataRequest = nullptr;
};
