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

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfo.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTMetaData.h"

/**
 * @class HTUpdateMetaDataRequest HTUpdateMetaDataRequest.h
 * HyperThoughtUtilities/HyperThoughtRequests/HTUpdateMetaDataRequest.h
 * @brief The HTUpdateMetaDataRequest sends a patch request to HyperThought to update a file's metadata.
 */
class HyperThoughtUtilities_EXPORT HTUpdateMetaDataRequest : public HTAbstractRequest
{
  Q_OBJECT

public:
  HTUpdateMetaDataRequest(HTConnection* connection, const QString& fileId, const HTMetaData& metadata, bool isAsync = false);
  ~HTUpdateMetaDataRequest() override;

  /**
   * @brief Performs the approriate request over the connection.
   * Emits the appropriate signals as the request is completed.
   * @return
   */
  void exec() override;

signals:
  void metaDataUpdated();

private:
  /**
   * @brief Creates and returns a QNetworkRequest for updating a HyperThought file's metadata.
   * @return
   */
  QNetworkRequest createNetworkRequest() const;

  /**
   * @brief Creates and returns the json payload for updating a HyperThought file's metadata.
   * @return
   */
  QJsonDocument createPayloadDoc() const;

  /**
   * @brief Called when the last recursive response has been received.
   * Inserts the HTFileInfoTree requested into the HyperThought file info cache
   * and emits the infoReceived signal.
   */
  void onRequestCompleted();

  // -----------------------------------------------------------------------------
  // Variables
  QString m_FileId;
  HTMetaData m_MetaData;
};
