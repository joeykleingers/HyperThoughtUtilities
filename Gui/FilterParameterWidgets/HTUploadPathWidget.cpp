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

#include "HTUploadPathWidget.h"

#include <QtWidgets/QInputDialog>

#include "SIMPLib/Filtering/AbstractFilter.h"

#include "HyperThoughtUtilities/FilterParameters/HTUploadPathFilterParameter.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTUtils.h"

#include "ui_HTUploadPathWidget.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTUploadPathWidget::HTUploadPathWidget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent)
: FilterParameterWidget(parameter, filter, parent)
, m_Ui(new Ui::HTUploadPathWidget)
{
  HTUploadPathFilterParameter* connectionFP = dynamic_cast<HTUploadPathFilterParameter*>(parameter);
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
HTUploadPathWidget::~HTUploadPathWidget() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::setupGui()
{
  m_Ui->setupUi(this);

  m_Ui->propNameLabel->setText(getFilterParameter()->getHumanLabel());
  connect(getFilter(), SIGNAL(preflightExecuted()), this, SLOT(afterPreflight()));

  m_FileModel = new HTFileInfoModel(this);
  m_FileModel->setMode(HTFileInfoModel::Mode::Directory);
  m_Ui->fileInfoTreeView->setModel(m_FileModel);

  // Update Widget from FP
  updateFromFP();

  // Update FP when changing the FP widget
  connect(m_Ui->sourceCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &HTUploadPathWidget::onSourceChanged);
  connect(m_Ui->sourceIdLE, &QLineEdit::textChanged, this, &HTUploadPathWidget::updateFP);
  connect(m_Ui->usernameLE, &QLineEdit::textChanged, this, &HTUploadPathWidget::updateFP);
  connect(m_Ui->pathLE, &QLineEdit::textChanged, this, &HTUploadPathWidget::updateFP);
  connect(m_Ui->syncBtn, &QPushButton::clicked, this, &HTUploadPathWidget::syncFileModel);

  connect(m_Ui->fileInfoTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &HTUploadPathWidget::onSelectionChanged);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::updateModel()
{
  m_FileModel->setFileInfoTree(getRequestedInfoTree());
  updateItemSelection();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::updateSourceVisibility(HTFilePath::ScopeType scopeType)
{
  bool isUserScope = (scopeType == HTFilePath::ScopeType::User);
  m_Ui->sourceIdLabel->setVisible(!isUserScope);
  m_Ui->sourceIdLE->setVisible(!isUserScope);

  // Set username visibility
  m_Ui->usernameLabel->setVisible(isUserScope);
  m_Ui->usernameLE->setVisible(isUserScope);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::onSourceChanged(int index)
{
  // Update Source ID visibility
  updateSourceVisibility(static_cast<HTFilePath::ScopeType>(index));

  updateModel();
  updateFP();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath HTUploadPathWidget::getParameterValue() const
{
  auto parameter = dynamic_cast<HTUploadPathFilterParameter*>(getFilterParameter());
  auto getter = parameter->getGetterCallback();
  return getter();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::setParameterValue(const HTFilePath& value)
{
  auto parameter = dynamic_cast<HTUploadPathFilterParameter*>(getFilterParameter());
  auto setter = parameter->getSetterCallback();
  setter(value);
  emit parametersChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnection* HTUploadPathWidget::getHyperThoughtConnection() const
{
  return HTConnection::GetExistingConnection(getFilter());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::widgetChanged(const QString& msg)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::beforePreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::afterPreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::filterNeedsInputParameters(AbstractFilter* filter)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::syncFileModel()
{
  // Asynchronously request file information from HyperThought
  m_InfoRequest = new HTFileInfoRequest(getHyperThoughtConnection(), getSourcePath(), true);
  connect(m_InfoRequest, &HTFileInfoRequest::infoReceived, this, &HTUploadPathWidget::onFileInfoReceived);

  m_InfoRequest->exec();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::onFileInfoReceived(const HTFileInfoTree& fileInfoTree)
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
void HTUploadPathWidget::updateItemSelection()
{
  QModelIndex index = m_FileModel->findIndex(getFilePath());
  auto selectionFlags = QItemSelectionModel::Clear | QItemSelectionModel::SelectCurrent;
  m_Ui->fileInfoTreeView->selectionModel()->select(index, selectionFlags);
  if(index.isValid())
  {
    updatePath(index);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::updateFromFP()
{
  HTFilePath filePath = getParameterValue();
  HTFilePath::ScopeType scopeType = filePath.getScopeType();
  m_Ui->sourceCombo->setCurrentIndex(static_cast<int>(scopeType));
  m_Ui->usernameLE->setText(filePath.getUsername());
  m_Ui->sourceIdLE->setText(filePath.getSourceId());
  m_Ui->pathLE->setText(filePath.getPath());

  // Update source ID visiblity
  updateSourceVisibility(filePath.getScopeType());
  updateItemSelection();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::updateFP()
{
  setParameterValue(getFilePath());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath HTUploadPathWidget::getSourcePath() const
{
  int sourceTypei = m_Ui->sourceCombo->currentIndex();
  HTFilePath::ScopeType scopeType = static_cast<HTFilePath::ScopeType>(sourceTypei);
  QString sourceId = m_Ui->sourceIdLE->text();
  QString username = m_Ui->usernameLE->text();

  HTFilePath path;
  path.setScopeType(scopeType);
  path.setSourceId(sourceId);
  path.setUsername(username);
  return path;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath HTUploadPathWidget::getFilePath() const
{
  HTFilePath path = getSourcePath();
  path.setPath(m_Ui->pathLE->text());
  return path;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::updatePath(const QModelIndex& index)
{
  HTFileInfo info = m_FileModel->getFileInfo(index);
  m_Ui->pathLE->setText(info.getPath() + info.getId() + ",");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTUploadPathWidget::onSelectionChanged(const QItemSelection& current, const QItemSelection& previous)
{
  QModelIndexList indexList = current.indexes();
  if(indexList.size() > 0)
  {
    updatePath(indexList[0]);
  }
  else
  {
    updatePath(QModelIndex());
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFileInfoTree HTUploadPathWidget::getRequestedInfoTree() const
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
HTFileInfo HTUploadPathWidget::getSelectedInfo() const
{
  auto selectionModel = m_Ui->fileInfoTreeView->selectionModel();
  return m_FileModel->getFileInfo(selectionModel->currentIndex());
}
