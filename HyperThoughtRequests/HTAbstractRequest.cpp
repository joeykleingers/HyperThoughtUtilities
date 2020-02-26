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

#include "HTAbstractRequest.h"

#include <QtCore/QEventLoop>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTAbstractRequest::HTAbstractRequest(HTConnection* connection, bool isAsync)
: QObject(connection)
, m_Connection(connection)
, m_IsAsync(isAsync)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTAbstractRequest::~HTAbstractRequest() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnection* HTAbstractRequest::getConnection() const
{
  return m_Connection;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool HTAbstractRequest::isAsync() const
{
  return m_IsAsync;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTAbstractRequest::setAsync(bool async)
{
  m_IsAsync = async;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTAbstractRequest::getAuthorityUrl() const
{
  return QString("%1/openid").arg(getConnection()->getBaseUrl());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTAbstractRequest::getAuthorityNoDnsUrl() const
{
  return QString("%1/openid").arg(getConnection()->getBaseUrl());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTAbstractRequest::getAuthorizationUrl() const
{
  return QString("%1/authorize").arg(getAuthorityNoDnsUrl());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTAbstractRequest::getUserInfoUrl() const
{
  return QString("%1/userinfo/").arg(getAuthorityUrl());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTAbstractRequest::getTokenUrl() const
{
  return QString("%1/token/").arg(getAuthorityUrl());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTAbstractRequest::getFilesApiUrl() const
{
  return QString("%1/api/files/").arg(getConnection()->getBaseUrl());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTAbstractRequest::getDownloadApiUrl() const
{
  return getFilesApiUrl() + "download/";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTAbstractRequest::waitForResponse(QNetworkReply* reply)
{
  // Asynchronous requests should not wait
  if(isAsync())
  {
    return;
  }

  QEventLoop loop;
  QObject::connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), &loop, &QEventLoop::quit);
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();
}
