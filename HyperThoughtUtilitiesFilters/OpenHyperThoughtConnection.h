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

#include <memory>
#include <vector>

#include <QNetworkReply>

#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "HyperThoughtUtilities/HyperThoughtConnection/HTFileInfo.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

class HTConnection;

/**
 * @brief The OpenHyperThoughtConnection class. See [Filter documentation](@ref openhyperthoughtconnection) for details.
 */
class HyperThoughtUtilities_EXPORT OpenHyperThoughtConnection : public AbstractFilter
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  // clang-format off
  PYB11_CREATE_BINDINGS(OpenHyperThoughtConnection SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(OpenHyperThoughtConnection)
  PYB11_FILTER_NEW_MACRO(OpenHyperThoughtConnection)
  PYB11_FILTER_PARAMETER(QString, ApiAccess)
  PYB11_PROPERTY(QString ApiAccess READ getApiAccess WRITE setApiAccess)
  // clang-format on
#endif

public:
  Q_PROPERTY(QString ApiAccess READ getApiAccess WRITE setApiAccess)

  using Self = OpenHyperThoughtConnection;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<OpenHyperThoughtConnection> New();

  /**
   * @brief Returns the name of the class for OpenHyperThoughtConnection
   */
  QString getNameOfClass() const override;

  /**
   * @brief Returns the name of the class for OpenHyperThoughtConnection
   */
  static QString ClassName();

  ~OpenHyperThoughtConnection() override;

  QString getApiAccess() const;

  void setApiAccess(const QString& code);

  /**
   * @brief Returns the HTConnection pointer.
   * @return
   */
  HTConnection* getHTConnection() const;

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
  OpenHyperThoughtConnection();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Creates a new HTConnection based on existing values.
   */
  void recreateHTConnection();

  /**
   * @brief Handle failure to connect to HyperThought.
   * @param err
   */
  void connectionFailed(QNetworkReply::NetworkError err);

private:
  QString m_ApiAccess;
  HTConnection* m_Connection = nullptr;

public:
  OpenHyperThoughtConnection(const OpenHyperThoughtConnection&) = delete;            // Copy Constructor Not Implemented
  OpenHyperThoughtConnection& operator=(const OpenHyperThoughtConnection&) = delete; // Copy Assignment Not Implemented
  OpenHyperThoughtConnection(OpenHyperThoughtConnection&&) = delete;                 // Move Constructor Not Implemented
  OpenHyperThoughtConnection& operator=(OpenHyperThoughtConnection&&) = delete;      // Move Assignment Not Implemented
};
