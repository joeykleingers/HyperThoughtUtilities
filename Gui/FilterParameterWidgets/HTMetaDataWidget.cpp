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

#include "HTMetaDataWidget.h"

#include <QtWidgets/QInputDialog>

#include "SIMPLib/Filtering/AbstractFilter.h"

#include "HyperThoughtUtilities/FilterParameters/HTMetaDataFilterParameter.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTUtils.h"

#include "ui_HTMetaDataWidget.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaDataWidget::HTMetaDataWidget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent)
: FilterParameterWidget(parameter, filter, parent)
, m_Ui(new Ui::HTMetaDataWidget)
{
  HTMetaDataFilterParameter* metaDataFP = dynamic_cast<HTMetaDataFilterParameter*>(parameter);
  if(nullptr == metaDataFP)
  {
    QString msg = "HTMetaDataWidget requires an HTMetaDataFilterParameter to operate.";
    Q_ASSERT_X(nullptr != parameter, msg.toLatin1().constData(), __FILE__);
  }

  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaDataWidget::~HTMetaDataWidget() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataWidget::setupGui()
{
  m_Ui->setupUi(this);

  m_Ui->propNameLabel->setText(getFilterParameter()->getHumanLabel());
  connect(getFilter(), SIGNAL(preflightExecuted()), this, SLOT(afterPreflight()));
  connect(m_Ui->addBtn, &QPushButton::clicked, this, &HTMetaDataWidget::addValue);
  connect(m_Ui->removeBtn, &QPushButton::clicked, this, &HTMetaDataWidget::removeValue);
  connect(m_Ui->metaDataTable, &QTableWidget::itemChanged, this, &HTMetaDataWidget::handleMetaDataChanged);

  updateTable();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataWidget::handleMetaDataChanged()
{
  auto setter = getMetaDataFP()->getSetterCallback();
  setter(getMetaData());
  emit parametersChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData HTMetaDataWidget::getMetaData() const
{
  HTMetaData metaData;
  int numRows = m_Ui->metaDataTable->rowCount();
  for(int row = 0; row < numRows; row++)
  {
    QString key;
    QString value;

    QTableWidgetItem* keyItem = m_Ui->metaDataTable->item(row, 0);
    QTableWidgetItem* valItem = m_Ui->metaDataTable->item(row, 1);
    if(nullptr != keyItem)
    {
      key = keyItem->text();
    }
    if(nullptr != valItem)
    {
      value = valItem->text();
    }
    metaData.setValue(key, value);
  }
  return metaData;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataWidget::addValue()
{
  int row = m_Ui->metaDataTable->rowCount();
  m_Ui->metaDataTable->insertRow(row);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataWidget::removeValue()
{
  auto selectionModel = m_Ui->metaDataTable->selectionModel();
  QModelIndex current = selectionModel->currentIndex();
  if(!current.isValid())
  {
    return;
  }

  m_Ui->metaDataTable->removeRow(current.row());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaDataFilterParameter* HTMetaDataWidget::getMetaDataFP() const
{
  return dynamic_cast<HTMetaDataFilterParameter*>(getFilterParameter());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataWidget::widgetChanged(const QString& msg)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataWidget::beforePreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataWidget::afterPreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataWidget::filterNeedsInputParameters(AbstractFilter* filter)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataWidget::setMetaData(const HTMetaData& data)
{
  // Update table data
  {
    QSignalBlocker blocker(this);
    m_Ui->metaDataTable->clear();
    int row = 0;
    for(auto iter = data.begin(); iter != data.end(); iter++)
    {
      m_Ui->metaDataTable->insertRow(row);

      QTableWidgetItem* keyItem = new QTableWidgetItem(iter.key());
      QTableWidgetItem* valItem = new QTableWidgetItem(iter.value());
      m_Ui->metaDataTable->setItem(row, 0, keyItem);
      m_Ui->metaDataTable->setItem(row, 1, valItem);
      row++;
    }
  }

  // Set filter parameter and trigger preflight
  handleMetaDataChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTMetaDataWidget::updateTable()
{
  auto metaDataCallback = getMetaDataFP()->getGetterCallback();
  HTMetaData metaData = metaDataCallback();
  setMetaData(metaData);
}

