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

#include <QtCore/QDir>
#include <QtCore/QFile>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfo.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTFilePath.h"

/**
 * @class HTDownloadRequest HTDownloadRequest.h HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTDownloadRequest.h
 * @brief The HTDownloadRequest class is used for creating download requests for a specified HyperThought connection.
 */
class HyperThoughtUtilities_EXPORT HTDownloadRequest : public HTAbstractRequest
{
  Q_OBJECT

public:
  /**
   * @brief Constructor
   * @param connection
   * @param path
   * @param isAsync
   */
  HTDownloadRequest(HTConnection* connection, const HTFilePath& path, bool isAsync = false);

  /**
   * @brief Destructor
   */
  ~HTDownloadRequest() override;

  /**
   * @brief Returns the HTFilePath to download
   * @return
   */
  HTFilePath getFilePath() const;

  /**
   * @brief Sets the HTFilePath to download
   * @param id
   */
  void setFilePath(const HTFilePath& path);

  /**
   * @brief Returns the target file name.
   * @return
   */
  QString getDownloadName() const;

  /**
   * @brief Sets the target file name.
   * @param name
   */
  void setDownloadName(const QString& name);

  /**
   * @brief Returns the download directory.
   * @return
   */
  QDir getDownloadDir() const;

  /**
   * @brief Sets the download directory.
   * @param dir
   */
  void setDownloadDir(const QDir& dir);

  /**
   * @brief Performs the approriate request over the connection.
   * Emits the appropriate signals as the request is completed.
   * @return
   */
  void exec() override;

signals:
  /**
   * @brief This signal is emitted when the file has finished downloading.
   */
  void downloadComplete();

  /**
   * @brief This signal is emitted when the file download has progressed its download.
   * @param bytesReceived
   * @param bytesTotal
   */
  void downloadProgress(int64_t bytesReceived, int64_t bytesTotal);

  /**
   * @brief This signal is emitted when the download failed.
   * @param err
   */
  void downloadFailed(QNetworkReply::NetworkError err);

private:
  /**
   * @brief Load bytes received into the target file and emit the downloadProgress signal.
   * @param bytesReceived
   * @param bytesTotal
   */
  void onDownloadProgress(int64_t bytesReceived, int64_t bytesTotal);

  /**
   * @brief Closes and deletes the target file before emitting the downloadComplete signal.
   */
  void onDownloadComplete();

  // -----------------------------------------------------------------------------
  // Variables
  HTFilePath m_FilePath;
  HTFileInfo m_FileInfo;
  QString m_DownloadName;
  QDir m_DownloadDir;
  QFile* m_File = nullptr;
};
