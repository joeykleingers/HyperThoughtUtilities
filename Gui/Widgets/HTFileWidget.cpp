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

#include "HTFileWidget.h"

#include <QtWidgets/QInputDialog>
#include <QtWidgets/QLabel>

#include "SIMPLib/Filtering/AbstractFilter.h"

#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTUtils.h"

#include "ui_HTFileWidget.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFileWidget::HTFileWidget(QWidget* parent)
: QWidget(parent)
, m_Ui(new Ui::HTFileWidget)
{
  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFileWidget::~HTFileWidget() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFileWidget::setupGui()
{
  m_Ui->setupUi(this);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFileInfo HTFileWidget::getFileInfo() const
{
  return m_FileInfo;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFileWidget::setFileInfo(const HTFileInfo& info)
{
  m_FileInfo = info;

  // Content
  m_Ui->fileIdView->setText(info.getContent().fileId);
  m_Ui->nameView->setText(info.getContent().name);
  m_Ui->fileTypeView->setText(info.getContent().fileType);
  m_Ui->sizeView->setText(QString::number(info.getContent().size));
  m_Ui->createdDateView->setText(info.getContent().createdDate);
  m_Ui->createdByView->setText(info.getContent().createdBy);
  m_Ui->modifiedDateView->setText(info.getContent().modifiedDate);
  m_Ui->modifiedByView->setText(info.getContent().modifiedBy);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void clearLayout(QLayout* layout)
{
  while(QLayoutItem* item = layout->takeAt(0))
  {
    if(item->layout())
    {
      clearLayout(item->layout());
      delete item->layout();
    }
    if(item->widget())
    {
      delete item->widget();
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFileWidget::fillLayoutWithMap(QGridLayout* layout, const QMap<QString, QString>& values)
{
  clearLayout(layout);

  int row = 0;
  for(auto iter = values.begin(); iter != values.end(); iter++)
  {
    QString keyStr = iter.key() + " : ";
    QString valueStr = iter.value();

    QLabel* keyLabel = new QLabel(keyStr);
    keyLabel->setToolTip(keyStr);

    QLabel* valLabel = new QLabel(valueStr);
    valLabel->setToolTip(valueStr);

    layout->addWidget(keyLabel, row, 0);
    layout->addWidget(valLabel, row, 1);
    keyLabel->show();
    valLabel->show();
    row++;
  }
}
