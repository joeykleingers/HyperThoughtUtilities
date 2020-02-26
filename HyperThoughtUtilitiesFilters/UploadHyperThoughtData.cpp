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

#include "UploadHyperThoughtData.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"

#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"

#include "HyperThoughtUtilities/FilterParameters/HTMetaDataFilterParameter.h"
#include "HyperThoughtUtilities/FilterParameters/HTUploadPathFilterParameter.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"
#include "HyperThoughtUtilities/HyperThoughtRequests/HTFileUploadRequest.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesConstants.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
UploadHyperThoughtData::UploadHyperThoughtData()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
UploadHyperThoughtData::~UploadHyperThoughtData() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Upload File", LocalFilePath, FilterParameter::Parameter, UploadHyperThoughtData));
  parameters.push_back(SIMPL_NEW_HTUPLOAD_PATH_FP("HyperThought Path", UploadFilePath, FilterParameter::Parameter, UploadHyperThoughtData));

  QStringList linkedProps = { "MetaData" };
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Set Metadata", UpdatesMetaData, FilterParameter::Parameter, UploadHyperThoughtData, linkedProps));
  parameters.push_back(SIMPL_NEW_HTMETADATA_FP("Meta Data", MetaData, FilterParameter::Parameter, UploadHyperThoughtData));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  HTConnection* connection = HTConnection::GetExistingConnection(this);
  if(nullptr == connection)
  {
    QString ss = "HyperThought connection required";
    setErrorCondition(-687, ss);
    return;
  }
  if(connection->getStatus() == HTConnection::Status::Disconnected || connection->getStatus() == HTConnection::Status::Invalid)
  {
    QString ss = "HyperThought connection is invalid or disconnected";
    setErrorCondition(-688, ss);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::preflight()
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
void UploadHyperThoughtData::execute()
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

  uploadFile();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::uploadFile()
{
  HTConnection* connection = HTConnection::GetExistingConnection(this);
  m_UploadRequest = new HTFileUploadRequest(connection, m_UploadFilePath, m_LocalFilePath);
  m_UploadRequest->setMetaData(m_UpdateMetaData, m_MetaData);
  connect(m_UploadRequest, &HTFileUploadRequest::uploadComplete, this, &UploadHyperThoughtData::onUploadComplete);
  connect(m_UploadRequest, &HTFileUploadRequest::requestFailed, this, &UploadHyperThoughtData::onUploadError);
  m_UploadRequest->exec();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::onUploadComplete()
{
  if(nullptr != m_UploadRequest)
  {
    m_UploadRequest->deleteLater();
    m_UploadRequest = nullptr;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::onUploadError(QNetworkReply::NetworkError err)
{
  if(nullptr != m_UploadRequest)
  {
    m_UploadRequest->deleteLater();
    m_UploadRequest = nullptr;
  }

  QString ss = tr("Encountered an error while attempting to upload file to HyperThought.");
  setErrorCondition(-674, ss);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString UploadHyperThoughtData::getLocalFilePath() const
{
  return m_LocalFilePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::setLocalFilePath(const QString& path)
{
  m_LocalFilePath = path;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath UploadHyperThoughtData::getUploadFilePath() const
{
  return m_UploadFilePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::setUploadFilePath(const HTFilePath& filePath)
{
  m_UploadFilePath = filePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool UploadHyperThoughtData::getUpdatesMetaData() const
{
  return m_UpdateMetaData;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::setUpdatesMetaData(bool updates)
{
  m_UpdateMetaData = updates;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData UploadHyperThoughtData::getMetaData() const
{
  return m_MetaData;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UploadHyperThoughtData::setMetaData(const HTMetaData& data)
{
  m_MetaData = data;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer UploadHyperThoughtData::newFilterInstance(bool copyFilterParameters) const
{
  UploadHyperThoughtData::Pointer filter = UploadHyperThoughtData::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString UploadHyperThoughtData::getCompiledLibraryName() const
{
  return HyperThoughtUtilitiesConstants::HyperThoughtUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString UploadHyperThoughtData::getBrandingString() const
{
  return "HyperThoughtUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString UploadHyperThoughtData::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << HyperThoughtUtilities::Version::Major() << "." << HyperThoughtUtilities::Version::Minor() << "." << HyperThoughtUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString UploadHyperThoughtData::getGroupName() const
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString UploadHyperThoughtData::getSubGroupName() const
{
  return "HyperThoughtUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString UploadHyperThoughtData::getHumanLabel() const
{
  return "UploadHyperThoughtData";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid UploadHyperThoughtData::getUuid() const
{
  return QUuid("{afff1ad4-a7a3-53a6-9a8e-02cccaaf6e48}");
}

// -----------------------------------------------------------------------------
UploadHyperThoughtData::Pointer UploadHyperThoughtData::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<UploadHyperThoughtData> UploadHyperThoughtData::New()
{
  struct make_shared_enabler : public UploadHyperThoughtData
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString UploadHyperThoughtData::getNameOfClass() const
{
  return QString("UploadHyperThoughtData");
}

// -----------------------------------------------------------------------------
QString UploadHyperThoughtData::ClassName()
{
  return QString("UploadHyperThoughtData");
}
