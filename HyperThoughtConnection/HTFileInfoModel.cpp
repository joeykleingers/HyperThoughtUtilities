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

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfoModel.h"

// -----------------------------------------------------------------------------
HTFileInfoModel::HTFileInfoModel(QObject* parent)
: QAbstractItemModel(parent)
{
}

// -----------------------------------------------------------------------------
HTFileInfoModel::~HTFileInfoModel() = default;

// -----------------------------------------------------------------------------
const HTFileInfoTree::Node* HTFileInfoModel::getNode(const QModelIndex& index) const
{
  if(!index.isValid())
  {
    return &m_FileTree.getRoot();
  }
  return static_cast<HTFileInfoTree::Node*>(index.internalPointer());
}

// -----------------------------------------------------------------------------
HTFileInfo HTFileInfoModel::getFileInfo(const QModelIndex& index) const
{
  const HTFileInfoTree::Node* node = getNode(index);
  if(nullptr == node)
  {
    return HTFileInfo();
  }
  return node->fileInfo;
}

// -----------------------------------------------------------------------------
bool HTFileInfoModel::isDir(const QModelIndex& index) const
{
  if(!index.isValid())
  {
    return true;
  }

  auto fileInfo = getFileInfo(index);
  return fileInfo.isDir();
}

// -----------------------------------------------------------------------------
std::vector<HTFileInfo> HTFileInfoModel::getTopLevelFiles() const
{
  const auto& root = m_FileTree.getRoot();
  size_t size = root.size();

  std::vector<HTFileInfo> topLevelFiles(size);
  for(size_t i = 0; i < size; i++)
  {
    topLevelFiles[i] = root[i]->fileInfo;
  }
  return topLevelFiles;
}

// -----------------------------------------------------------------------------
std::vector<HTFileInfo> HTFileInfoModel::getChildItems(const QModelIndex& parent) const
{
  if(!parent.isValid())
  {
    return getTopLevelFiles();
  }

  std::vector<HTFileInfo> childItems;
  auto childNodes = getNode(parent)->children;
  for(const auto& node : childNodes)
  {
    childItems.push_back(node->fileInfo);
  }
  return childItems;
}

// -----------------------------------------------------------------------------
std::vector<HTFileInfo> HTFileInfoModel::getChildItems(const HTFileInfo& parent) const
{
  const HTFileInfoTree::Node* parentNode = m_FileTree.findNode(parent);
  if(nullptr == parentNode)
  {
    return {};
  }
  std::vector<HTFileInfo> childItems;
  auto childNodes = parentNode->children;
  for(const auto& node : childNodes)
  {
    childItems.push_back(node->fileInfo);
  }
  return childItems;
}

// -----------------------------------------------------------------------------
QModelIndex HTFileInfoModel::findIndex(const HTFilePath& path) const
{
  const HTFileInfoTree::Node* constNode = m_FileTree.findNode(path);
  if(nullptr == constNode)
  {
    return QModelIndex();
  }
  int row = m_FileTree.getIndexWithinParent(constNode);
  auto node = const_cast<HTFileInfoTree::Node*>(constNode);
  return createIndex(row, 0, node);
}

// -----------------------------------------------------------------------------
void HTFileInfoModel::setFileInfoTree(const HTFileInfoTree& tree)
{
  emit beginResetModel();
  m_FileTree = tree;
  emit endResetModel();
}

// -----------------------------------------------------------------------------
HTFileInfoModel::Mode HTFileInfoModel::getMode() const
{
  return m_Mode;
}

// -----------------------------------------------------------------------------
void HTFileInfoModel::setMode(Mode mode)
{
  m_Mode = mode;
}

// -----------------------------------------------------------------------------
int HTFileInfoModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

// -----------------------------------------------------------------------------
int HTFileInfoModel::rowCount(const QModelIndex& parent) const
{
  const HTFileInfoTree::Node* parentNode = getNode(parent);
  return parentNode->size();
}

// -----------------------------------------------------------------------------
QVariant HTFileInfoModel::data(const QModelIndex& index, int role) const
{
  HTFileInfo info = getFileInfo(index);
  switch(role)
  {
  case Qt::DisplayRole:
    return info.getFileName();
  case Qt::ToolTipRole:
    return info.getFileName();
  case Qt::DecorationRole:
    // return getFileIcon(info);
    break;
  }

  return QVariant();
}

// -----------------------------------------------------------------------------
Qt::ItemFlags HTFileInfoModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);
  if(Mode::Directory == m_Mode)
  {
    HTFileInfo info = getFileInfo(index);
    flags.setFlag(Qt::ItemIsEnabled, info.isDir());
    flags.setFlag(Qt::ItemIsSelectable, info.isDir());
  }
  return flags;
}

// -----------------------------------------------------------------------------
QModelIndex HTFileInfoModel::index(int row, int column, const QModelIndex& parent) const
{
  const HTFileInfoTree::Node* parentNode = getNode(parent);
  if(parentNode->size() <= row)
  {
    return QModelIndex();
  }

  HTFileInfoTree::Node* node = parentNode->children[row];
  return createIndex(row, column, node);
}

// -----------------------------------------------------------------------------
QModelIndex HTFileInfoModel::parent(const QModelIndex& index) const
{
  if(!index.isValid())
  {
    return QModelIndex();
  }

  const HTFileInfoTree::Node* node = getNode(index);
  size_t parentIndex = m_FileTree.getIndexWithinParent(node);
  return createIndex(parentIndex, 0, node->parent);
}
