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

#include <QtCore/QSharedPointer>

#include "SVWidgetsLib/FilterParameterWidgets/FilterParameterWidget.h"

#include "HyperThoughtUtilities/HyperThoughtConnection/HTMetaData.h"

namespace Ui
{
class HTMetaDataWidget;
}

class HTMetaDataFilterParameter;

/**
 * @class HTMetaDataWidget HTMetaDataWidget.h
 * HyperthoughtUtilities/Gui/FilterParameterWidgets/HTMetaDataWidget.h
 * @brief The HTMetaDataWidget filter parameter is designed to allow the user
 * to create HTConnections without saving access credentials to the
 * FilterPipeline's json.
 */
class HTMetaDataWidget : public FilterParameterWidget
{
  Q_OBJECT

public:
  /**
   * @brief Constructor
   * @param parameter The FilterParameter object that this widget represents
   * @param filter The instance of the filter that this parameter is a part of
   * @param parent The parent QWidget for this Widget
   */
  HTMetaDataWidget(FilterParameter* parameter, AbstractFilter* filter = nullptr, QWidget* parent = nullptr);

  virtual ~HTMetaDataWidget();

  /**
   * @brief Initializes some of the GUI elements with selections or other GUI related numItems
   */
  virtual void setupGui();

  /**
   * @brief Creates a meta data object from the table values.
   * @return
   */
  HTMetaData getMetaData() const;

  /**
   * @brief Adds an empty meta data value.
   */
  void addValue();

  /**
   * @brief Removes the current meta data value.
   */
  void removeValue();

public slots:
  void widgetChanged(const QString& msg);
  void beforePreflight();
  void afterPreflight();
  void filterNeedsInputParameters(AbstractFilter* filter);
  void setMetaData(const HTMetaData& data);
  void updateTable();

private:
  HTMetaDataFilterParameter* getMetaDataFP() const;
  void handleMetaDataChanged();

  // -----------------------------------------------------------------------------
  // Variables
  QSharedPointer<Ui::HTMetaDataWidget> m_Ui;
};
