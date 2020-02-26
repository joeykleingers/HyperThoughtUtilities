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

#include "TagHyperThoughtData.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"

#include "HyperThoughtUtilities/FilterParameters/HTFilePathFilterParameter.h"
#include "HyperThoughtUtilities/FilterParameters/HTMetaDataFilterParameter.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesConstants.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TagHyperThoughtData::TagHyperThoughtData()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TagHyperThoughtData::~TagHyperThoughtData() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TagHyperThoughtData::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TagHyperThoughtData::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_HTFILE_PATH_FP("HyperThought Path", HyperThoughtPath, FilterParameter::Parameter, TagHyperThoughtData));
  parameters.push_back(SIMPL_NEW_HTMETADATA_FP("Meta Data", MetaData, FilterParameter::Parameter, TagHyperThoughtData));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TagHyperThoughtData::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  HTConnection* connection = HTConnection::GetExistingConnection(this);
  if(nullptr == connection)
  {
    QString ss = "HyperThought connection required";
    setErrorCondition(-677, ss);
    return;
  }
  if(connection->getStatus() == HTConnection::Status::Disconnected || connection->getStatus() == HTConnection::Status::Invalid)
  {
    QString ss = "HyperThought connection is invalid or disconnected";
    setErrorCondition(-678, ss);
  }

  if(!connection->getFileCache().hasFileInfo(m_FilePath))
  {
    QString ss = "Cannot find cached file info for the current path. Please sync with HyperThought for updated information regarding the current file scope";
    setErrorCondition(-679, ss);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TagHyperThoughtData::preflight()
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
void TagHyperThoughtData::execute()
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

  updateFileMetaData();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TagHyperThoughtData::updateFileMetaData()
{
  HTConnection* connection = HTConnection::GetExistingConnection(this);
  m_UpdateRequest = new HTUpdateMetaDataRequest(connection, getFileInfo().getId(), m_MetaData);
  connect(m_UpdateRequest, &HTUpdateMetaDataRequest::requestFailed, this, &TagHyperThoughtData::onRequestFailed);
  connect(m_UpdateRequest, &HTUpdateMetaDataRequest::finished, this, &TagHyperThoughtData::onRequestCompleted);
  m_UpdateRequest->exec();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TagHyperThoughtData::onRequestFailed(QNetworkReply::NetworkError error)
{
  if(nullptr != m_UpdateRequest)
  {
    m_UpdateRequest->deleteLater();
    m_UpdateRequest = nullptr;

    QString ss = tr("Update request failed");
    setErrorCondition(-687, ss);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TagHyperThoughtData::onRequestCompleted()
{
  if(nullptr != m_UpdateRequest)
  {
    m_UpdateRequest->deleteLater();
    m_UpdateRequest = nullptr;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath TagHyperThoughtData::getHyperThoughtPath() const
{
  return m_FilePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TagHyperThoughtData::setHyperThoughtPath(const HTFilePath& filePath)
{
  m_FilePath = filePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFileInfo TagHyperThoughtData::getFileInfo() const
{
  HTConnection* connection = HTConnection::GetExistingConnection(this);
  return connection->getFileCache().getFileInfo(m_FilePath);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTMetaData TagHyperThoughtData::getMetaData() const
{
  return m_MetaData;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TagHyperThoughtData::setMetaData(const HTMetaData& value)
{
  m_MetaData = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer TagHyperThoughtData::newFilterInstance(bool copyFilterParameters) const
{
  TagHyperThoughtData::Pointer filter = TagHyperThoughtData::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TagHyperThoughtData::getCompiledLibraryName() const
{
  return HyperThoughtUtilitiesConstants::HyperThoughtUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TagHyperThoughtData::getBrandingString() const
{
  return "HyperThoughtUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TagHyperThoughtData::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << HyperThoughtUtilities::Version::Major() << "." << HyperThoughtUtilities::Version::Minor() << "." << HyperThoughtUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TagHyperThoughtData::getGroupName() const
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TagHyperThoughtData::getSubGroupName() const
{
  return "HyperThought Utilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TagHyperThoughtData::getHumanLabel() const
{
  return "Tag HyperThought Data";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid TagHyperThoughtData::getUuid() const
{
  return QUuid("{e82cf120-540a-5f4c-b9a1-083396fd334a}");
}

// -----------------------------------------------------------------------------
TagHyperThoughtData::Pointer TagHyperThoughtData::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<TagHyperThoughtData> TagHyperThoughtData::New()
{
  struct make_shared_enabler : public TagHyperThoughtData
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString TagHyperThoughtData::getNameOfClass() const
{
  return QString("TagHyperThoughtData");
}

// -----------------------------------------------------------------------------
QString TagHyperThoughtData::ClassName()
{
  return QString("TagHyperThoughtData");
}
