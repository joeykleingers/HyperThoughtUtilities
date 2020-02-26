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

#include <QtCore/QDataStream>
#include <QtCore/QJsonObject>
#include <QtCore/QString>

#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

/**
 * @calass HTFilePath HTFilePath.h HyperThoughtUtilities/HyperThoughtConnection/HTFilePath.h
 * @brief The HTFilePath class contains information pertaining to files on the HyperThought server.
 */
class HyperThoughtUtilities_EXPORT HTFilePath
{
public:
  enum class ScopeType
  {
    User = 0,
    Group,
    Project
  };

  /**
   * @brief Default constructor
   */
  HTFilePath();

  /**
   * @brief Copy constructor
   * @param other
   */
  HTFilePath(const HTFilePath& other);

  /**
   * @brief Destructor
   */
  ~HTFilePath();

  /**
   * @brief Returns the scope name based on the current ScopeType value.
   * @return
   */
  QString getScopeName() const;

  /**
   * @brief Returns the current ScopeType value.
   * @return
   */
  ScopeType getScopeType() const;

  /**
   * @brief Sets the current ScopeType value.
   * @param type
   */
  void setScopeType(ScopeType type);

  /**
   * @brief Gets the current Source ID.
   * @return
   */
  QString getSourceId() const;

  /**
   * @brief Sets the current Source ID.
   * @param id
   */
  void setSourceId(const QString& id);

  /**
   * @brief Returns the current HyperThought Path value.
   * @return
   */
  QString getPath() const;

  /**
   * @brief Sets the HyperThought Path value.
   * @param path
   */
  void setPath(const QString& path);

  /**
   * @brief Returns the current username.
   * @return
   */
  QString getUsername() const;

  /**
   * @brief Sets the username.
   * @param name
   */
  void setUsername(const QString& name);

  /**
   * @brief Returns the path broken up into segments.
   * @return
   */
  QStringList getPathFragments() const;

  /**
   * @brief Creates an HTFilePath from the given json object.
   * @param json
   * @return
   */
  static HTFilePath FromJson(const QJsonObject& json);

  /**
   * @brief Creates a json object from the current values.
   * @return
   */
  QJsonObject toJson() const;

private:
  ScopeType m_SourceType = ScopeType::User;
  QString m_SourceId;
  QString m_Path;
  QString m_Username;
};

QDataStream& operator<<(QDataStream& out, const HTFilePath& myObj);
QDataStream& operator>>(QDataStream& in, HTFilePath& myObj);

Q_DECLARE_METATYPE(HTFilePath)
