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
#include <QtCore/QMetaType>
#include <QtCore/QString>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfo.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTFilePath.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

/**
 * @class HTFileInfoTree HTFileInfoTree.h HyperThoughtUtilities/HyperThoughtConnection/HTFileInfoTree.h
 * @brief The HTFileInfoTree stores HTFileInfo nodes in a tree structure based on file paths.
 */
class HyperThoughtUtilities_EXPORT HTFileInfoTree
{
public:
  struct Node
  {
    HTFileInfo fileInfo;
    std::vector<Node*> children;
    Node* parent = nullptr;

    Node();
    Node(const Node& other);
    Node(Node&& other);
    ~Node();

    /**
     * @brief Returns the number of children the node has.
     * @return
     */
    size_t size() const;

    /**
     * @brief Returns the child at the given index.
     */
    const Node* operator[](int index) const;

    /**
     * @brief Recursively sorts the children by name and isDirectory status.
     */
    void sortChildren();

    /**
     * @brief Copy assignment
     * @param other
     * @return
     */
    Node& operator=(const Node& other);

    /**
     * @brief Mode assignment
     * @param other
     * @return
     */
    Node& operator=(Node&& other);
  };

  HTFileInfoTree();
  HTFileInfoTree(const HTFileInfoTree& other);
  HTFileInfoTree(HTFileInfoTree&& other);
  virtual ~HTFileInfoTree();

  /**
   * @brief Clears the tree and deletes the nodes it contains.
   */
  void clear();

  /**
   * @brief Inserts a collection of HTFileInfo items into the tree.
   * @param items
   */
  void insert(const std::vector<HTFileInfo>& items);

  /**
   * @brief Inserts the HTFileInfo item into the tree.
   * @param info
   */
  void insert(const HTFileInfo& info);

  /**
   * @brief Checks if the tree contains an object at the specified path.
   * @param path
   * @return
   */
  bool contains(const HTFilePath& path) const;

  /**
   * @brief Finds the HTFileInfo for the given path.
   * Returns a default-constructed HTFileInfo if nothing is found.
   * @param path
   * @return
   */
  HTFileInfo find(const HTFilePath& path) const;

  /**
   * @brief Returns the node containing the given value.
   * @param info
   * @return
   */
  Node* findNode(const HTFileInfo& info);

  /**
   * @brief Returns the node containing the given value.
   * @param info
   * @return
   */
  const Node* findNode(const HTFileInfo& info) const;

  /**
   * @brief Returns the node for the given path.
   * Returns nullptr if nothing is found.
   * @param path
   * @return
   */
  Node* findNode(const HTFilePath& path);

  /**
   * @brief Returns the node for the given path.
   * Returns nullptr if nothing is found.
   * @param path
   * @return
   */
  const Node* findNode(const HTFilePath& path) const;

  /**
   * @brief Returns a const reference to the root node.
   * @return
   */
  const Node& getRoot() const;

  /**
   * @brief Returns the index for looking up the given node within its parent.
   * Returns -1 if the node or its parent are null.
   * Returns -1 if the node cannot be found within its parent.
   * @param node
   * @return
   */
  size_t getIndexWithinParent(const Node* node) const;

  /**
   * @brief Sort the tree nodes. Directories are listed before files.
   * Files and directories are sorted by filename.
   */
  void sort();

  /**
   * @brief Assignment operator
   * @param other
   * @return
   */
  HTFileInfoTree& operator=(const HTFileInfoTree& other);

  /**
   * @brief Move operator
   * @param other
   * @return
   */
  HTFileInfoTree& operator=(HTFileInfoTree&& other);

private:
  /**
   * @brief Recursively traverses the tree for the Node specified by the given path.
   * Returns nullptr if the Node could not be found.
   * @param pathFragments
   * @return
   */
  Node* findNode(const QStringList& pathFragments);

  /**
   * @brief Recursively traverses the tree for the Node specified by the given path.
   * Returns nullptr if the Node could not be found.
   * @param pathFragments
   * @return
   */
  const Node* findNode(const QStringList& pathFragments) const;

  /**
   * @brief Recursively traverses the tree for the Node specified by the given path.
   * Returns nullptr if the Node could not be found.
   * @param node
   * @param pathFragments
   * @param index
   * @return
   */
  Node* findNode(Node* node, const QStringList& pathFragments, int index) const;

  /**
   * @brief Recursively traverses the tree for the Node specified by the given path.
   * Returns nullptr if the Node could not be found.
   * @param node
   * @param pathFragments
   * @param index
   * @return
   */
  const Node* findNode(const Node* node, const QStringList& pathFragments, int index) const;

  // -----------------------------------------------------------------------------
  // Variables
  Node m_Root;
};

QDataStream& operator<<(QDataStream& out, const HTFileInfoTree& myObj);
QDataStream& operator>>(QDataStream& in, HTFileInfoTree& myObj);

Q_DECLARE_METATYPE(HTFileInfoTree)
