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

#include <memory>
#include <vector>

#include <QtCore/QDir>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtNetwork/QNetworkCookieJar>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslConfiguration>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileCache.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfoTree.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTFilePath.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

class QFile;
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

class AbstractFilter;
class HTFileInfo;
class HTAbstractRequest;

/**
 * @class HTConnection HTConnection.h HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTConnection.h
 * @brief The HTConnection class contains a connection with a target
 * HyperThought server for the purpose of downloading, uploading, and tagging
 * data.
 */
class HyperThoughtUtilities_EXPORT HTConnection : public QObject
{
  Q_OBJECT

  friend class HTAbstractRequest;

public:
  enum class Status
  {
    Invalid,
    Disconnected,
    Connecting,
    Connected
  };
  Q_ENUM(Status);

  /**
   * @brief Retrieves the last created HTConnection already created in the given filter's pipeline.
   * @param filter
   * @return
   */
  static HTConnection* GetExistingConnection(const AbstractFilter* filter);

  /**
   * @brief Constructs an invalid HTConnection.
   */
  HTConnection();

#if 0
  /**
   * @brief Constructs an HTConnection from authority URLs and a client ID.
   * @param baseAuthUrl
   * @param baseAuthIpUrl
   * @param clientId
   */
  HTConnection(const QString& baseAuthUrl, const QString& baseAuthIpUrl, const QString& clientId, QObject* parent = nullptr);
#endif

  /**
   * @brief API Access constructor.
   * @param apiAccessToken
   * @param parent
   */
  HTConnection(const QString& apiAccessToken, QObject* parent = nullptr);

  /**
   * @brief Copy constructor for HTConnection.
   * @param rhs
   */
  HTConnection(const HTConnection& rhs);

  /**
   * @brief Destroys the HTConnection object.
   */
  ~HTConnection();

  /**
   * @brief Returns true if the connection is valid. Returns false otherwise.
   * Validity requires an access token and corresponding connection values set
   * by the constructor.
   * @return
   */
  bool isValid() const;

#if 0
  /**
   * @brief Asynchronously attempts to refresh the access token.
   * Emits accessTokenRefreshed() if successful.
   * Emits failedRefreshAccessToken(QNetworkReply::NetworkError) if unsuccessful.
   */
  void refreshAccessToken();
#endif

  /**
   * @brief Returns the current connection status.
   * @return
   */
  Status getStatus() const;

  /**
   * @brief Returns the Client ID.
   * @return
   */
  QString getClientId() const;

  /**
   * @brief Returns a const reference to the file info cache.
   * @return
   */
  const HTFileCache& getFileCache() const;

  /**
   * @brief Returns a const reference to the file info cache.
   * @return
   */
  HTFileCache& getFileCacheRef();

  /**
   * @brief Returns a pointer to the QNetworkAccessManager.
   * @return
   */
  QNetworkAccessManager* getNetworkManager() const;

  /**
   * @brief Perform a GET request. Returns a QNetworkReply pointer.
   * Wrapper for getNetworkManager()->get(const QNetworkRequest&);
   * @param request
   * @return
   */
  QNetworkReply* get(const QNetworkRequest& request);

  /**
   * @brief Perform a POST request. Returns a QNetworkReply pointer.
   * Wrapper for getNetworkManager()->post(const QNetworkRequest&, QIODevice*);
   * @param request
   * @param data
   * @return
   */
  QNetworkReply* post(const QNetworkRequest& request, QIODevice* data);

  /**
   * @brief Perform a POST request. Returns a QNetworkReply pointer.
   * Wrapper for getNetworkManager()->post(const QNetworkRequest&, const QByteArray&);
   * @param request
   * @param data
   * @return
   */
  QNetworkReply* post(const QNetworkRequest& request, const QByteArray& data);

  /**
   * @brief Perform a PUT request. Returns a QNetworkReply pointer.
   * Wrapper for getNetworkManager()->put(const QNetworkRequest&, QIODevice*);
   * @param request
   * @param data
   * @return
   */
  QNetworkReply* put(const QNetworkRequest& request, QIODevice* data);

  /**
   * @brief Perform a POST request. Returns a QNetworkReply pointer.
   * Wrapper for getNetworkManager()->put(const QNetworkRequest&, const QByteArray&);
   * @param request
   * @param data
   * @return
   */
  QNetworkReply* put(const QNetworkRequest& request, const QByteArray& data);

  /**
   * @brief Perform a PATCH request. Returns a QNetworkReply pointer.
   * Wrapper for getNetworkManager()->sendCustomRequest(const QNetworkRequest&, "PATCH", QIODevice*);
   * @param request
   * @param data
   * @return
   */
  QNetworkReply* patch(const QNetworkRequest& request, QIODevice* data);

  /**
   * @brief Perform a PATCH request. Returns a QNetworkReply pointer.
   * Wrapper for getNetworkManager()->sendCustomRequest(const QNetworkRequest&, "PATCH", const QByteArray&);
   * @param request
   * @param data
   * @return
   */
  QNetworkReply* patch(const QNetworkRequest& request, const QByteArray& data);

  /**
   * @brief Creates and returns a default network request for accessing HyperThought server.
   * @return
   */
  QNetworkRequest createDefaultNetworkRequest() const;

  /**
   * @brief Moves the cache from the given connection to this one.
   */
  void takeFileCache(const HTConnection& other);

signals:
  /**
   * @brief This signal is emitted when initial authentication succeeds.
   */
  void authenticationSuccessful();

  /**
   * @brief This signal is emitted when the initial authentication fails.
   * @param err
   */
  void failedToAuthenticate(QNetworkReply::NetworkError err);

  /**
   * @brief This signal is emitted when the access token is refreshed.
   */
  void accessTokenRefreshed();

  /**
   * @brief This signal is emitted when the access token has failed to update.
   * @param err
   */
  void failedRefreshAccessToken();

  /**
   * @brief This signal is emitted when the connection status changes.
   * @param newStatus
   */
  void statusChanged(Status newStatus);

protected:
  /**
   * @brief Returns the core HyperThought URL.
   * @return
   */
  QString getBaseUrl() const;

  /**
   * @brief Returns the authorization info as a QJsonObject.
   * @return
   */
  QJsonObject getAuthInformation() const;

  /**
   * @brief Extracts and returns the access token from the authorization info.
   * @return
   */
  QString getAccessToken() const;

private:
  /**
   * @brief Sets the cookie jar, parent, and SSL error connection for the network manager.
   */
  void setupNetworkManager();

  /**
   * @brief Creates the DoD Banner cookie using the host address
   * This method requires the access token to have been parsed before use.
   */
  void createDoDCookie();

  /**
   * @brief Decodes the API Access string and stores the resulting values.
   * @param apiAccess
   */
  void decodeApiAccess(const QString& apiAccess);

  /**
   * @brief Slot connection for the access token QNetworkReply.
   * @param err
   */
  void onAccessTokenErr(QNetworkReply::NetworkError err);

  /**
   * @brief Called when the access token request is finished.
   * This will still be called if there is a network error.
   */
  void onAccessTokenResponse();

  /**
   * @brief Slot connection for the authentication QNetworkReply.
   * @param err
   */
  void onAuthenticationErr(QNetworkReply::NetworkError err);

  /**
   * @brief Called when the authentication request is finished.
   * This will still be called if there is a network error.
   */
  void onAuthenticationResponse();

  /**
   * @brief Prints the errors to qDebug()
   * @param reply
   * @param errs
   */
  void onSslErrors(QNetworkReply* reply, const QList<QSslError>& errs);

  /**
   * @brief Sets the connection status and emits the statusChanged signal when the value changes.
   * @param status
   */
  void setStatus(Status status);

  // -----------------------------------------------------------------------------
  // Variables
  struct RequiredInfo
  {
    QString clientId;
    QString baseUrl;
    QString redirectUri;
    QString scope;
    QString nonce;
    QString responseType;
  };

  Status m_Status = Status::Invalid;
  RequiredInfo m_RequiredInfo;
  QJsonObject m_AuthorizationInfo;

  // Network handling
  QNetworkAccessManager* m_NetworkManager;
  QNetworkCookieJar* m_CookieJar;

  HTFileCache m_FileInfoCache;
};

Q_DECLARE_METATYPE(HTConnection)
