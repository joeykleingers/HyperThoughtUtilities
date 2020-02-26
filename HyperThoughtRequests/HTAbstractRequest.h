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

#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>

#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

class HTConnection;

/**
 * @class HTAbstractRequest HTAbstractRequest.h HyperThoughtUtilities/HyperThoughtConnection/HTAbstractRequest.h
 * @brief The HTAbstractRequest class serves as the basis of all HyperThought requests.
 * It provides access to HyperThought connection values to its subclasses as well as
 * synchronous / asynchronous control flow.
 */
class HyperThoughtUtilities_EXPORT HTAbstractRequest : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Constructor
   * @param connection
   * @param isAsync
   */
  HTAbstractRequest(HTConnection* connection, bool isAsync = false);

  /**
   * @brief Destructor
   */
  ~HTAbstractRequest() override;

  /**
   * @brief Returns the HTConnection pointer.
   * @return
   */
  HTConnection* getConnection() const;

  /**
   * @brief Checks if the request is handled asynchronously or not.
   * @return
   */
  bool isAsync() const;

  /**
   * @brief Sets the asynchronous behaviour.
   * @param async
   */
  void setAsync(bool async);

  /**
   * @brief Makes the approriate request over the connection.
   * Emits the appropriate signals as the request is completed.
   * @return
   */
  virtual void exec() = 0;

signals:
  void requestFailed(QNetworkReply::NetworkError err);
  void finished();

protected:
  /**
   * @brief Returns the Authority URL.
   * @return
   */
  QString getAuthorityUrl() const;

  /**
   * @brief Returns the Authority No DNS URL.
   * @return
   */
  QString getAuthorityNoDnsUrl() const;

  /**
   * @brief Returns the Authorization URL.
   * @return
   */
  QString getAuthorizationUrl() const;

  /**
   * @brief Returns the User Info URL.
   * @return
   */
  QString getUserInfoUrl() const;

  /**
   * @brief Returns the Token URL.
   * @return
   */
  QString getTokenUrl() const;

  /**
   * @brief Returns the Files API URL.
   * @return
   */
  QString getFilesApiUrl() const;

  /**
   * @brief Returns the Files Download API URL.
   * @return
   */
  QString getDownloadApiUrl() const;

  /**
   * @brief This method is used to make a request behave synchronously.
   * An event loop is connected to the given reply and will only stop when an
   * error is emitted or is the request is completed.
   * If the request is asynchronous, this method does nothing.
   * @param reply
   */
  void waitForResponse(QNetworkReply* reply);

private:
  HTConnection* m_Connection = nullptr;
  bool m_IsAsync = false;
};
