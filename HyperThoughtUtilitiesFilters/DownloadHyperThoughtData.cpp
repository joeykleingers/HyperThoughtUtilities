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

#include "DownloadHyperThoughtData.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/OutputPathFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "HyperThoughtUtilities/FilterParameters/HTFilePathFilterParameter.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTConnection.h"
#include "HyperThoughtUtilities/HyperThoughtRequests/HTDownloadRequest.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesFilters/util/HTUtils.h"

#include "HyperThoughtUtilities/HyperThoughtUtilitiesConstants.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DownloadHyperThoughtData::DownloadHyperThoughtData()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DownloadHyperThoughtData::~DownloadHyperThoughtData() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DownloadHyperThoughtData::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DownloadHyperThoughtData::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_HTFILE_PATH_FP("HyperThought Path", FilePath, FilterParameter::Parameter, DownloadHyperThoughtData));
  parameters.push_back(SIMPL_NEW_OUTPUT_PATH_FP("Download Directory", DownloadDir, FilterParameter::Parameter, DownloadHyperThoughtData));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DownloadHyperThoughtData::dataCheck()
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

  if(m_FilePath.getPath().isEmpty())
  {
    QString ss = "A HyperThought file path is required";
    setErrorCondition(-679, ss);
    return;
  }
  if(!connection->getFileCache().hasFileInfo(m_FilePath))
  {
    QString ss = "The local HyperThought cache does not contain a value for the specified path";
    setErrorCondition(-680, ss);
    return;
  }
  if(connection->getFileCache().getFileInfo(m_FilePath).isDir())
  {
    QString ss = "The selected HyperThought file cannot be a directory";
    setErrorCondition(-681, ss);
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DownloadHyperThoughtData::preflight()
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
void DownloadHyperThoughtData::execute()
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

  downloadFiles();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DownloadHyperThoughtData::downloadFiles()
{
  // Download files from HyperThought to target directory
  HTConnection* connection = HTConnection::GetExistingConnection(this);
  m_DownloadRequest = new HTDownloadRequest(connection, m_FilePath);
  m_DownloadRequest->setDownloadDir(m_DownloadDir);
  auto fileInfo = connection->getFileCache().getFileInfo(m_FilePath);
  m_DownloadRequest->setDownloadName(fileInfo.getFileName());

  connect(m_DownloadRequest, &HTDownloadRequest::downloadProgress, this, &DownloadHyperThoughtData::onDownloadProgress, Qt::UniqueConnection);
  connect(m_DownloadRequest, &HTDownloadRequest::downloadComplete, this, &DownloadHyperThoughtData::onDownloadComplete, Qt::UniqueConnection);
  m_DownloadRequest->exec();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HTFilePath DownloadHyperThoughtData::getFilePath() const
{
  return m_FilePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DownloadHyperThoughtData::setFilePath(const HTFilePath& path)
{
  m_FilePath = path;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString DownloadHyperThoughtData::getDownloadDir() const
{
  return m_DownloadDir;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DownloadHyperThoughtData::setDownloadDir(const QString& downloadDir)
{
  m_DownloadDir = downloadDir;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer DownloadHyperThoughtData::newFilterInstance(bool copyFilterParameters) const
{
  DownloadHyperThoughtData::Pointer filter = DownloadHyperThoughtData::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString DownloadHyperThoughtData::getCompiledLibraryName() const
{
  return HyperThoughtUtilitiesConstants::HyperThoughtUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString DownloadHyperThoughtData::getBrandingString() const
{
  return "HyperThoughtUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString DownloadHyperThoughtData::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << HyperThoughtUtilities::Version::Major() << "." << HyperThoughtUtilities::Version::Minor() << "." << HyperThoughtUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString DownloadHyperThoughtData::getGroupName() const
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString DownloadHyperThoughtData::getSubGroupName() const
{
  return "HyperThought Utilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString DownloadHyperThoughtData::getHumanLabel() const
{
  return "Download HyperThought Data";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid DownloadHyperThoughtData::getUuid() const
{
  return QUuid("{f0700883-3dda-5318-b976-d9f0000580db}");
}

// -----------------------------------------------------------------------------
DownloadHyperThoughtData::Pointer DownloadHyperThoughtData::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<DownloadHyperThoughtData> DownloadHyperThoughtData::New()
{
  struct make_shared_enabler : public DownloadHyperThoughtData
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString DownloadHyperThoughtData::getNameOfClass() const
{
  return QString("DownloadHyperThoughtData");
}

// -----------------------------------------------------------------------------
QString DownloadHyperThoughtData::ClassName()
{
  return QString("DownloadHyperThoughtData");
}

// -----------------------------------------------------------------------------
void DownloadHyperThoughtData::onDownloadProgress(int64_t bytesRead, int64_t bytesTotal)
{
  double percentDownloaded = bytesRead / static_cast<double>(bytesTotal);
  notifyStatusMessage("Download: " + QString::number(percentDownloaded));
}

// -----------------------------------------------------------------------------
void DownloadHyperThoughtData::onDownloadComplete()
{
  m_DownloadRequest->deleteLater();
  m_DownloadRequest = nullptr;
}
