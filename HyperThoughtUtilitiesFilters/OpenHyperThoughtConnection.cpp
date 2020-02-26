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

#include "OpenHyperThoughtConnection.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"

#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "HyperThoughtUtilities/HyperThoughtUtilitiesConstants.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesVersion.h"

#include "HyperThoughtUtilities/FilterParameters/HTConnectionFilterParameter.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
OpenHyperThoughtConnection::OpenHyperThoughtConnection()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
OpenHyperThoughtConnection::~OpenHyperThoughtConnection()
{
  if(m_Connection)
  {
    m_Connection->deleteLater();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OpenHyperThoughtConnection::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OpenHyperThoughtConnection::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_HTCONNECTION_FP("API Access", ApiAccess, FilterParameter::Parameter, OpenHyperThoughtConnection));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OpenHyperThoughtConnection::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  // Create a new HTConnection using the current API Access code
  recreateHTConnection();

  // Test HTConnection
  if(getHTConnection())
  {
    if(getHTConnection()->isValid())
    {
      // m_Connection->refreshAccessToken();
    }
    else
    {
      QString ss = QObject::tr("Could not form HyperThought connection");
      setErrorCondition(-666, ss);
    }
  }
  else
  {
    QString ss = QObject::tr("No HyperThought connection available");
    setErrorCondition(-667, ss);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OpenHyperThoughtConnection::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true);              // Set the fact that we are preflighting.
  emit preflightAboutToExecute();    // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck();                       // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted();          // We are done preflighting this filter
  setInPreflight(false);             // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OpenHyperThoughtConnection::execute()
{
  initialize();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  if(getCancel())
  {
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OpenHyperThoughtConnection::connectionFailed(QNetworkReply::NetworkError err)
{
  QString ss = QObject::tr("HyperThought connection failed. Code: ").arg(err);
  setErrorCondition(-667, ss);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString OpenHyperThoughtConnection::getApiAccess() const
{
  return m_ApiAccess;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OpenHyperThoughtConnection::setApiAccess(const QString& code)
{
  m_ApiAccess = code;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTConnection* OpenHyperThoughtConnection::getHTConnection() const
{
  return m_Connection;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OpenHyperThoughtConnection::recreateHTConnection()
{
  HTConnection* oldConnection = m_Connection;
  m_Connection = new HTConnection(getApiAccess());

  // Delete old collection
  if(nullptr != oldConnection)
  {
    m_Connection->takeFileCache(*oldConnection);
    oldConnection->deleteLater();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer OpenHyperThoughtConnection::newFilterInstance(bool copyFilterParameters) const
{
  OpenHyperThoughtConnection::Pointer filter = OpenHyperThoughtConnection::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString OpenHyperThoughtConnection::getCompiledLibraryName() const
{
  return HyperThoughtUtilitiesConstants::HyperThoughtUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString OpenHyperThoughtConnection::getBrandingString() const
{
  return "HyperThought Utilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString OpenHyperThoughtConnection::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << HyperThoughtUtilities::Version::Major() << "." << HyperThoughtUtilities::Version::Minor() << "." << HyperThoughtUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString OpenHyperThoughtConnection::getGroupName() const
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString OpenHyperThoughtConnection::getSubGroupName() const
{
  return "HyperThought Utilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString OpenHyperThoughtConnection::getHumanLabel() const
{
  return "Open HyperThought Connection";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid OpenHyperThoughtConnection::getUuid() const
{
  return QUuid("{04c5efaa-e66a-5ede-a168-14b6038080c9}");
}

// -----------------------------------------------------------------------------
OpenHyperThoughtConnection::Pointer OpenHyperThoughtConnection::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<OpenHyperThoughtConnection> OpenHyperThoughtConnection::New()
{
  struct make_shared_enabler : public OpenHyperThoughtConnection
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString OpenHyperThoughtConnection::getNameOfClass() const
{
  return QString("OpenHyperThoughtConnection");
}

// -----------------------------------------------------------------------------
QString OpenHyperThoughtConnection::ClassName()
{
  return QString("OpenHyperThoughtConnection");
}
