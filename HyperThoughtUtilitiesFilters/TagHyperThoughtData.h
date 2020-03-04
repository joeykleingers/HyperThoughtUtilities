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

#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfo.h"
#include "HyperThoughtUtilities/HyperThoughtConnection/HTFilePath.h"
#include "HyperThoughtUtilities/HyperThoughtRequests/HTUpdateMetaDataRequest.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

/**
 * @brief The TagHyperThoughtData class. See [Filter documentation](@ref taghyperthoughtdata) for details.
 */
class HyperThoughtUtilities_EXPORT TagHyperThoughtData : public AbstractFilter
{
  Q_OBJECT

  Q_PROPERTY(HTFilePath HyperThoughtPath READ getHyperThoughtPath WRITE setHyperThoughtPath)
  Q_PROPERTY(HTMetaData MetaData READ getMetaData WRITE setMetaData)

#ifdef SIMPL_ENABLE_PYTHON
  // clang-format off
  PYB11_CREATE_BINDINGS(TagHyperThoughtData SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(TagHyperThoughtData)
  PYB11_FILTER_NEW_MACRO(TagHyperThoughtData)
  PYB11_FILTER_PARAMETER(HTFilePath, HyperThoughtPath)
  PYB11_FILTER_PARAMETER(HTMetaData, MetaData)
  PYB11_PROPERTY(HTFilePath HyperThoughtPath READ getHyperThoughtPath WRITE setHyperThoughtPath)
  PYB11_PROPERTY(HTMetaData MetaData READ getMetaData WRITE setMetaData)
  // clang-format on
#endif

public:
  using Self = TagHyperThoughtData;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<TagHyperThoughtData> New();

  /**
   * @brief Returns the name of the class for TagHyperThoughtData
   */
  QString getNameOfClass() const override;

  /**
   * @brief Returns the name of the class for TagHyperThoughtData
   */
  static QString ClassName();

  ~TagHyperThoughtData() override;

  /**
   * @brief Returns the HyperThought path for the target file.
   * @return
   */
  HTFilePath getHyperThoughtPath() const;

  /**
   * @brief Sets the HyperThought path for the target file.
   * @param filePath
   */
  void setHyperThoughtPath(const HTFilePath& filePath);

  /**
   * @brief Gets the cached HTFileInfo for the current path.
   * @return
   */
  HTFileInfo getFileInfo() const;

  /**
   * @brief Returns the HTMetaData value to upload to HyperThought.
   * @return
   */
  HTMetaData getMetaData() const;

  /**
   * @brief Sets the HTMetaData to upload to HyperThought.
   */
  void setMetaData(const HTMetaData& value);

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
  TagHyperThoughtData();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Sends the patch request to HyperThought for updating a file's metadata.
   * Calls onRequestCompleted() on success.
   * Calls onRequestFailed(QNetworkReply::NetworkError) if an error occurs.
   */
  void updateFileMetaData();

  /**
   * @brief Called when the HyperThought request encounters an error.
   * @param error
   */
  void onRequestFailed(QNetworkReply::NetworkError error);

  /**
   * @brief Called when the HyperThought request has been completed.
   */
  void onRequestCompleted();

private:
  HTFilePath m_FilePath;
  HTMetaData m_MetaData;
  HTUpdateMetaDataRequest* m_UpdateRequest = nullptr;

public:
  TagHyperThoughtData(const TagHyperThoughtData&) = delete;            // Copy Constructor Not Implemented
  TagHyperThoughtData& operator=(const TagHyperThoughtData&) = delete; // Copy Assignment Not Implemented
  TagHyperThoughtData(TagHyperThoughtData&&) = delete;                 // Move Constructor Not Implemented
  TagHyperThoughtData& operator=(TagHyperThoughtData&&) = delete;      // Move Assignment Not Implemented
};
