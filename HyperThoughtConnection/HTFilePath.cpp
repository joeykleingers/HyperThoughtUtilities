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

#include "HTFilePath.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath::HTFilePath()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath::HTFilePath(const HTFilePath& other)
: m_SourceType(other.m_SourceType)
, m_SourceId(other.m_SourceId)
, m_Username(other.m_Username)
, m_Path(other.m_Path)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath::~HTFilePath() = default;

// -----------------------------------------------------------------------------
QString HTFilePath::getScopeName() const
{
  switch(m_SourceType)
  {
  case ScopeType::User:
    return "user";
  case ScopeType::Group:
    return "group";
  case ScopeType::Project:
    return "project";
  }
  return QString();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath::ScopeType HTFilePath::getScopeType() const
{
  return m_SourceType;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePath::setScopeType(ScopeType type)
{
  m_SourceType = type;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTFilePath::getSourceId() const
{
  return m_SourceId;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePath::setSourceId(const QString& id)
{
  m_SourceId = id;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTFilePath::getPath() const
{
  return m_Path;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePath::setPath(const QString& path)
{
  m_Path = path;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HTFilePath::getUsername() const
{
  return m_Username;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePath::setUsername(const QString& name)
{
  m_Username = name;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList HTFilePath::getPathFragments() const
{
  QStringList pathFragments = m_Path.split(",", QString::SkipEmptyParts);
  return pathFragments;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath HTFilePath::FromJson(const QJsonObject& json)
{
  int typei = json.value("SourceType").toInt();
  QString id = json.value("SourceId").toString();
  QString path = json.value("Path").toString();
  QString username = json.value("Username").toString();

  HTFilePath filePath;
  filePath.setScopeType(static_cast<ScopeType>(typei));
  filePath.setSourceId(id);
  filePath.setPath(path);
  filePath.setUsername(username);
  return filePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QJsonObject HTFilePath::toJson() const
{
  QJsonObject json;
  json["SourceType"] = static_cast<int>(m_SourceType);
  json["SourceId"] = m_SourceId;
  json["Username"] = m_Username;
  json["Path"] = m_Path;
  return json;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QDataStream& operator<<(QDataStream& out, const HTFilePath& myObj)
{
  out << static_cast<int>(myObj.getScopeType()) << myObj.getSourceId() << myObj.getPath() << myObj.getUsername();
  return out;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QDataStream& operator>>(QDataStream& in, HTFilePath& myObj)
{
  int sourceType;
  QString sourceId;
  QString path;
  QString username;
  in >> sourceType >> sourceId >> path >> username;

  myObj.setScopeType(static_cast<HTFilePath::ScopeType>(sourceType));
  myObj.setSourceId(sourceId);
  myObj.setPath(path);
  myObj.setUsername(username);
  return in;
}
