/*
 * Your License or Copyright can go here
 */

#include "HyperThoughtUtilitiesFilter.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"



#include "HyperThoughtUtilities/HyperThoughtUtilitiesConstants.h"
#include "HyperThoughtUtilities/HyperThoughtUtilitiesVersion.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HyperThoughtUtilitiesFilter::HyperThoughtUtilitiesFilter() 
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HyperThoughtUtilitiesFilter::~HyperThoughtUtilitiesFilter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HyperThoughtUtilitiesFilter::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HyperThoughtUtilitiesFilter::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HyperThoughtUtilitiesFilter::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HyperThoughtUtilitiesFilter::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HyperThoughtUtilitiesFilter::execute()
{
  initialize();
  dataCheck();
  if(getErrorCode() < 0) { return; }

  if (getCancel()) { return; }

  if (getWarningCode() < 0)
  {
    QString ss = QObject::tr("Some warning message");
    setWarningCondition(-88888888, ss);
  }

  if (getErrorCode() < 0)
  {
    QString ss = QObject::tr("Some error message");
    setErrorCondition(-99999999, ss);
    return;
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer HyperThoughtUtilitiesFilter::newFilterInstance(bool copyFilterParameters) const
{
  HyperThoughtUtilitiesFilter::Pointer filter = HyperThoughtUtilitiesFilter::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesFilter::getCompiledLibraryName() const
{ 
  return HyperThoughtUtilitiesConstants::HyperThoughtUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesFilter::getBrandingString() const
{
  return "HyperThoughtUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesFilter::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  HyperThoughtUtilities::Version::Major() << "." << HyperThoughtUtilities::Version::Minor() << "." << HyperThoughtUtilities::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesFilter::getGroupName() const
{ 
  return SIMPL::FilterGroups::Unsupported; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesFilter::getSubGroupName() const
{ 
  return "HyperThoughtUtilities"; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesFilter::getHumanLabel() const
{ 
  return "HyperThoughtUtilitiesFilter"; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid HyperThoughtUtilitiesFilter::getUuid() const
{
  return QUuid("{e26e6efb-54c3-56bd-987f-d292412552d0}");
}

// -----------------------------------------------------------------------------
HyperThoughtUtilitiesFilter::Pointer HyperThoughtUtilitiesFilter::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<HyperThoughtUtilitiesFilter> HyperThoughtUtilitiesFilter::New()
{
  struct make_shared_enabler : public HyperThoughtUtilitiesFilter
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesFilter::getNameOfClass() const
{
  return QString("HyperThoughtUtilitiesFilter");
}

// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesFilter::ClassName()
{
  return QString("HyperThoughtUtilitiesFilter");
}




