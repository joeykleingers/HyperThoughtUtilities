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

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfoTree.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTFilePath.h"

class HyperThoughtUtilities_EXPORT HTFileInfoRequest : public HTAbstractRequest
{
  Q_OBJECT

public:
  HTFileInfoRequest(HTConnection* connection, const HTFilePath& path, bool isAsync = false);
  ~HTFileInfoRequest() override;

  /**
   * @brief Returns the HyperThought file path to recursively request file info from.
   * @return
   */
  HTFilePath getFilePath() const;

  /**
   * @brief Sets the HyperThought file path to recursively request file info from.
   * @param filePath
   */
  void setFilePath(const HTFilePath& filePath);

  /**
   * @brief Performs the approriate request over the connection.
   * Emits the appropriate signals as the request is completed.
   * @return
   */
  void exec() override;

signals:
  void infoReceived(const HTFileInfoTree& fileInfoTree);

private:
  /**
   * @brief Uses the given QNetworkRequest to request files from the HyperThought server.
   * Connects the appropriate signals and slots.
   * @param request
   */
  void requestFileInfo(const QNetworkRequest& request);

  /**
   * @brief Recursive helper method for requesting HTFileInfo items from HyperThought.
   * @param filePath
   */
  void requestAdditionalFileInfoItems(const HTFilePath& filePath);

  /**
   * @brief Creates a QNetworkRequest for the specified HTFilePath.
   * @param path
   * @return
   */
  QNetworkRequest getFileListRequest(const HTFilePath& path);

  /**
   * @brief Creates and returns a QNetworkRequest for the given path and project.
   * @param path
   * @return
   */
  QNetworkRequest createUserFileInfoRequest(const QString& path) const;

  /**
   * @brief Creates and returns a QNetworkRequest for the given path, method, and pk.
   * This can be used for both project and group methods.
   * @param path
   * @param method
   * @param pk
   */
  QNetworkRequest createFileInfoRequest(const QString& path, const QString& method, const QString& pk);

  /**
   * @brief Handles responses from any of the recursive requests for file info.
   * Emits infoReceived when the last request has been completed.
   */
  void onFileInfoResponse();

  /**
   * @brief Called when the last recursive response has been received.
   * Inserts the HTFileInfoTree requested into the HyperThought file info cache
   * and emits the infoReceived signal.
   */
  void onRequestCompleted();

  // -----------------------------------------------------------------------------
  // Variables
  struct FileInfoSearch
  {
    HTFileInfoTree FileTree;
    HTFilePath FilePath;
    size_t RemainingItems = 0;
  };

  HTFilePath m_Path;
  FileInfoSearch m_RecursiveSearch;
};
