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

#include "HTMetaDataFilterParameter.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaDataFilterParameter::HTMetaDataFilterParameter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaDataFilterParameter::~HTMetaDataFilterParameter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaDataFilterParameter::Pointer HTMetaDataFilterParameter::New(const QString& humanLabel, const QString& propertyName, const HTMetaData& defaultValue, Category category,
                                                                  SetterCallbackType setterCallback, GetterCallbackType getterCallback)
{
  HTMetaDataFilterParameter::Pointer ptr = HTMetaDataFilterParameter::New();
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
QString HTMetaDataFilterParameter::getWidgetType() const
{
  return QString("HTMetaDataWidget");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataFilterParameter::readJson(const QJsonObject& json)
{
  QJsonValue jsonValue = json[getPropertyName()];
  if(!jsonValue.isUndefined() && m_SetterCallback)
  {
    QJsonArray jsonArr = jsonValue.toArray();
    m_SetterCallback(HTMetaData(jsonArr));
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataFilterParameter::writeJson(QJsonObject& json)
{
  if(m_GetterCallback)
  {
    HTMetaData metaData = m_GetterCallback();
    json[getPropertyName()] = metaData.toJson();
  }
}

// -----------------------------------------------------------------------------
HTMetaDataFilterParameter::Pointer HTMetaDataFilterParameter::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
HTMetaDataFilterParameter::Pointer HTMetaDataFilterParameter::New()
{
  Pointer sharedPtr(new(HTMetaDataFilterParameter));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
QString HTMetaDataFilterParameter::getNameOfClass() const
{
  return QString("HTMetaDataFilterParameter");
}

// -----------------------------------------------------------------------------
QString HTMetaDataFilterParameter::ClassName()
{
  return QString("HTMetaDataFilterParameter");
}

// -----------------------------------------------------------------------------
void HTMetaDataFilterParameter::setSetterCallback(const HTMetaDataFilterParameter::SetterCallbackType& value)
{
  m_SetterCallback = value;
}

// -----------------------------------------------------------------------------
HTMetaDataFilterParameter::SetterCallbackType HTMetaDataFilterParameter::getSetterCallback() const
{
  return m_SetterCallback;
}

// -----------------------------------------------------------------------------
void HTMetaDataFilterParameter::setGetterCallback(const HTMetaDataFilterParameter::GetterCallbackType& value)
{
  m_GetterCallback = value;
}

// -----------------------------------------------------------------------------
HTMetaDataFilterParameter::GetterCallbackType HTMetaDataFilterParameter::getGetterCallback() const
{
  return m_GetterCallback;
}
