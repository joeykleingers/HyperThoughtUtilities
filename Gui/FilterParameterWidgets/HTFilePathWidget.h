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

#include <QtCore/QItemSelection>
#include <QtCore/QSharedPointer>

#include "SVWidgetsLib/FilterParameterWidgets/FilterParameterWidget.h"

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfoModel.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfoTree.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTFilePath.h"
#include "HyperThoughtUtilities/HyperThoughtRequests/HTFileInfoRequest.h"

namespace Ui
{
class HTFilePathWidget;
}

class HTConnection;
class HTFilePathWidget;
class HTFilePathFilterParameter;

/**
 * @class HTFilePathWidget HTFilePathWidget.h
 * HyperthoughtUtilities/Gui/FilterParameterWidgets/HTFilePathWidget.h
 * @brief The HTFilePathWidget filter parameter is designed to allow the user
 * to create HTConnections without saving access credentials to the
 * FilterPipeline's json.
 */
class HTFilePathWidget : public FilterParameterWidget
{
  Q_OBJECT

public:
  /**
   * @brief Constructor
   * @param parameter The FilterParameter object that this widget represents
   * @param filter The instance of the filter that this parameter is a part of
   * @param parent The parent QWidget for this Widget
   */
  HTFilePathWidget(FilterParameter* parameter, AbstractFilter* filter = nullptr, QWidget* parent = nullptr);

  virtual ~HTFilePathWidget();

  /**
   * @brief Initializes some of the GUI elements with selections or other GUI related numItems
   */
  virtual void setupGui();

public slots:
  void widgetChanged(const QString& msg);
  void beforePreflight();
  void afterPreflight();
  void filterNeedsInputParameters(AbstractFilter* filter);

private:
  /**
   * @brief Returns the HTFilePath from the filter parameter.
   * @return
   */
  HTFilePath getParameterValue() const;

  /**
   * @brief Sets the HTFilePath for the filter parameter and emits the parametersChanged signal.
   * @param value
   */
  void setParameterValue(const HTFilePath& value);

  /**
   * @brief Gets the filter's HyperThought connection.
   * Returns nullptr if no HyperThought connection is available.
   * @return
   */
  HTConnection* getHyperThoughtConnection() const;

  /**
   * @brief Creates a HyperThought file path for the source type and ID.
   * @return
   */
  HTFilePath getSourcePath() const;

  /**
   * @brief Creates a HyperThought file path for the source type, ID, and path.
   */
  HTFilePath getFilePath() const;

  /**
   * @brief Updates the widget values from the filter parameter.
   */
  void updateFromFP();

  /**
   * @brief Updates the filter parameter from the widget values
   */
  void updateFP();

  /**
   * @brief Begins an asynchronous request for HyperThought file info.
   * Updates the file info model when complete.
   */
  void syncFileModel();

  /**
   * @brief Called when the file info request is completed.
   * Updates the HyperThought file info cache for the current source with the returned values.
   * @param fileInfoTree
   */
  void onFileInfoReceived(const HTFileInfoTree& fileInfoTree);

  /**
   * @brief Updates the file info model from the HyperThought file info cache.
   */
  void updateModel();

  /**
   * @brief Updates widget visibility based on the given source type.
   * @param sourceType
   */
  void updateSourceVisibility(HTFilePath::ScopeType sourceType);

  /**
   * @brief Called when the source type combo is changed.
   * Calls updateSourceVisibility for the corresponding ScopeType.
   * @param index
   */
  void onSourceChanged(int index);

  /**
   * @brief Updates the HyperThought file info widget for the item at the given index.
   * @param index
   */
  void viewFileInfo(const QModelIndex& index);

  /**
   * @brief Called when the current selection changes.
   * Calls viewFileInfo for the current index.
   * @param current
   * @param previous
   */
  void onSelectionChanged(const QItemSelection& current, const QItemSelection& previous);

  /**
   * @brief Returns the corresponding HyperThought file info tree for the current source.
   * @return
   */
  HTFileInfoTree getRequestedInfoTree() const;

  /**
   * @brief Returns the selected HyperThought file info.
   * @return
   */
  HTFileInfo getSelectedInfo() const;

  /**
   * @brief Updates the currently selected item based on the current HyperThought file path.
   * Clears the selection if the current path does not point to a cached file.
   */
  void updateItemSelection();

  // -----------------------------------------------------------------------------
  // Variables
  QSharedPointer<Ui::HTFilePathWidget> m_Ui;
  HTFileInfoModel* m_FileModel;
  HTFileInfoRequest* m_InfoRequest = nullptr;
};
