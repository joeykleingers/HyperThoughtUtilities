/*
 * Your License or Copyright can go here
 */

#include "HyperThoughtUtilitiesPlugin.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>

#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/IFilterFactory.hpp"
#include "SIMPLib/Filtering/FilterFactory.hpp"

#include "HyperThoughtUtilities/HyperThoughtUtilitiesConstants.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HyperThoughtUtilitiesPlugin::HyperThoughtUtilitiesPlugin() :
m_Version("0.1.0"),                            // Initialize HyperThoughtUtilities's Version Number Here
m_CompatibilityVersion("0.1.0"), // Initialize HyperThoughtUtilities's Compatibility Version Number Here
m_Vendor("Vendor Name"),                                // Initialize HyperThoughtUtilities's Vendor Name Here
m_URL("URL"),                                           // Initialize Company URL Here
m_Location("Location"),                                 // Initialize HyperThoughtUtilities library Location Here
m_Description("Description"),                           // Initialize HyperThoughtUtilities's Description Here
m_Copyright("Copyright"),                               // Initialize HyperThoughtUtilities's Copyright Here
m_Filters(QList<QString>()),                        // Initialize HyperThoughtUtilities's List of Dependencies Here
m_DidLoad(false)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HyperThoughtUtilitiesPlugin::~HyperThoughtUtilitiesPlugin() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getPluginFileName()
{
  return HyperThoughtUtilitiesConstants::HyperThoughtUtilitiesPluginFile;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getPluginDisplayName()
{
  return HyperThoughtUtilitiesConstants::HyperThoughtUtilitiesPluginDisplayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getPluginBaseName()
{
  return HyperThoughtUtilitiesConstants::HyperThoughtUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getVersion()
{
  return m_Version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getCompatibilityVersion()
{
  return m_CompatibilityVersion;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getVendor()
{
  return m_Vendor;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getURL()
{
  return m_URL;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getLocation()
{
  return m_Location;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getDescription()
{
  /* PLEASE UPDATE YOUR PLUGIN'S DESCRIPTION FILE.
  It is located at HyperThoughtUtilities/Resources/HyperThoughtUtilities/HyperThoughtUtilitiesDescription.txt */

  QFile licenseFile(":/HyperThoughtUtilities/HyperThoughtUtilitiesDescription.txt");
  QFileInfo licenseFileInfo(licenseFile);
  QString text = "<<--Description was not read-->>";

  if ( licenseFileInfo.exists() )
  {
    if ( licenseFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
      QTextStream in(&licenseFile);
      text = in.readAll();
    }
  }
  return text;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getCopyright()
{
  return m_Copyright;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString HyperThoughtUtilitiesPlugin::getLicense()
{
  /* PLEASE UPDATE YOUR PLUGIN'S LICENSE FILE.
  It is located at HyperThoughtUtilities/Resources/HyperThoughtUtilities/HyperThoughtUtilitiesLicense.txt */

  QFile licenseFile(":/HyperThoughtUtilities/HyperThoughtUtilitiesLicense.txt");
  QFileInfo licenseFileInfo(licenseFile);
  QString text = "<<--License was not read-->>";

  if ( licenseFileInfo.exists() )
  {
    if ( licenseFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
      QTextStream in(&licenseFile);
      text = in.readAll();
    }
  }
  return text;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QMap<QString, QString> HyperThoughtUtilitiesPlugin::getThirdPartyLicenses()
{
  QMap<QString, QString> licenseMap;
  QList<QString> fileStrList;

  for(const auto& item : fileStrList)
  {
    QFile file(item);
    QFileInfo licenseFileInfo(file);

    if ( licenseFileInfo.exists() )
    {
      if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
      {
        QTextStream in(&file);
        licenseMap.insert(licenseFileInfo.baseName(), in.readAll());
      }
    }
  }

  return licenseMap;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool HyperThoughtUtilitiesPlugin::getDidLoad()
{
  return m_DidLoad;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HyperThoughtUtilitiesPlugin::setDidLoad(bool didLoad)
{
  m_DidLoad = didLoad;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HyperThoughtUtilitiesPlugin::setLocation(QString filePath)
{
  m_Location = filePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HyperThoughtUtilitiesPlugin::writeSettings(QSettings& prefs)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HyperThoughtUtilitiesPlugin::readSettings(QSettings& prefs)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void HyperThoughtUtilitiesPlugin::registerFilterWidgets(FilterWidgetManager* fwm)
{
}

#include "HyperThoughtUtilitiesFilters/RegisterKnownFilters.cpp"


