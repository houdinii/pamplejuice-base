/*
==============================================================================

   This file is part of the Houdinii Effects module
   Copyright (c) 2024 - Houdinii

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:               houdinii_effects
  vendor:           Houdinii
  version:          1.0.0
  name:             Houdinii Effects
  description:      Audio effects for Houdinii's plugins
  website:          https://yourwebsite.com
  license:          MIT
  minimumCppStandard: 17

  dependencies:     houdinii_core, juce_audio_basics, juce_audio_processors, juce_dsp

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once
#define HOUDINII_EFFECTS_H_INCLUDED

#include <houdinii_core/houdinii_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

namespace houdinii {

// Module version
static const char* houdinii_effectsVersion = "1.0.0";

}

// Include implementation files
#include "distortion/SoftClipperSection.h"
#include "dynamics/CompressorSection.h"
#include "filter/DCBlockerSection.h"
#include "filter/LowPassFilterSection.h"
#include "gain/GainSection.h"
#include "modulation/StereoWidenerSection.h"