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

#include <map>

#include <QtCore/QString>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfoTree.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTFilePath.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

/**
 * @class HTFileCache HTFileCache.h HyperThoughtUtilities/HyperThoughtConnection/HTFileCache.h
 * @brief The HTFileCache class serves as a cache for HTFileInfoTrees across different scopes and IDs.
 * Each group or project stored has their own space in the cache to allow multiple projects or groups
 * to be cached without overwriting previous collections assuming all IDs are different. If a new group
 * or project has the same ID as a previous item of the same Scope, the previous item is overwritten.
 */
class HyperThoughtUtilities_EXPORT HTFileCache
{
  using MapType = std::map<QString, HTFileInfoTree>;

public:
  HTFileCache();
  virtual ~HTFileCache();

  /**
   * @brief Checks if file info exists for the specified path.
   * Returns true if the data exists. Returns false otherwise.
   * @param path
   * @return
   */
  bool hasFileInfo(const HTFilePath& path) const;

  /**
   * @brief Returns the HTFileInfo for the item at the specified path.
   * Returns a default-constructed HTFileInfo if none was found.
   * @param path
   */
  HTFileInfo getFileInfo(const HTFilePath& path) const;

  /**
   * @brief Returns the HTFileInfoTree for the given source.
   * Only the ScopeType and optional SourceId are extracted from the source.
   * @param source
   * @return
   */
  HTFileInfoTree getFileInfoTree(const HTFilePath& source) const;

  /**
   * @brief Sets the HTFileInfoTree for the given source.
   * ScopeType and optional SourceId are taken from the source path for reference purposes.
   * @param source
   * @param infoTree
   */
  void setFileInfoTree(const HTFilePath& source, const HTFileInfoTree& infoTree);

  /**
   * @brief Checks if the cache contains information for the given group ID
   * @param id
   * @return
   */
  bool containsGroup(const QString& id) const;

  /**
   * @brief Checks if the cache contains information for the given project ID
   * @param id
   * @return
   */
  bool containsProject(const QString& id) const;

  /**
   * @brief Returns the group HTFileInfoTree for the given ID.
   * @param id
   * @return
   */
  HTFileInfoTree getGroupTree(const QString& id) const;

  /**
   * @brief Returns the project HTFileInfoTree for the given ID.
   * @param id
   * @return
   */
  HTFileInfoTree getProjectTree(const QString& id) const;

private:
  HTFileInfoTree m_UserInfoTree;
  MapType m_GroupInfoMap;
  MapType m_ProjectInfoMap;
};
