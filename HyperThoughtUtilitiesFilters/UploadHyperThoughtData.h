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

#pragma once

#include <QtNetwork/QNetworkReply>

#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFilePath.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTMetaData.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

class HTDataUploadRequest;
class IDataArray;
using IDataArrayShPtrType = std::shared_ptr<IDataArray>;
using IDataArrayWkPtrType = std::weak_ptr<IDataArray>;

/**
 * @brief The UploadHyperThoughtData class. See [Filter documentation](@ref uploadhyperthoughtdata) for details.
 */
class HyperThoughtUtilities_EXPORT UploadHyperThoughtData : public AbstractFilter
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  // clang-format off
  PYB11_CREATE_BINDINGS(UploadHyperThoughtData SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(UploadHyperThoughtData)
  PYB11_FILTER_NEW_MACRO(UploadHyperThoughtData)
  PYB11_FILTER_PARAMETER(DataArrayPath UploadDataArrayPath)
  PYB11_FILTER_PARAMETER(QString UploadFileName)
  PYB11_FILTER_PARAMETER(HTFilePath, UploadFilePath)
  PYB11_FILTER_PARAMETER(bool, UpdateMetaData)
  PYB11_FILTER_PARAMETER(HTMetaData, MetaData)
  PYB11_PROPERTY(DataArrayPath UploadDataArrayPath READ getUploadDataArrayPath WRITE setUploadDataArrayPath)
  PYB11_PROPERTY(QString UploadFileName READ getUploadFileName WRITE setUploadFileName)
  PYB11_PROPERTY(HTFilePath UploadFilePath READ getUploadFilePath WRITE setUploadFilePath)
  PYB11_PROPERTY(bool UpdateMetaData READ getUpdatesMetaData WRITE setUpdatesMetaData)
  PYB11_PROPERTY(HTMetaData MetaData READ getMetaData WRITE setMetaData)

  // clang-format on
#endif

  Q_PROPERTY(DataArrayPath UploadDataArrayPath READ getUploadDataArrayPath WRITE setUploadDataArrayPath)
  Q_PROPERTY(QString UploadFileName READ getUploadFileName WRITE setUploadFileName)
  Q_PROPERTY(HTFilePath UploadFilePath READ getUploadFilePath WRITE setUploadFilePath)
  Q_PROPERTY(bool UpdateMetaData READ getUpdatesMetaData WRITE setUpdatesMetaData)
  Q_PROPERTY(HTMetaData MetaData READ getMetaData WRITE setMetaData)

public:
  using Self = UploadHyperThoughtData;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<UploadHyperThoughtData> New();

  /**
   * @brief Returns the name of the class for UploadHyperThoughtData
   */
  QString getNameOfClass() const override;

  /**
   * @brief Returns the name of the class for UploadHyperThoughtData
   */
  static QString ClassName();

  ~UploadHyperThoughtData() override;

  /**
   * @brief Returns the upload data array path.
   * @return
   */
  DataArrayPath getUploadDataArrayPath() const;

  /**
   * @brief Sets the upload data array path.
   * @param path
   */
  void setUploadDataArrayPath(const DataArrayPath& path);

  /**
   * @brief Returns the upload file name.
   * @return
   */
  QString getUploadFileName() const;

  /**
   * @brief Sets the upload file name.
   * @param fileName
   */
  void setUploadFileName(const QString& fileName);

  /**
   * @brief Returns the HyperThought file path for uploading the file.
   * @return
   */
  HTFilePath getUploadFilePath() const;

  /**
   * @brief Sets the HyperThought upload file path.
   * @param filePath
   */
  void setUploadFilePath(const HTFilePath& filepPath);

  /**
   * @brief Returns whether or not the filter updates the uploaded data's metadata.
   * @return
   */
  bool getUpdatesMetaData() const;

  /**
   * @brief Sets whether or not the filter updates the uploaded data's metadata.
   * @param updates
   */
  void setUpdatesMetaData(bool updates);

  /**
   * @brief Returns the current metadata for uploading the data.
   * @return
   */
  HTMetaData getMetaData() const;

  /**
   * @brief Sets the metadata for uploading the data.
   * @param data
   */
  void setMetaData(const HTMetaData& data);

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  QString getFilterVersion() const override;

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  QUuid getUuid() const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
   * @brief preflight Reimplemented from @see AbstractFilter class
   */
  void preflight() override;

signals:
  /**
   * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
   * be pushed from a user-facing control (such as a widget)
   * @param filter Filter instance pointer
   */
  void updateFilterParameters(AbstractFilter* filter);

  /**
   * @brief parametersChanged Emitted when any Filter parameter is changed internally
   */
  void parametersChanged();

  /**
   * @brief preflightAboutToExecute Emitted just before calling dataCheck()
   */
  void preflightAboutToExecute();

  /**
   * @brief preflightExecuted Emitted just after calling dataCheck()
   */
  void preflightExecuted();

protected:
  UploadHyperThoughtData();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Begin uploading the chosen data array to HyperThought.
   * This method waits for the request to be completed before continuing.
   */
  void uploadData(const QByteArray& data);

  /**
   * @brief Called when the data has been uploaded to HyperThought.
   */
  void onUploadComplete();

  /**
   * @brief Called when there was an error uploading the data to HyperThought.
   * @param err
   */
  void onUploadError(QNetworkReply::NetworkError err);

private:
  DataArrayPath m_UploadDataArrayPath;
  QString m_UploadFileName;
  HTFilePath m_UploadFilePath;
  bool m_UpdateMetaData = true;
  HTMetaData m_MetaData;
  HTDataUploadRequest* m_UploadRequest = nullptr;
  IDataArrayWkPtrType m_UploadDataArrayWeakPtr;

  /**
   * @brief Converts a DataArray to a QByteArray
   * @param dataArrayPtr
   * @return
   */
  QByteArray convertDataArrayToByteArray(const IDataArrayShPtrType& dataArrayPtr);

public:
  UploadHyperThoughtData(const UploadHyperThoughtData&) = delete;            // Copy Constructor Not Implemented
  UploadHyperThoughtData& operator=(const UploadHyperThoughtData&) = delete; // Copy Assignment Not Implemented
  UploadHyperThoughtData(UploadHyperThoughtData&&) = delete;                 // Move Constructor Not Implemented
  UploadHyperThoughtData& operator=(UploadHyperThoughtData&&) = delete;      // Move Assignment Not Implemented
};
