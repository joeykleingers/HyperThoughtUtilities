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

#include "HTFileInfoTree.h"

// -----------------------------------------------------------------------------
HTFileInfoTree::HTFileInfoTree()
{
}

// -----------------------------------------------------------------------------
HTFileInfoTree::HTFileInfoTree(const HTFileInfoTree& other)
: m_Root(other.m_Root)
{
}

// -----------------------------------------------------------------------------
HTFileInfoTree::HTFileInfoTree(HTFileInfoTree&& other)
: m_Root(std::move(other.m_Root))
{
}

// -----------------------------------------------------------------------------
HTFileInfoTree::~HTFileInfoTree() = default;

// -----------------------------------------------------------------------------
void HTFileInfoTree::clear()
{
  // Delete root children
  size_t size = m_Root.size();
  for(size_t i = 0; i < size; i++)
  {
    delete m_Root[i];
  }
  m_Root.children.clear();
}

// -----------------------------------------------------------------------------
void HTFileInfoTree::insert(const std::vector<HTFileInfo>& items)
{
  for(const auto& item : items)
  {
    insert(item);
  }
}

// -----------------------------------------------------------------------------
void HTFileInfoTree::insert(const HTFileInfo& info)
{
  QStringList pathFragments = info.getPathFragments();
  Node* parentNode = findNode(pathFragments);
  if(nullptr == parentNode)
  {
    parentNode = &m_Root;
  }

  Node* newNode = new Node();
  newNode->fileInfo = info;
  newNode->parent = parentNode;
  parentNode->children.push_back(newNode);
}

// -----------------------------------------------------------------------------
bool HTFileInfoTree::contains(const HTFilePath& path) const
{
  return nullptr != findNode(path);
}

// -----------------------------------------------------------------------------
HTFileInfo HTFileInfoTree::find(const HTFilePath& path) const
{
  const Node* node = findNode(path);
  if(nullptr == node)
  {
    return HTFileInfo();
  }
  return node->fileInfo;
}

// -----------------------------------------------------------------------------
HTFileInfoTree::Node* HTFileInfoTree::findNode(const HTFileInfo& info)
{
  QStringList pathFragments = info.getPathFragments();
  return findNode(pathFragments);
}

// -----------------------------------------------------------------------------
const HTFileInfoTree::Node* HTFileInfoTree::findNode(const HTFileInfo& info) const
{
  QStringList pathFragments = info.getPathFragments();
  return findNode(pathFragments);
}

// -----------------------------------------------------------------------------
HTFileInfoTree::Node* HTFileInfoTree::findNode(const HTFilePath& path)
{
  return findNode(&m_Root, path.getPathFragments(), 0);
}

// -----------------------------------------------------------------------------
const HTFileInfoTree::Node* HTFileInfoTree::findNode(const HTFilePath& path) const
{
  return findNode(&m_Root, path.getPathFragments(), 0);
}

// -----------------------------------------------------------------------------
HTFileInfoTree::Node* HTFileInfoTree::findNode(const QStringList& pathFragments)
{
  return findNode(&m_Root, pathFragments, 0);
}

// -----------------------------------------------------------------------------
const HTFileInfoTree::Node* HTFileInfoTree::findNode(const QStringList& pathFragments) const
{
  return findNode(&m_Root, pathFragments, 0);
}

// -----------------------------------------------------------------------------
HTFileInfoTree::Node* HTFileInfoTree::findNode(Node* node, const QStringList& pathFragments, int index) const
{  
  const int size = pathFragments.size();
  if(size == index)
  {
    // Path has reached its end
    return node;
  }

  // Search through children for the current path fragment
  const QString fragment = pathFragments[index];
  for(const auto& child : node->children)
  {
    if(fragment == child->fileInfo.getId())
    {
      return findNode(child, pathFragments, index + 1);
    }
  }
  return nullptr;
}

// -----------------------------------------------------------------------------
const HTFileInfoTree::Node* HTFileInfoTree::findNode(const Node* node, const QStringList& pathFragments, int index) const
{
  const int size = pathFragments.size();
  if(size == index)
  {
    // Reached the end of the path
    return node;
  }

  // Search through children for the current path fragment
  const QString fragment = pathFragments[index];
  for(const auto& child : node->children)
  {
    if(fragment == child->fileInfo.getId())
    {
      return findNode(child, pathFragments, index + 1);
    }
  }
  return nullptr;
}

// -----------------------------------------------------------------------------
size_t HTFileInfoTree::getIndexWithinParent(const Node* node) const
{
  if(nullptr == node)
  {
    return -1;
  }
  if(nullptr == node->parent)
  {
    return -1;
  }
  const Node* parent = node->parent;
  for(size_t i = 0; i < parent->size(); i++)
  {
    if((*parent)[i]->fileInfo == node->fileInfo)
    {
      return i;
    }
  }
  return -1;
}

// -----------------------------------------------------------------------------
const HTFileInfoTree::Node& HTFileInfoTree::getRoot() const
{
  return m_Root;
}

// -----------------------------------------------------------------------------
void HTFileInfoTree::sort()
{
  m_Root.sortChildren();
}

// -----------------------------------------------------------------------------
HTFileInfoTree& HTFileInfoTree::operator=(const HTFileInfoTree& other)
{
  QByteArray bArray;
  QDataStream oStream(&bArray, QIODevice::WriteOnly);
  oStream << other;

  QDataStream iStream(&bArray, QIODevice::ReadOnly);
  iStream >> *this;
  return *this;
}

// -----------------------------------------------------------------------------
HTFileInfoTree& HTFileInfoTree::operator=(HTFileInfoTree&& other)
{
  m_Root = std::move(other.m_Root);
  return *this;
}

// -----------------------------------------------------------------------------
HTFileInfoTree::Node::Node() = default;

// -----------------------------------------------------------------------------
HTFileInfoTree::Node::Node(const Node& other)
: fileInfo(other.fileInfo)
{
  children.reserve(other.size());
  for(const Node* child : other.children)
  {
    Node* newChild = new Node(*child);
    newChild->parent = this;
    children.push_back(newChild);
  }
}

// -----------------------------------------------------------------------------
HTFileInfoTree::Node::Node(Node&& other)
: fileInfo(std::move(other.fileInfo))
, children(std::move(other.children))
, parent(std::move(other.parent))
{
}

// -----------------------------------------------------------------------------
HTFileInfoTree::Node::~Node()
{
  for(auto child : children)
  {
    delete child;
  }
}

// -----------------------------------------------------------------------------
size_t HTFileInfoTree::Node::size() const
{
  return children.size();
}

// -----------------------------------------------------------------------------
const HTFileInfoTree::Node* HTFileInfoTree::Node::operator[](int index) const
{
  return children[index];
}

// -----------------------------------------------------------------------------
HTFileInfoTree::Node& HTFileInfoTree::Node::operator=(const Node& other)
{
  fileInfo = other.fileInfo;
  children.reserve(other.size());
  for(const Node* child : other.children)
  {
    Node* newChild = new Node(*child);
    newChild->parent = this;
    children.push_back(newChild);
  }
  return *this;
}

// -----------------------------------------------------------------------------
HTFileInfoTree::Node& HTFileInfoTree::Node::operator=(Node&& other)
{
  fileInfo = std::move(other.fileInfo);
  children = std::move(other.children);
  parent = std::move(other.parent);
  return *this;
}

// Sort nodes using a custom function object
struct NodeCompareLess {
  bool operator()(const HTFileInfoTree::Node* a, const HTFileInfoTree::Node* b) const
  {
    // IsDir inequality
    if(a->fileInfo.isDir() && !b->fileInfo.isDir())
    {
      return true;
    }
    if(!a->fileInfo.isDir() && b->fileInfo.isDir())
    {
      return false;
    }

    return a->fileInfo.getFileName() < b->fileInfo.getFileName();
  }
};

// -----------------------------------------------------------------------------
void HTFileInfoTree::Node::sortChildren()
{
  // Sort children's children
  for(auto child : children)
  {
    child->sortChildren();
  }

  NodeCompareLess comp;
  std::sort(children.begin(), children.end(), comp);
}

// -----------------------------------------------------------------------------
QDataStream& operator<<(QDataStream& out, const HTFileInfoTree::Node* node)
{
  out << node->fileInfo;
  uint64_t numChildren = node->size();
  out << numChildren;

  for(auto child : node->children)
  {
    out << child;
  }
  return out;
}

// -----------------------------------------------------------------------------
QDataStream& operator<<(QDataStream& out, const HTFileInfoTree& myObj)
{
  const HTFileInfoTree::Node* root = &myObj.getRoot();
  out << root;
  return out;
}

// -----------------------------------------------------------------------------
QDataStream& operator>>(QDataStream& in, HTFileInfoTree::Node* node)
{
  in >> node->fileInfo;
  uint64_t numChildren;
  in >> numChildren;

  node->children.resize(numChildren);
  for(size_t i = 0; i < numChildren; i++)
  {
    HTFileInfoTree::Node* childNode = new HTFileInfoTree::Node();
    node->children[i] = childNode;
    childNode->parent = node;
    in >> childNode;
  }
  return in;
}

// -----------------------------------------------------------------------------
QDataStream& operator>>(QDataStream& in, HTFileInfoTree& myObj)
{
  const HTFileInfoTree::Node* constRoot = &myObj.getRoot();
  HTFileInfoTree::Node* root = const_cast<HTFileInfoTree::Node*>(constRoot);
  in >> root;
  return in;
}
