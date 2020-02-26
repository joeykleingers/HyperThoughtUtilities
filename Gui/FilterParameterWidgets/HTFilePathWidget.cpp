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

#include "HTFilePathWidget.h"

#include <QtWidgets/QInputDialog>

#include "SIMPLib/Filtering/AbstractFilter.h"

#include "HyperThoughtUtilities/FilterParameters/HTFilePathFilterParameter.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTUtils.h"

#include "ui_HTFilePathWidget.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePathWidget::HTFilePathWidget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent)
: FilterParameterWidget(parameter, filter, parent)
, m_Ui(new Ui::HTFilePathWidget)
{
  HTFilePathFilterParameter* connectionFP = dynamic_cast<HTFilePathFilterParameter*>(parameter);
  if(nullptr == connectionFP)
  {
    QString msg = "HTFileSelectionWidget requires an HTFileSelectionFilterParameter to operate.";
    Q_ASSERT_X(nullptr != parameter, msg.toLatin1().constData(), __FILE__);
  }

  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePathWidget::~HTFilePathWidget() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::setupGui()
{
  m_Ui->setupUi(this);

  m_Ui->propNameLabel->setText(getFilterParameter()->getHumanLabel());
  connect(getFilter(), SIGNAL(preflightExecuted()), this, SLOT(afterPreflight()));

  m_FileModel = new HTFileInfoModel(this);
  m_Ui->fileInfoTreeView->setModel(m_FileModel);

  // Update Widget from FP
  updateFromFP();

  // Update FP when changing the FP widget
  connect(m_Ui->sourceCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &HTFilePathWidget::onSourceChanged);
  connect(m_Ui->sourceIdLE, &QLineEdit::textChanged, this, &HTFilePathWidget::updateFP);
  connect(m_Ui->pathLE, &QLineEdit::textChanged, this, &HTFilePathWidget::updateFP);
  connect(m_Ui->syncBtn, &QPushButton::clicked, this, &HTFilePathWidget::syncFileModel);

  connect(m_Ui->fileInfoTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &HTFilePathWidget::onSelectionChanged);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::updateModel()
{
  m_FileModel->setFileInfoTree(getRequestedInfoTree());
  updateItemSelection();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::updateSourceVisibility(HTFilePath::ScopeType sourceType)
{
  bool showSourceId = (sourceType != HTFilePath::ScopeType::User);
  m_Ui->sourceIdLabel->setVisible(showSourceId);
  m_Ui->sourceIdLE->setVisible(showSourceId);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::onSourceChanged(int index)
{
  // Update Source ID visibility
  updateSourceVisibility(static_cast<HTFilePath::ScopeType>(index));

  updateModel();
  updateFP();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath HTFilePathWidget::getParameterValue() const
{
  auto parameter = dynamic_cast<HTFilePathFilterParameter*>(getFilterParameter());
  auto getter = parameter->getGetterCallback();
  return getter();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::setParameterValue(const HTFilePath& value)
{
  auto parameter = dynamic_cast<HTFilePathFilterParameter*>(getFilterParameter());
  auto setter = parameter->getSetterCallback();
  setter(value);
  emit parametersChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnection* HTFilePathWidget::getHyperThoughtConnection() const
{
  return HTConnection::GetExistingConnection(getFilter());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::widgetChanged(const QString& msg)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::beforePreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::afterPreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::filterNeedsInputParameters(AbstractFilter* filter)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::syncFileModel()
{
  // Asynchronously request file information from HyperThought
  m_InfoRequest = new HTFileInfoRequest(getHyperThoughtConnection(), getSourcePath(), true);
  connect(m_InfoRequest, &HTFileInfoRequest::infoReceived, this, &HTFilePathWidget::onFileInfoReceived);

  m_InfoRequest->exec();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::onFileInfoReceived(const HTFileInfoTree& fileInfoTree)
{
  m_FileModel->setFileInfoTree(fileInfoTree);
  updateItemSelection();
  emit parametersChanged();

  delete m_InfoRequest;
  m_InfoRequest = nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::updateItemSelection()
{
  QModelIndex index = m_FileModel->findIndex(getFilePath());
  auto selectionFlags = QItemSelectionModel::Clear | QItemSelectionModel::SelectCurrent;
  m_Ui->fileInfoTreeView->selectionModel()->select(index, selectionFlags);
  if(index.isValid())
  {
    viewFileInfo(index);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::updateFromFP()
{
  HTFilePath filePath = getParameterValue();
  m_Ui->sourceCombo->setCurrentIndex(static_cast<int>(filePath.getScopeType()));
  m_Ui->sourceIdLE->setText(filePath.getSourceId());
  m_Ui->pathLE->setText(filePath.getPath());

  // Update source ID visiblity
  updateSourceVisibility(filePath.getScopeType());
  updateItemSelection();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::updateFP()
{
  setParameterValue(getFilePath());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath HTFilePathWidget::getSourcePath() const
{
  int sourceTypei = m_Ui->sourceCombo->currentIndex();
  HTFilePath path;
  path.setScopeType(static_cast<HTFilePath::ScopeType>(sourceTypei));
  path.setSourceId(m_Ui->sourceIdLE->text());
  return path;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath HTFilePathWidget::getFilePath() const
{
  HTFilePath path = getSourcePath();
  path.setPath(m_Ui->pathLE->text());
  return path;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::viewFileInfo(const QModelIndex& index)
{
  HTFileInfo info = m_FileModel->getFileInfo(index);
  m_Ui->pathLE->setText(info.getPath() + info.getId() + ",");
  m_Ui->fileInfoWidget->setFileInfo(info);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTFilePathWidget::onSelectionChanged(const QItemSelection& current, const QItemSelection& previous)
{
  QModelIndexList indexList = current.indexes();
  if(indexList.size() > 0)
  {
    viewFileInfo(indexList[0]);
  }
  else
  {
    viewFileInfo(QModelIndex());
  }

  emit parametersChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFileInfoTree HTFilePathWidget::getRequestedInfoTree() const
{
  HTConnection* htConnection = getHyperThoughtConnection();
  if(nullptr == htConnection)
  {
    return {};
  }

  HTFilePath filePath = getFilePath();
  return htConnection->getFileCache().getFileInfoTree(filePath);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFileInfo HTFilePathWidget::getSelectedInfo() const
{
  auto selectionModel = m_Ui->fileInfoTreeView->selectionModel();
  return m_FileModel->getFileInfo(selectionModel->currentIndex());
}
