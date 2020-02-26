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

#include "HTFileInfo.h"

#include <QtCore/QDataStream>
#include <QtCore/QJsonArray>

namespace FileTags
{
const QString Content = "content";
const QString Triples = "triples";
const QString Headers = "headers";
const QString MetaData = "metadata";
const QString Permissions = "permissions";
const QString Restrictions = "restrictions";
} // namespace FileTags

namespace ContentTags
{
const QString NumItems = "items";
const QString FileId = "file";
const QString ModifiedDate = "modified";
const QString ModifiedBy = "modified_by";
const QString Path = "path";
const QString CreatedDate = "created";
const QString CreatedBy = "created_by";
const QString FileName = "name";
const QString PK = "pk";
const QString Backend = "backend";
const QString FileType = "ftype";
const QString PathStr = "path_string";
const QString Size = "size";
} // namespace ContentTags

namespace PermissionsTags
{
const QString Groups = "groups";
const QString Projects = "projects";
const QString Users = "users";
} // namespace PermissionsTags

// -----------------------------------------------------------------------------
HTFileInfo::HTFileInfo()
{
}

// -----------------------------------------------------------------------------
HTFileInfo::HTFileInfo(const QJsonObject& json)
{
  fromJson(json);
}

// -----------------------------------------------------------------------------
HTFileInfo::HTFileInfo(const HTFileInfo& rhs)
: m_Content(rhs.m_Content)
, m_MetaData(rhs.m_MetaData)
, m_Permissions(rhs.m_Permissions)
, m_Restrictions(rhs.m_Restrictions)
{
}

// -----------------------------------------------------------------------------
HTFileInfo::HTFileInfo(HTFileInfo&& other)
: m_Content(std::move(other.m_Content))
, m_MetaData(std::move(other.m_MetaData))
, m_Permissions(std::move(other.m_Permissions))
, m_Restrictions(std::move(other.m_Restrictions))
{
}

// -----------------------------------------------------------------------------
HTFileInfo::~HTFileInfo() = default;

// -----------------------------------------------------------------------------
std::vector<HTFileInfo> HTFileInfo::FromDocument(const QJsonDocument& doc)
{
  std::vector<HTFileInfo> files;
  QJsonArray arr = doc.array();
  for(const QJsonValueRef& val : arr)
  {
    files.emplace_back(val.toObject());
  }
  return files;
}

// -----------------------------------------------------------------------------
QString HTFileInfo::getId() const
{
  return m_Content.pk;
}

// -----------------------------------------------------------------------------
QString HTFileInfo::getPath() const
{
  return m_Content.path;
}

// -----------------------------------------------------------------------------
QStringList HTFileInfo::getPathFragments() const
{
  return getPath().split(",", QString::SkipEmptyParts);
}

// -----------------------------------------------------------------------------
bool HTFileInfo::isDir() const
{
  return m_Content.fileType == "Folder";
}

// -----------------------------------------------------------------------------
bool HTFileInfo::hasParent() const
{
  return m_Content.path != ",";
}

// -----------------------------------------------------------------------------
QString HTFileInfo::getParentId() const
{
  QStringList pathFragments = getPathFragments();
  int size = pathFragments.size();
  if(size < 1)
  {
    return "";
  }
  return pathFragments[size - 1];
}

// -----------------------------------------------------------------------------
QString HTFileInfo::getFileName() const
{
  return m_Content.name;
}

// -----------------------------------------------------------------------------
HTFileInfo::Content HTFileInfo::getContent() const
{
  return m_Content;
}

// -----------------------------------------------------------------------------
HTMetaData HTFileInfo::getMetaData() const
{
  return m_MetaData;
}

// -----------------------------------------------------------------------------
HTFileInfo::Permissions HTFileInfo::getPermissions() const
{
  return m_Permissions;
}

// -----------------------------------------------------------------------------
HTFileInfo::NameValuePairs HTFileInfo::getRestrictions() const
{
  return m_Restrictions;
}

#if 1
// -----------------------------------------------------------------------------
void HTFileInfo::setContent(const Content& data)
{
  m_Content = data;
}

// -----------------------------------------------------------------------------
void HTFileInfo::setPermissions(const Permissions& data)
{
  m_Permissions = data;
}

// -----------------------------------------------------------------------------
void HTFileInfo::setRestrictions(const NameValuePairs& value)
{
  m_Restrictions = value;
}
#endif

// -----------------------------------------------------------------------------
void HTFileInfo::setMetaData(const HTMetaData& data)
{
  m_MetaData = data;
}

// -----------------------------------------------------------------------------
bool HTFileInfo::hasMetaDataKey(const QString& key)
{
  return m_MetaData.hasKey(key);
}

// -----------------------------------------------------------------------------
void HTFileInfo::setMetaDataValue(const QString& key, const QString& value)
{
  m_MetaData.setValue(key, value);
}

// -----------------------------------------------------------------------------
void HTFileInfo::removeMetaDataKey(const QString& tag)
{
  m_MetaData.removeKey(tag);
}

// -----------------------------------------------------------------------------
QJsonObject HTFileInfo::toJson() const
{
  QJsonObject json;
  json[FileTags::Content] = writeContentToJson();
  json[FileTags::MetaData] = writeMetaDataToJson();
  json[FileTags::Permissions] = writePermissionsToJson();
  json[FileTags::Restrictions] = writeRestrictionsToJson();
  return json;
}

// -----------------------------------------------------------------------------
void HTFileInfo::fromJson(const QJsonObject& json)
{
  QJsonObject content = json[FileTags::Content].toObject();
  QJsonArray metaData = json[FileTags::MetaData].toArray();
  QJsonObject permissions = json[FileTags::Permissions].toObject();
  QJsonObject restrictions = json[FileTags::Restrictions].toObject();

  parseContent(content);
  parseMetaData(metaData);
  parsePermission(permissions);
  parseRestrictions(restrictions);
}

// -----------------------------------------------------------------------------
void HTFileInfo::parseContent(const QJsonObject& json)
{
  m_Content.numItems = json[ContentTags::NumItems].toInt();
  m_Content.fileId = json[ContentTags::FileId].toString();
  m_Content.path = json[ContentTags::Path].toString();
  m_Content.name = json[ContentTags::FileName].toString();
  m_Content.fileType = json[ContentTags::FileType].toString();
  m_Content.pathStr = json[ContentTags::PathStr].toString();
  m_Content.size = json[ContentTags::Size].toInt();
  m_Content.pk = json[ContentTags::PK].toString();
  m_Content.backend = json[ContentTags::Backend].toString();
  m_Content.createdDate = json[ContentTags::CreatedDate].toString();
  m_Content.createdBy = json[ContentTags::CreatedBy].toString();
  m_Content.modifiedDate = json[ContentTags::ModifiedDate].toString();
  m_Content.modifiedBy = json[ContentTags::ModifiedBy].toString();
}

// -----------------------------------------------------------------------------
void HTFileInfo::parseMetaData(const QJsonArray& json)
{
  m_MetaData = HTMetaData(json);
}

// -----------------------------------------------------------------------------
HTFileInfo::NameValuePairs parseNameValuePairs(const QJsonObject& json)
{
  HTFileInfo::NameValuePairs values;
  for(auto& iter = json.begin(); iter != json.end(); iter++)
  {
    values[iter.key()] = iter.value().toString();
  }
  return values;
}

// -----------------------------------------------------------------------------
void HTFileInfo::parsePermission(const QJsonObject& json)
{
  m_Permissions.groups = parseNameValuePairs(json[PermissionsTags::Groups].toObject());
  m_Permissions.projects = parseNameValuePairs(json[PermissionsTags::Projects].toObject());
  m_Permissions.users = parseNameValuePairs(json[PermissionsTags::Users].toObject());
}

// -----------------------------------------------------------------------------
void HTFileInfo::parseRestrictions(const QJsonObject& json)
{
  m_Restrictions = parseNameValuePairs(json["restrictions"].toObject());
}

// -----------------------------------------------------------------------------
QJsonObject writeNamedValuesToJson(const HTFileInfo::NameValuePairs& valuePairs)
{
  QJsonObject json;
  for(auto& iter = valuePairs.begin(); iter != valuePairs.end(); iter++)
  {
    json[iter.key()] = iter.value();
  }
  return json;
}

// -----------------------------------------------------------------------------
QJsonObject HTFileInfo::writeContentToJson() const
{
  QJsonObject json;
  json[ContentTags::NumItems] = m_Content.numItems;
  json[ContentTags::FileId] = m_Content.fileId;
  json[ContentTags::Path] = m_Content.path;
  json[ContentTags::FileName] = m_Content.name;
  json[ContentTags::FileType] = m_Content.fileType;
  json[ContentTags::PathStr] = m_Content.pathStr;
  json[ContentTags::Size] = m_Content.size;
  json[ContentTags::PK] = m_Content.pk;
  json[ContentTags::Backend] = m_Content.backend;
  json[ContentTags::CreatedDate] = m_Content.createdDate;
  json[ContentTags::CreatedBy] = m_Content.createdBy;
  json[ContentTags::ModifiedDate] = m_Content.modifiedDate;
  json[ContentTags::ModifiedBy] = m_Content.modifiedBy;
  return json;
}

// -----------------------------------------------------------------------------
QJsonArray HTFileInfo::writeMetaDataToJson() const
{
  return m_MetaData.toJson();
}

// -----------------------------------------------------------------------------
QJsonObject HTFileInfo::writePermissionsToJson() const
{
  QJsonObject json;
  json[PermissionsTags::Groups] = writeNamedValuesToJson(m_Permissions.groups);
  json[PermissionsTags::Projects] = writeNamedValuesToJson(m_Permissions.projects);
  json[PermissionsTags::Users] = writeNamedValuesToJson(m_Permissions.users);
  return json;
}

// -----------------------------------------------------------------------------
QJsonObject HTFileInfo::writeRestrictionsToJson() const
{
  return writeNamedValuesToJson(m_Restrictions);
}

// -----------------------------------------------------------------------------
HTFileInfo& HTFileInfo::operator=(const HTFileInfo& other)
{
  m_Content = other.m_Content;
  m_MetaData = other.m_MetaData;
  m_Permissions = other.m_Permissions;
  m_Restrictions = other.Restrictions;
  return *this;
}

// -----------------------------------------------------------------------------
HTFileInfo& HTFileInfo::operator=(HTFileInfo&& other)
{
  m_Content = std::move(other.m_Content);
  m_MetaData = std::move(other.m_MetaData);
  m_Permissions = std::move(other.m_Permissions);
  m_Restrictions = std::move(other.Restrictions);
  return *this;
}

// -----------------------------------------------------------------------------
bool operator==(const HTFileInfo& lhs, const HTFileInfo& rhs)
{
  return lhs.getId() == rhs.getId();
}

// -----------------------------------------------------------------------------
QDataStream& operator<<(QDataStream& out, const HTFileInfo& v)
{
  // Content
  auto content = v.getContent();
  out << content.fileId << content.path << content.name << content.size << content.numItems;
  out << content.pk << content.backend << content.createdDate << content.createdBy;
  out << content.modifiedDate << content.modifiedBy << content.fileType << content.pathStr;

  // MetaData
  auto metaData = v.getMetaData();
  out << metaData.size();
  for(auto iter = metaData.begin(); iter != metaData.end(); iter++)
  {
    out << iter.key() << iter.value();
  }

  // Permissions
  auto permissions = v.getPermissions();
  auto groups = permissions.groups;
  out << groups.size();
  for(auto iter = groups.begin(); iter != groups.end(); iter++)
  {
    out << iter.key() << iter.value();
  }

  auto projects = permissions.projects;
  out << projects.size();
  for(auto iter = projects.begin(); iter != projects.end(); iter++)
  {
    out << iter.key() << iter.value();
  }

  auto users = permissions.users;
  out << users.size();
  for(auto iter = users.begin(); iter != users.end(); iter++)
  {
    out << iter.key() << iter.value();
  }

  // Restrictions
  auto restrictions = v.getRestrictions();
  out << restrictions.size();
  for(auto iter = restrictions.begin(); iter != restrictions.end(); iter++)
  {
    out << iter.key() << iter.value();
  }

  return out;
}

// -----------------------------------------------------------------------------
QDataStream& operator>>(QDataStream& in, HTFileInfo& v)
{
  HTFileInfo::Content content;
  in >> content.fileId >> content.path >> content.name >> content.size >> content.numItems;
  in >> content.pk >> content.backend >> content.createdDate >> content.createdBy;
  in >> content.modifiedDate >> content.modifiedBy >> content.fileType >> content.pathStr;
  v.setContent(content);

  int size;
  in >> size;
  HTMetaData metaData;
  for(int i = 0; i < size; i++)
  {
    QString key;
    QString value;
    in >> key >> value;
    metaData.setValue(key, value);
  }
  v.setMetaData(metaData);

  // Permissions
  HTFileInfo::Permissions permissions;
  // Permissions: Groups
  in >> size;
  auto& groups = permissions.groups;
  for(int i = 0; i < size; i++)
  {
    QString key;
    QString value;
    in >> key >> value;
    groups[key] = value;
  }

  // Permissions: Projects
  in >> size;
  auto& projects = permissions.projects;
  for(int i = 0; i < size; i++)
  {
    QString key;
    QString value;
    in >> key >> value;
    projects[key] = value;
  }

  // Permissions: Users
  in >> size;
  auto& users = permissions.users;
  for(int i = 0; i < size; i++)
  {
    QString key;
    QString value;
    in >> key >> value;
    users[key] = value;
  }
  v.setPermissions(permissions);

  // Restrictions
  in >> size;
  HTFileInfo::NameValuePairs restrictions;
  for(int i = 0; i < size; i++)
  {
    QString key;
    QString value;
    in >> key >> value;
    restrictions[key] = value;
  }
  v.setRestrictions(restrictions);

  return in;
}
