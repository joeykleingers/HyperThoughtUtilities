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

#include "HTMetaData.h"

#include <QtCore/QDataStream>
#include <QtCore/QJsonObject>
#include <QtCore/QSet>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData::HTMetaData()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData::HTMetaData(const QJsonArray& json)
{
  for(const auto& obj : json)
  {
    QString key = obj["keyName"].toString();
    QJsonObject valueObj = obj["value"].toObject();
    QString value = valueObj["link"].toString();

    m_Values[key] = value;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData::HTMetaData(const HTMetaData& rhs)
: m_Values(rhs.m_Values)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData::~HTMetaData() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool HTMetaData::hasKey(const QString& key) const
{
  return m_Values.contains(key);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList HTMetaData::getKeys() const
{
  return m_Values.keys();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTMetaData::getValue(const QString& key) const
{
  return m_Values.value(key);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaData::setValue(const QString& key, const QString& value)
{
  m_Values[key] = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaData::removeKey(const QString& key)
{
  m_Values.remove(key);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTMetaData::operator[](const QString& key) const
{
  return m_Values.value(key);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString& HTMetaData::operator[](const QString& key)
{
  return m_Values[key];
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData::const_iterator HTMetaData::begin() const
{
  return m_Values.begin();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData::const_iterator HTMetaData::end() const
{
  return m_Values.end();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int HTMetaData::size() const
{
  return m_Values.size();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QJsonArray HTMetaData::toJson() const
{
  QJsonArray json;
  for(auto iter = m_Values.begin(); iter != m_Values.end(); iter++)
  {
    QJsonObject obj;
    obj["keyName"] = iter.key();

    QJsonObject valueObj;
    valueObj["type"] = "string";
    valueObj["link"] = iter.value();
    obj["value"] = valueObj;
    json.push_back(obj);
  }
  return json;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QJsonObject HTMetaData::toJsonDict() const
{
  QJsonObject json;
  for(auto iter = m_Values.begin(); iter != m_Values.end(); iter++)
  {
    json[iter.key()] = iter.value();
  }
  return json;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList getOverlap(const QStringList& first, const QStringList& second)
{
  QSet<QString> intersection = first.toSet().intersect(second.toSet());
  return intersection.toList();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList subtract(const QStringList& first, const QStringList& second)
{
  QSet<QString> result = first.toSet().subtract(second.toSet());
  return result.toList();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData::NameValuePairs HTMetaData::GetUpdatedPairs(const HTMetaData& start, const HTMetaData& end)
{
  NameValuePairs updatedValues;

  QStringList sharedKeys = getOverlap(start.getKeys(), end.getKeys());
  for(const QString& key : sharedKeys)
  {
    if(start[key] != end[key])
    {
      updatedValues[key] = end[key];
    }
  }

  // "Update" new keys?
  QStringList newKeys = subtract(end.getKeys(), start.getKeys());
  for(const QString& key : newKeys)
  {
    updatedValues[key] = end[key];
  }

  return updatedValues;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList HTMetaData::GetRemovedKeys(const HTMetaData& start, const HTMetaData& end)
{
  return subtract(start.getKeys(), end.getKeys());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData::NameValuePairs HTMetaData::GetCreatedPairs(const HTMetaData& start, const HTMetaData& end)
{
  NameValuePairs createdValues;

  QStringList newKeys = subtract(end.getKeys(), start.getKeys());
  for(const QString& key : newKeys)
  {
    createdValues[key] = end[key];
  }
  return createdValues;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QDataStream& operator<<(QDataStream& out, const HTMetaData& v)
{
  out << v.size();
  for(auto iter = v.begin(); iter != v.end(); iter++)
  {
    out << iter.key() << iter.value();
  }

  return out;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QDataStream& operator>>(QDataStream& in, HTMetaData& v)
{
  int size;
  in >> size;
  QString key;
  QString value;

  for(int i = 0; i < size; i++)
  {
    in >> key >> value;
  }
  return in;
}
