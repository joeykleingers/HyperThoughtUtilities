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

#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfo.h"

namespace Ui
{
class HTFileWidget;
}

class HTConnection;

/**
 * @class HTFileWidget HTFileWidget.h
 * HyperthoughtUtilities/Gui/FilterParameterWidgets/HTFileWidget.h
 * @brief The HTFileWidget filter parameter is designed to allow the user
 * to create HTConnections without saving access credentials to the
 * FilterPipeline's json.
 */
class HTFileWidget : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Constructor
   * @param parent The parent QWidget for this Widget
   */
  HTFileWidget(QWidget* parent = nullptr);

  virtual ~HTFileWidget();

  /**
   * @brief Returns the current HyperThought FileInfo.
   * @return
   */
  HTFileInfo getFileInfo() const;

  /**
   * @brief Sets the current HyperThought FileInfo and updates the GUI.
   * @param info
   */
  void setFileInfo(const HTFileInfo& info);

private:
  /**
   * @brief Initializes some of the GUI elements with selections or other GUI related numItems
   */
  void setupGui();

  void fillLayoutWithMap(QGridLayout* layout, const QMap<QString, QString>& values);

  // -----------------------------------------------------------------------------
  // Variables
  QSharedPointer<Ui::HTFileWidget> m_Ui;
  HTFileInfo m_FileInfo;
};
