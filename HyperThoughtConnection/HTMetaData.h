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

#include <QtCore/QJsonArray>
#include <QtCore/QMap>
#include <QtCore/QMetaType>
#include <QtCore/QString>

#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

/**
 * @class HTMetaData HTMetaData.h HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTMetaData.h
 * @brief The HTMetaData class is for storing, accessing, and setting
 * HyperThought metadata.
 */
class HyperThoughtUtilities_EXPORT HTMetaData
{
public:
  using NameValuePairs = QMap<QString, QString>;
  using const_iterator = NameValuePairs::const_iterator;

  HTMetaData();
  HTMetaData(const QJsonArray& json);
  HTMetaData(const HTMetaData& rhs);
  ~HTMetaData();

  /**
   * @brief Checks if the given key is used.
   * @param key
   * @return
   */
  bool hasKey(const QString& key) const;

  /**
   * @brief Returns all keys in use.
   * @return
   */
  QStringList getKeys() const;

  /**
   * @brief Returns the metadata value for the given key.
   * Returns an empty QString if the key does not exist.
   * @aparam key
   * @return
   */
  QString getValue(const QString& key) const;

  /**
   * @brief Sets the metadata value for the given key. Creates a new key-value pair if required.
   * @param key
   * @param value
   */
  void setValue(const QString& key, const QString& value);

  /**
   * @brief Removes the metadata value for the given key.
   * @param key
   */
  void removeKey(const QString& key);

  /**
   * @brief Returns the metadata value for the given key.
   * Returns an empty value if the key does not exist.
   * @param key
   * @return
   */
  QString operator[](const QString& key) const;

  /**
   * @brief Returns the metadata value for the given key.
   * Inserts an empty value if the key does not exist.
   * @param key
   * @return
   */
  QString& operator[](const QString& key);

  /**
   * @brief Returns stl const_iterator.
   * @return
   */
  const_iterator begin() const;

  /**
   * @brief Returns stl const_iterator.
   * @return
   */
  const_iterator end() const;

  /**
   * @brief Returns the number of items.
   * @return
   */
  int size() const;

  /**
   * @brief Returns the json representation of the current values as an array
   * of key, value, type objects. For a simple dictionary lookup, use
   * toJsonDict().
   * @return
   */
  QJsonArray toJson() const;

  /**
   * @brief Returns a json representing the key-value map of values.
   * Unlike toJson, this returns a QJsonObject and compresses the data to a
   * single layer instead of an array of key, value, type objects.
   * @return
   */
  QJsonObject toJsonDict() const;

  /**
   * @brief Returns a map of updated values between two HTMetaData objects.
   * @param start
   * @param end
   * @return
   */
  static NameValuePairs GetUpdatedPairs(const HTMetaData& start, const HTMetaData& end);

  /**
   * @brief Returns a list of keys removed between start and end HTMetaData objects.
   * @param start
   * @param end
   * @return
   */
  static QStringList GetRemovedKeys(const HTMetaData& start, const HTMetaData& end);

  /**
   * @brief Returns a map of created pairs between start and end HTMetaData objects.
   * @param start
   * @param end
   * @return
   */
  static NameValuePairs GetCreatedPairs(const HTMetaData& start, const HTMetaData& end);

  HTMetaData& operator=(const HTMetaData& fileInfo) = default; // Assignment Operator Not Implemented
  HTMetaData(HTMetaData&&) = default;                          // Move Constructor Not Implemented
  HTMetaData& operator=(HTMetaData&&) = default;               // Move Assignment Not Implemented

private:
  NameValuePairs m_Values;
};

QDataStream& operator<<(QDataStream& out, const HTMetaData& v);
QDataStream& operator>>(QDataStream& in, HTMetaData& v);

Q_DECLARE_METATYPE(HTMetaData)
