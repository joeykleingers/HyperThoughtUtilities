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

#include "HTConnection.h"

#include <cstdlib>
#include <ctime>

#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QtCore/QByteArray>
#include <QtCore/QEventLoop>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfo.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesFilters/OpenHyperThoughtConnection.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTUtils.h"

// -----------------------------------------------------------------------------
inline void waitForResponse(QNetworkReply* reply)
{
  QEventLoop loop;
  QObject::connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), &loop, &QEventLoop::quit);
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();
}

// -----------------------------------------------------------------------------
HTConnection* HTConnection::GetExistingConnection(const AbstractFilter* filter)
{
  // Find OpenHyperThoughtConnection filter
  const OpenHyperThoughtConnection* htFilter = nullptr;
  while(nullptr == htFilter && nullptr != filter)
  {
    // Do not check disabled filters
    if(filter->getEnabled())
    {
      htFilter = dynamic_cast<const OpenHyperThoughtConnection*>(filter);
    }
    if(nullptr == htFilter)
    {
      filter = filter->getPreviousFilter().lock().get();
    }
  }

  // Return HTConnection from the filter.
  if(htFilter)
  {
    return htFilter->getHTConnection();
  }
  return nullptr;
}

// -----------------------------------------------------------------------------
HTConnection::HTConnection()
: QObject(nullptr)
, m_NetworkManager(nullptr)
{
}

#if 0
// -----------------------------------------------------------------------------
HTConnection::HTConnection(const QString& baseAuthUrl, const QString& baseAuthIpUrl, const QString& clientId, QObject* parent)
: QObject(parent)
, m_Status(Status::Disconnected)
, m_Authority(QString("%1/openid").arg(baseAuthUrl))
, m_AuthorityNodns(QString("%1/openid").arg(baseAuthIpUrl))
, m_AuthorizationUrl(QString("%1/authorize").arg(m_AuthorityNodns))
, m_UserInfoUrl(QString("%1/userinfo/").arg(m_Authority))
, m_TokenUrl(QString("%1/token/").arg(m_Authority))
, m_FilesApiUrl(QString("%1/api/files/").arg(baseAuthUrl))
, m_NetworkManager(new QNetworkAccessManager())
, m_CookieJar(new QNetworkCookieJar())
{
  m_RequiredInfo.clientId = clientId;
  m_RequiredInfo.baseUrl = baseAuthUrl;
  m_RequiredInfo.redirectUri = QString("%1/access-code/").arg(baseAuthIpUrl);
  m_RequiredInfo.scope = "openid";
  m_RequiredInfo.nonce = generateNonce();
  m_RequiredInfo.responseType = "code";

  createNetworkManager();
}
#endif

// -----------------------------------------------------------------------------
HTConnection::HTConnection(const QString& encodedAccessToken, QObject* parent)
: QObject(parent)
, m_Status(Status::Disconnected)
, m_NetworkManager(new QNetworkAccessManager())
, m_CookieJar(new QNetworkCookieJar())
{
  setupNetworkManager();
  decodeApiAccess(encodedAccessToken);
  createDoDCookie();

  // Test Code
  // requestProjectFileInfo(",", "c1aab5fc-0fd0-4604-a347-ca136e1d58a1");
}

// -----------------------------------------------------------------------------
HTConnection::HTConnection(const HTConnection& rhs)
: QObject(rhs.parent())
, m_Status(rhs.m_Status)
, m_RequiredInfo(rhs.m_RequiredInfo)
, m_AuthorizationInfo(rhs.m_AuthorizationInfo)
, m_NetworkManager(new QNetworkAccessManager())
, m_CookieJar(new QNetworkCookieJar())
{
  setupNetworkManager();
  createDoDCookie();
}

// -----------------------------------------------------------------------------
HTConnection::~HTConnection() = default;

// -----------------------------------------------------------------------------
void HTConnection::setupNetworkManager()
{
  m_NetworkManager->setCookieJar(m_CookieJar);
  m_NetworkManager->setParent(this);

  connect(m_NetworkManager, &QNetworkAccessManager::sslErrors, this, &HTConnection::onSslErrors);
}

// -----------------------------------------------------------------------------
void HTConnection::createDoDCookie()
{
  // Network cookie
  QUrl baseUrl(m_RequiredInfo.baseUrl);
  QString cookieDomain = baseUrl.host();
  QNetworkCookie dodCookie("dodAccessBanner", "true");
  dodCookie.setDomain(cookieDomain);
  m_CookieJar->insertCookie(dodCookie);
}

// -----------------------------------------------------------------------------
void HTConnection::decodeApiAccess(const QString& apiAccess)
{
  setStatus(Status::Connecting);
  QString decodedStr = HTUtils::decode64(apiAccess.toStdString());
  qDebug() << decodedStr;
  QJsonDocument document = QJsonDocument::fromJson(decodedStr.toUtf8());
  if(!document.isObject())
  {
    setStatus(Status::Invalid);
    return;
  }

  QJsonObject obj = document.object();
  if(!obj.value("baseUrl").isString())
  {
    setStatus(Status::Invalid);
    return;
  }
  if(!obj.value("clientId").isString())
  {
    setStatus(Status::Invalid);
    return;
  }

  m_AuthorizationInfo = obj;
  QString baseUrl = obj.value("baseUrl").toString();

  m_RequiredInfo.clientId = obj.value("clientId").toString();
  m_RequiredInfo.baseUrl = baseUrl;
  m_RequiredInfo.redirectUri = QString("%1/access-code/").arg(baseUrl);
  m_RequiredInfo.scope = "openid";
  m_RequiredInfo.nonce = HTUtils::generateNonce();
  m_RequiredInfo.responseType = "code";

  setStatus(Status::Connected);
}

// -----------------------------------------------------------------------------
bool HTConnection::isValid() const
{
  if(nullptr == m_NetworkManager)
  {
    return false;
  }
  if(m_RequiredInfo.baseUrl.isEmpty() || m_RequiredInfo.clientId.isEmpty())
  {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------
void HTConnection::onAuthenticationErr(QNetworkReply::NetworkError err)
{
  setStatus(Status::Disconnected);

  QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
  if(nullptr == reply)
  {
    throw std::exception("Invalid sender. QNetworkReply required");
  }

  // Check for unhandled error
  if(err >= 400)
  {
    emit failedToAuthenticate(reply->error());
  }
}

// -----------------------------------------------------------------------------
void HTConnection::onAuthenticationResponse()
{
  QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
  if(nullptr == reply)
  {
    throw std::exception("Invalid sender. QNetworkReply required");
  }

  // Check for unhandled error
  if(reply->error() >= 400)
  {
    setStatus(Status::Disconnected);
    emit failedToAuthenticate(reply->error());
  }
  else
  {
    QVariant statusVar = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(!statusVar.isValid())
    {
      setStatus(Status::Disconnected);
      return;
    }
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode != 200)
    {
      QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
      qDebug() << reason;
    }

    QByteArray bytes = reply->readAll();
    QString str = QString::fromUtf8(bytes.data(), bytes.size());
    // int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << QVariant(statusCode).toString() << ": " << str;

    QJsonDocument document = QJsonDocument::fromJson(bytes);
    m_AuthorizationInfo = document.object();

    setStatus(Status::Connected);
    emit authenticationSuccessful();
  }
  reply->deleteLater();
}

#if 0
// -----------------------------------------------------------------------------
void HTConnection::refreshAccessToken()
{
  // Validity check
  if(!isValid())
  {
    return;
  }

  setStatus(Status::Connecting);

  QString refreshToken = getAuthInformation()["refreshToken"].toString();

#if 0
  QJsonObject dataObj;
  dataObj.insert("client_id", m_RequiredInfo.clientId);
  dataObj.insert("grant_type", "refresh_token");
  dataObj.insert("refresh_token", refreshToken);
  QJsonDocument dataDoc(dataObj);
  QString dataStr = dataDoc.toJson().replace("\n", "");
#endif

  QString jsonStr = QString("{\"client_id\":\"%1\",\"grant_type\":\"refresh_token\",\"refresh_token\":\"%2\"}").arg(m_RequiredInfo.clientId).arg(refreshToken);
  QByteArray json = jsonStr.toUtf8();
  QByteArray contentLength = QByteArray::number(json.size());

  qDebug() << "Refresh Access Token: " << json;

  QNetworkRequest request = createDefaultNetworkRequest();
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Content-Length", contentLength);

  QNetworkReply* reply = m_NetworkManager->post(request, json);
  connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &HTConnection::onAccessTokenErr);
  connect(reply, &QNetworkReply::finished, this, &HTConnection::onAccessTokenResponse);

  // Required for running during execute.
  waitForResponse(reply);
}
#endif

// -----------------------------------------------------------------------------
void HTConnection::onAccessTokenErr(QNetworkReply::NetworkError err)
{
  setStatus(Status::Disconnected);

  QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
  if(nullptr == reply)
  {
    throw std::exception("Invalid sender. QNetworkReply required");
  }

  qDebug() << reply->errorString();

  // Check for unhandled error
  int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  if(statusCode >= 400)
  {
    emit failedRefreshAccessToken();
  }
}

// -----------------------------------------------------------------------------
void HTConnection::onAccessTokenResponse()
{
  QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
  if(nullptr == reply)
  {
    setStatus(Status::Disconnected);
    throw std::exception("Invalid sender. QNetworkReply required");
  }

  // Check for unhandled error
  int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  if(statusCode >= 400)
  {
    setStatus(Status::Disconnected);
    qDebug() << reply->readAll();

    emit failedRefreshAccessToken();
  }
  else
  {
    if(statusCode != 200)
    {
      QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
      qDebug() << reason;
    }

    QByteArray bytes = reply->readAll();
    QString str = QString::fromUtf8(bytes.data(), bytes.size());
    qDebug() << QVariant(statusCode).toString() << ": " << str;

    QJsonDocument document = QJsonDocument::fromJson(bytes);
    m_AuthorizationInfo = document.object();

    setStatus(Status::Connected);
    emit accessTokenRefreshed();
  }
  reply->deleteLater();
}

// -----------------------------------------------------------------------------
QNetworkReply* HTConnection::get(const QNetworkRequest& request)
{
  return m_NetworkManager->get(request);
}

// -----------------------------------------------------------------------------
QNetworkReply* HTConnection::post(const QNetworkRequest& request, QIODevice* data)
{
  return m_NetworkManager->post(request, data);
}

// -----------------------------------------------------------------------------
QNetworkReply* HTConnection::post(const QNetworkRequest& request, const QByteArray& data)
{
  return m_NetworkManager->post(request, data);
}

// -----------------------------------------------------------------------------
QNetworkReply* HTConnection::put(const QNetworkRequest& request, QIODevice* data)
{
  return m_NetworkManager->put(request, data);
}

// -----------------------------------------------------------------------------
QNetworkReply* HTConnection::put(const QNetworkRequest& request, const QByteArray& data)
{
  return m_NetworkManager->put(request, data);
}

// -----------------------------------------------------------------------------
QNetworkReply* HTConnection::patch(const QNetworkRequest& request, QIODevice* data)
{
  return m_NetworkManager->sendCustomRequest(request, "PATCH", data);
}

// -----------------------------------------------------------------------------
QNetworkReply* HTConnection::patch(const QNetworkRequest& request, const QByteArray& data)
{
  return m_NetworkManager->sendCustomRequest(request, "PATCH", data);
}

// -----------------------------------------------------------------------------
QNetworkRequest HTConnection::createDefaultNetworkRequest() const
{
  QString accessToken = getAccessToken();
  QString authorizationVal = QString("Bearer %1").arg(accessToken);

  QNetworkRequest request;
  request.setRawHeader("Authorization", authorizationVal.toUtf8());

  // Do not verify SSL connection. This is actually a bad idea,
  // but we neither have nor can guarantee access to HyperThought's
  // Certificate Authority public key.
  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  return request;
}

// -----------------------------------------------------------------------------
QJsonObject HTConnection::getAuthInformation() const
{
  return m_AuthorizationInfo;
}

// -----------------------------------------------------------------------------
QString HTConnection::getAccessToken() const
{
  return getAuthInformation()["accessToken"].toString();
}

// -----------------------------------------------------------------------------
HTConnection::Status HTConnection::getStatus() const
{
  return m_Status;
}

// -----------------------------------------------------------------------------
void HTConnection::setStatus(Status status)
{
  if(status != m_Status)
  {
    m_Status = status;
    emit statusChanged(status);
  }
}

// -----------------------------------------------------------------------------
QNetworkAccessManager* HTConnection::getNetworkManager() const
{
  return m_NetworkManager;
}

// -----------------------------------------------------------------------------
void HTConnection::onSslErrors(QNetworkReply* reply, const QList<QSslError>& errs)
{
  // This is bad practice, but we cannot verify the HyperThought SSL connection.
  reply->ignoreSslErrors();
}

// -----------------------------------------------------------------------------
QString HTConnection::getClientId() const
{
  return m_RequiredInfo.clientId;
}

// -----------------------------------------------------------------------------
const HTFileCache& HTConnection::getFileCache() const
{
  return m_FileInfoCache;
}

// -----------------------------------------------------------------------------
HTFileCache& HTConnection::getFileCacheRef()
{
  return m_FileInfoCache;
}

#if 0
// -----------------------------------------------------------------------------
void HTConnection::setCallbackUrl(const QString& callback)
{
  m_CallbackUrl = callback;
}
#endif

// -----------------------------------------------------------------------------
QString HTConnection::getBaseUrl() const
{
  return m_RequiredInfo.baseUrl;
}

// -----------------------------------------------------------------------------
void HTConnection::takeFileCache(const HTConnection& other)
{
  m_FileInfoCache = std::move(other.getFileCache());
}
