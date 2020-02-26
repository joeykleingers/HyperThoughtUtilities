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

#include <vector>

#include <QtCore/QAbstractItemModel>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfoTree.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

/**
 * @class HTFileInfoModel HTFileInfoModel.h HyperThoughtUtilities/HyperThoughtConnection/HTFileInfoModel.h
 * @brief The HTFileInfoModel class is derived from QAbstractItemModel to allow viewing
 * files and directories within HyperThought Groups, Projects, and User space.
 */
class HyperThoughtUtilities_EXPORT HTFileInfoModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  enum class Mode
  {
    Normal,
    Directory
  };

  /**
   * @brief Default constructor
   * @param parent
   */
  HTFileInfoModel(QObject* parent = nullptr);

  /**
   * @brief Destructor
   */
  ~HTFileInfoModel() override;

  /**
   * @brief Returns the HTFileInfo for the given index.
   * Creates an empty HTFileInfo if one could not be found.
   * @param index
   * @return
   */
  HTFileInfo getFileInfo(const QModelIndex& index) const;

  /**
   * @brief Returns a vector of HTFileInfo from the children of the specified parent index.
   * @param parent
   * @return
   */
  std::vector<HTFileInfo> getChildItems(const QModelIndex& parent) const;

  /**
   * @brief Returns a vector of HTFileInfo from the children of the specified HTFileInfo.
   * @param parent
   * @return
   */
  std::vector<HTFileInfo> getChildItems(const HTFileInfo& parent) const;

  /**
   * @brief Finds the QModelIndex for the specified HTFilePath.
   * @param path
   * @return
   */
  QModelIndex findIndex(const HTFilePath& path) const;

  /**
   * @brief Checks if the HTFileInfo at the given index is a directory.
   * @param index
   * @return
   */
  bool isDir(const QModelIndex& index) const;

  /**
   * @brief Sets the HTFileInfoTree used as the basis for the model.
   * Resets the model, invalidating the current indices and updating all views using it.
   * @param tree
   */
  void setFileInfoTree(const HTFileInfoTree& tree);

  /**
   * @brief Returns the Mode.
   * @return
   */
  Mode getMode() const;

  /**
   * @brief Sets the mode for the item model. The mode determines which flags are enabled for items.
   * The Normal mode returns default flags for all items.
   * The Directory mode disables and prevents the selection of non-directory items.
   * @param mode.
   */
  void setMode(Mode mode);

  // QAbstractItemModel API
  /**
   * @brief Returns the column count under the specified parent index.
   * @param parent
   * @return
   */
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Returns the row count under the specified parent index.
   * @param parent
   * @return
   */
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Returns a QVariant containing data for the given index and data role.
   * Returns an empty QVariant if there is nothing to return.
   * @param index
   * @param role
   * @return
   */
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  /**
   * @brief Returns the flags for the specified index.
   * Flags are set based on the current Mode and whether the target item is a directory or not.
   * Normal Mode returns a default implementation of flags(const QModelIndex&).
   * Directory Mode disables items and selection of items that are not directories.
   * @param index
   * @return
   */
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  /**
   * @brief Returns a QModelIndex targetting a child of the specified parent
   * @param row
   * @param column
   * @param parent
   * @return
   */
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Returns a QModelIndex targetting the parent of the specified index
   * @param index
   * @return
   */
  QModelIndex parent(const QModelIndex& index) const override;

private:
  /**
   * @brief Returns a const Node* for the HTFileInfoTree::Node at the specified index.
   * Returns nullptr if none are found.
   * @param index
   * @return
   */
  const HTFileInfoTree::Node* getNode(const QModelIndex& index) const;

  /**
   * @brief Returns a vector of all top-level HTFileInfo within the file tree.
   * @return
   */
  std::vector<HTFileInfo> getTopLevelFiles() const;

  // -----------------------------------------------------------------------------
  // Variables
  HTFileInfoTree m_FileTree;
  Mode m_Mode = Mode::Normal;
};
