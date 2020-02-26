/* ============================================================================
 *Copyright(c) 2020 - 2020 BlueQuartz Software, LLC
 *
 * Redistributionand use in sourceand binary forms, with or without modification,
 * are permitted provided that the following conditions are met :
 *
 *Redistributions of source code must retain the above copyright notice, this
 * list of conditionsand the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditionsand the following disclaimer in the documentationand /or
 *other materials provided with the distribution.
 *
 *Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 *THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the followig contracts :
 *United States Air Force Prime Contract FA8650 - 15 - D - 5231
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "HTConnectionDialog.h"

#include <QtWidgets/QPushButton>
#include <QtWidgets/QStyle>

#include "HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTConnection.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTUtils.h"

#include "ui_HTConnectionDialog.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnectionDialog::HTConnectionDialog(QWidget* parent)
: QDialog(parent)
, m_Ui(new Ui::HTConnectionDialog())
{
  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnectionDialog::~HTConnectionDialog() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionDialog::setupGui()
{
  m_Ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose, true);
  setWindowFlag(Qt::WindowContextHelpButtonHint, false);

  checkBaseValues();
  checkCallbackUrl();

  connect(m_Ui->authorityLE, &QLineEdit::textChanged, this, &HTConnectionDialog::checkBaseValues);
  connect(m_Ui->authorityIpLE, &QLineEdit::textChanged, this, &HTConnectionDialog::checkBaseValues);
  connect(m_Ui->clientIdLE, &QLineEdit::textChanged, this, &HTConnectionDialog::checkBaseValues);
  connect(m_Ui->callbackLE, &QLineEdit::textChanged, this, &HTConnectionDialog::handleCallbackEdited);
  connect(m_Ui->apiAccessLE, &QLineEdit::textChanged, this, &HTConnectionDialog::handleApiAccessEdited);

  connect(this, &QDialog::accepted, this, &HTConnectionDialog::createConnection);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionDialog::checkBaseValues()
{
  bool hasAuthority = !m_Ui->authorityLE->text().isEmpty();
  bool hasAuthorityIp = !m_Ui->authorityIpLE->text().isEmpty();
  bool hasClientId = !m_Ui->clientIdLE->text().isEmpty();

  bool hasValues = hasAuthority && hasAuthorityIp && hasClientId;
  m_Ui->connectionTabWidget->setTabEnabled(1, hasValues);
  checkCallbackUrl();
  updateDialogStyling();
  if(hasValues)
  {
    updateGoToUrl();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionDialog::updateGoToUrl()
{
  QString ipUrl = m_Ui->authorityIpLE->text();
  QString clientId = m_Ui->clientIdLE->text();
  QString encodedUrl = HTUtils::generateAuthUrl(ipUrl, clientId);
  m_Ui->urlLabel->setText(encodedUrl);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionDialog::handleCallbackEdited()
{
  checkCallbackUrl();
  QString callbackStr = m_Ui->callbackLE->text();
  auto mappedParams = HTUtils::parseQueryParams(callbackStr);
  QString codeStr = mappedParams["code"];
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionDialog::checkCallbackUrl()
{
  bool hasCallbackUrl = !m_Ui->callbackLE->text().isEmpty();
  QPushButton* applyBtn = m_Ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok);
  applyBtn->setEnabled(hasCallbackUrl);
  updateDialogStyling();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionDialog::createConnection()
{
  QString authUrlStr = m_Ui->authorityLE->text();
  QString authIpUrlStr = m_Ui->authorityIpLE->text();
  QString clientIdStr = m_Ui->clientIdLE->text();
  QString callbackUrlStr = m_Ui->callbackLE->text();

  QString callbackStr = m_Ui->callbackLE->text();
  auto mappedParams = HTUtils::parseQueryParams(callbackStr);
  QString codeStr = mappedParams["code"];

#if 0
  HTConnection* connection = new HTConnection(authUrlStr, authIpUrlStr, clientIdStr);
  connection->setCallbackUrl(callbackUrlStr);
  emit connectionCreated(connection);
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionDialog::updateDialogStyling()
{
  QPushButton* applyBtn = m_Ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok);
  QStyle* style = applyBtn->style();
  style->unpolish(applyBtn);
  style->polish(applyBtn);
  
  repaint();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HTConnectionDialog::handleApiAccessEdited()
{
  std::string encodedAccess = m_Ui->apiAccessLE->text().toStdString();
  m_Ui->decodedApiView->setText(HTUtils::decode64(encodedAccess));
}
