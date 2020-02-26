/*
 * Your License should go here
 */
#ifndef _hyperthoughtutilitiesconstants_h_
#define _hyperthoughtutilitiesconstants_h_

#include <QtCore/QString>

/**
* @brief This namespace is used to define some Constants for the plugin itself.
*/
namespace HyperThoughtUtilitiesConstants
{
  const QString HyperThoughtUtilitiesPluginFile("HyperThoughtUtilitiesPlugin");
  const QString HyperThoughtUtilitiesPluginDisplayName("HyperThoughtUtilities");
  const QString HyperThoughtUtilitiesBaseName("HyperThoughtUtilities");

  namespace FilterGroups
  {
  	const QString HyperThoughtUtilitiesFilters("HyperThoughtUtilities");
  }
}

/**
* @brief Use this namespace to define any custom GUI widgets that collect FilterParameters
* for a filter. Do NOT define general reusable widgets here.
*/
namespace FilterParameterWidgetType
{
/* const QString SomeCustomWidget("SomeCustomWidget"); */
}
#endif
