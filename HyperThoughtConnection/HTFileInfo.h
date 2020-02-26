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

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QMap>
#include <QtCore/QMetaType>
#include <QtCore/QVector>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTMetaData.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

/**
 * @class HTFileInfo HTFileInfo.h HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTFileInfo.h
 * @brief The HTFileInfo class is used to extract HyperThought data about a file on the server.
 */
class HyperThoughtUtilities_EXPORT HTFileInfo
{
public:
  using NameValuePairs = QMap<QString, QString>;

  struct Content
  {
    int numItems = 0;
    QString fileId;
    QString path;
    QString name;
    int size = 0;
    QString pk;
    QString backend;
    QString createdDate;
    QString createdBy;
    QString modifiedDate;
    QString modifiedBy;
    QString fileType;
    QString pathStr;
  };
  struct Permissions
  {
    NameValuePairs groups;
    NameValuePairs projects;
    NameValuePairs users;
  };
  NameValuePairs Restrictions;

  /**
   * @brief Default constructor
   */
  HTFileInfo();

  /**
   * @brief Constructor
   * @param json
   */
  HTFileInfo(const QJsonObject& json);

  /**
   * @brief Copy Constructor
   * @param rhs
   */
  HTFileInfo(const HTFileInfo& rhs);

  /**
   * @brief Move Constructor
   * @param other
   */
  HTFileInfo(HTFileInfo&& other);

  /**
   * @brief Destructor
   */
  ~HTFileInfo();

  /**
   * @brief Constructs a vector of HTFiles from the given json document.
   * @param doc
   * @return
   */
  static std::vector<HTFileInfo> FromDocument(const QJsonDocument& doc);

  /**
   * @brief Returns the ID value.
   * @return
   */
  QString getId() const;

  /**
   * @brief Returns the file's HyperThought path.
   * @return
   */
  QString getPath() const;

  /**
   * @brief Returns the file's HyperThought path broken up by pk values.
   * @return
   */
  QStringList getPathFragments() const;

  /**
   * @brief Checks if the target is a directory. Returns true if it is. Returns false otherwise.
   * @return
   */
  bool isDir() const;

  /**
   * @brief Returns true if the file has a parent.
   * Returns false if this is a top-level item.
   * @return
   */
  bool hasParent() const;

  /**
   * @brief Returns the parent directory's ID from the path.
   * @return
   */
  QString getParentId() const;

  /**
   * @brief Returns the file's name.
   * @return
   */
  QString getFileName() const;

  /**
   * @brief Returns file content information.
   * @return
   */
  Content getContent() const;

  /**
   * @brief Returns file meta data.
   * @return
   */
  HTMetaData getMetaData() const;

  /**
   * @brief Returns file permissions.
   * @return
   */
  Permissions getPermissions() const;

  /**
   * @brief Returns file restrictions.
   * @return
   */
  NameValuePairs getRestrictions() const;

  /**
   * @brief Sets the local content data.
   * @param value
   */
  void setContent(const Content& data);

  /**
   * @brief Sets the local permissions data.
   * @param value
   */
  void setPermissions(const Permissions& data);

  /**
   * @brief Sets the local restrictions data.
   * @param value
   */
  void setRestrictions(const NameValuePairs& value);

  /**
   * @brief Sets the local meta data.
   * @param data
   */
  void setMetaData(const HTMetaData& data);

  /**
   * @brief Checks if the file has the given meta data key.
   * @param key
   * @return
   */
  bool hasMetaDataKey(const QString& key);

  /**
   * @brief Sets the given meta data value.
   * Overwrites existing data if the key is in use.
   * @param key
   * @param value
   */
  void setMetaDataValue(const QString& key, const QString& value);

  /**
   * @brief Removes the given meta data key.
   * @param key
   */
  void removeMetaDataKey(const QString& key);

  /**
   * @brief Returns json representation of the HTFileInfo
   */
  QJsonObject toJson() const;

  /**
   * @brief Assignment operator.
   * @param other
   * @return
   */
  HTFileInfo& operator=(const HTFileInfo& other);

  /**
   * @brief Move operator
   * @param other
   * @return
   */
  HTFileInfo& operator=(HTFileInfo&& other);

private:
  /**
   * @brief Parses the provided json object for information about the HyperThought file.
   * @param json
   */
  void fromJson(const QJsonObject& json);

  /**
   * @brief Parses the provided json object for content information.
   * @param json
   */
  void parseContent(const QJsonObject& json);

  /**
   * @brief Parses the provided json array for meta data information.
   * @param json
   */
  void parseMetaData(const QJsonArray& json);

  /**
   * @brief Parses the provided json object for permissions information.
   * @param jsons
   */
  void parsePermission(const QJsonObject& json);

  /**
   * @brief Parses the provided json object for restrictions information.
   * @param json
   */
  void parseRestrictions(const QJsonObject& json);

  /**
   * @brief Writes and returns content information to a QJsonObject.
   * @return
   */
  QJsonObject writeContentToJson() const;

  /**
   * @brief Writes and returns meta data information to a QJsonObject.
   * @return
   */
  QJsonArray writeMetaDataToJson() const;

  /**
   * @brief Writes and returns permissions information to a QJsonObject.
   * @return
   */
  QJsonObject writePermissionsToJson() const;

  /**
   * @brief Writes and returns restrictions information to a QJsonObject.
   * @return
   */
  QJsonObject writeRestrictionsToJson() const;

  // -----------------------------------------------------------------------------
  // Variables
  Content m_Content;
  HTMetaData m_MetaData;
  Permissions m_Permissions;
  NameValuePairs m_Restrictions;
};

/**
 * @brief HTFileInfo equality operator.
 * Equality is checked using only their ID values.
 * @param lhs
 * @param rhs
 * @return
 */
bool operator==(const HTFileInfo& lhs, const HTFileInfo& rhs);

QDataStream& operator<<(QDataStream& out, const HTFileInfo& myObj);
QDataStream& operator>>(QDataStream& in, HTFileInfo& myObj);

Q_DECLARE_METATYPE(HTFileInfo)
