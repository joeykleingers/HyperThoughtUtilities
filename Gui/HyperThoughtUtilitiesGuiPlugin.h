#pragma once

#include "HyperThoughtUtilities/HyperThoughtUtilitiesPlugin.h"

class HyperThoughtUtilitiesGuiPlugin : public HyperThoughtUtilitiesPlugin
{
  Q_OBJECT
  Q_INTERFACES(ISIMPLibPlugin)
  Q_PLUGIN_METADATA(IID "com.your.domain.HyperThoughtUtilitiesGuiPlugin")

public:
  HyperThoughtUtilitiesGuiPlugin();
  ~HyperThoughtUtilitiesGuiPlugin() override;
  
  /**
   * @brief Register all the filters with the FilterWidgetFactory
   */
  void registerFilterWidgets(FilterWidgetManager* fwm) override;

public:
  HyperThoughtUtilitiesGuiPlugin(const HyperThoughtUtilitiesGuiPlugin&) = delete;            // Copy Constructor Not Implemented
  HyperThoughtUtilitiesGuiPlugin(HyperThoughtUtilitiesGuiPlugin&&) = delete;                 // Move Constructor
  HyperThoughtUtilitiesGuiPlugin& operator=(const HyperThoughtUtilitiesGuiPlugin&) = delete; // Copy Assignment Not Implemented
  HyperThoughtUtilitiesGuiPlugin& operator=(HyperThoughtUtilitiesGuiPlugin&&) = delete;      // Move Assignment Not Implemented
};
