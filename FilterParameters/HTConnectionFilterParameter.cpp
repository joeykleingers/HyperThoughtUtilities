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

#include "HTConnectionFilterParameter.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>

#if 0
namespace
{
const QString AuthUrl = "AuthUrl";
const QString AuthIpUrl = "AuthIpUrl";
const QString ClientId = "ClientId";

// -----------------------------------------------------------------------------
HTConnection* CreateFromJson(const QJsonObject& json)
{
  QString authUrl = json[AuthUrl].toString();
  QString ipUrl = json[AuthIpUrl].toString();
  QString clientId = json[ClientId].toString();

  return new HTConnection(authUrl, ipUrl, clientId);
}

// -----------------------------------------------------------------------------
QJsonObject WriteToJson(HTConnection* connection)
{
  QJsonObject json;

  json[AuthUrl] = connection->getAuthUrl();
  json[AuthIpUrl] = connection->getAuthIpUrl();
  json[ClientId] = connection->getClientId();

  return json;
}
} // namespace
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnectionFilterParameter::HTConnectionFilterParameter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnectionFilterParameter::~HTConnectionFilterParameter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnectionFilterParameter::Pointer HTConnectionFilterParameter::New(const QString& humanLabel, const QString& propertyName, const QString& defaultValue, Category category,
                                                                      SetterCallbackType setterCallback, GetterCallbackType getterCallback)
{
  HTConnectionFilterParameter::Pointer ptr = HTConnectionFilterParameter::New();
  ptr->setHumanLabel(humanLabel);
  ptr->setPropertyName(propertyName);
  QVariant v;
  v.setValue(defaultValue);
  ptr->setDefaultValue(v);
  ptr->setCategory(category);
  ptr->setSetterCallback(setterCallback);
  ptr->setGetterCallback(getterCallback);

  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTConnectionFilterParameter::getWidgetType() const
{
  return QString("HTConnectionWidget");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionFilterParameter::readJson(const QJsonObject& json)
{
#if 0
  QJsonValue jsonValue = json[getPropertyName()];
  if(!jsonValue.isUndefined() && m_SetterCallback)
  {
    QJsonObject jsonObj = jsonValue.toObject();
    HTConnection* connection = CreateFromJson(jsonObj);
    m_SetterCallback(connection);
  }
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionFilterParameter::writeJson(QJsonObject& json)
{
#if 0
  if(m_GetterCallback)
  {
    HTConnection* connection = m_GetterCallback();
    QJsonObject jsonObj = WriteToJson(connection);
    json[getPropertyName()] = jsonObj;
  }
#endif
}

// -----------------------------------------------------------------------------
HTConnectionFilterParameter::Pointer HTConnectionFilterParameter::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
HTConnectionFilterParameter::Pointer HTConnectionFilterParameter::New()
{
  Pointer sharedPtr(new(HTConnectionFilterParameter));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
QString HTConnectionFilterParameter::getNameOfClass() const
{
  return QString("HTConnectionFilterParameter");
}

// -----------------------------------------------------------------------------
QString HTConnectionFilterParameter::ClassName()
{
  return QString("HTConnectionFilterParameter");
}

// -----------------------------------------------------------------------------
void HTConnectionFilterParameter::setSetterCallback(const HTConnectionFilterParameter::SetterCallbackType& value)
{
  m_SetterCallback = value;
}

// -----------------------------------------------------------------------------
HTConnectionFilterParameter::SetterCallbackType HTConnectionFilterParameter::getSetterCallback() const
{
  return m_SetterCallback;
}

// -----------------------------------------------------------------------------
void HTConnectionFilterParameter::setGetterCallback(const HTConnectionFilterParameter::GetterCallbackType& value)
{
  m_GetterCallback = value;
}

// -----------------------------------------------------------------------------
HTConnectionFilterParameter::GetterCallbackType HTConnectionFilterParameter::getGetterCallback() const
{
  return m_GetterCallback;
}
