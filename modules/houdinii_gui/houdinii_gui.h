/*
==============================================================================

   This file is part of the Houdinii GUI module
   Copyright (c) 2024 - Houdinii

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:               houdinii_gui
  vendor:           Houdinii
  version:          1.0.0
  name:             Houdinii GUI
  description:      GUI components for Houdinii's audio plugins
  website:          https://yourwebsite.com
  license:          MIT
  minimumCppStandard: 17

  dependencies:     houdinii_core, houdinii_effects, juce_gui_basics, juce_audio_processors

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once
#define HOUDINII_GUI_H_INCLUDED

#include <houdinii_core/houdinii_core.h>
#include <houdinii_effects/houdinii_effects.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace houdinii {
    // Module version
    static const char* houdinii_guiVersion = "1.0.0";
}

// Include implementation files
#include "components/EffectSectionUI.h"
#include "components/GainSectionUI.h"
#include "components/DCBlockerSectionUI.h"
#include "components/LowPassFilterSectionUI.h"
#include "components/CompressorSectionUI.h"
#include "components/SoftClipperSectionUI.h"
#include "components/StereoWidenerSectionUI.h"
#include "components/MeterSectionUI.h"
#include "visualizers/LevelMeterComponent.h"
#include "visualizers/WaveShapeVisualizerComponent.h"