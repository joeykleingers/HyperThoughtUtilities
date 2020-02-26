

#include "HyperThoughtUtilitiesGuiPlugin.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HyperThoughtUtilitiesGuiPlugin::HyperThoughtUtilitiesGuiPlugin()
: HyperThoughtUtilitiesPlugin()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HyperThoughtUtilitiesGuiPlugin::~HyperThoughtUtilitiesGuiPlugin() = default;

#include "HyperThoughtUtilities/Gui/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"
