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

#include "HTConnectionWidget.h"

#include <QtWidgets/QInputDialog>

#include "SIMPLib/Filtering/AbstractFilter.h"

#include "HyperThoughtUtilities/FilterParameters/HTConnectionFilterParameter.h"
#include "HyperThoughtUtilities/Gui/Widgets/HTConnectionDialog.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTUtils.h"

#include "ui_HTConnectionWidget.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnectionWidget::HTConnectionWidget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent)
: FilterParameterWidget(parameter, filter, parent)
, m_Ui(new Ui::HTConnectionWidget)
{
  HTConnectionFilterParameter* connectionFP = dynamic_cast<HTConnectionFilterParameter*>(parameter);
  if(nullptr == connectionFP)
  {
    QString msg = "HTConnectionWidget requires an HTConnectionFilterParameter to operate.";
    Q_ASSERT_X(nullptr != parameter, msg.toLatin1().constData(), __FILE__);
  }

  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnectionWidget::~HTConnectionWidget() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionWidget::setupGui()
{
  m_Ui->setupUi(this);
  m_Ui->propNameLabel->setText(getFilterParameter()->getHumanLabel());
  m_Ui->apiAccessLE->setText(getConnectionFP()->getGetterCallback()());

  connect(m_Ui->apiAccessLE, &QLineEdit::textChanged, this, &HTConnectionWidget::updateApiAccess);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnectionFilterParameter* HTConnectionWidget::getConnectionFP() const
{
  return dynamic_cast<HTConnectionFilterParameter*>(getFilterParameter());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionWidget::updateApiAccess()
{
  auto setter = getConnectionFP()->getSetterCallback();
  QString apiAccess = m_Ui->apiAccessLE->text();
  setter(apiAccess);
  emit parametersChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionWidget::widgetChanged(const QString& msg)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionWidget::beforePreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionWidget::afterPreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionWidget::filterNeedsInputParameters(AbstractFilter* filter)
{
}
