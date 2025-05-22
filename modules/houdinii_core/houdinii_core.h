/*
==============================================================================

   This file is part of the Houdinii Core module
   Copyright (c) 2024 - Houdinii

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:               houdinii_core
  vendor:           Houdinii
  version:          1.0.0
  name:             Houdinii Core
  description:      Core classes and utilities for Houdinii's audio plugins
  website:          https://yourwebsite.com
  license:          MIT
  minimumCppStandard: 17

  dependencies:     juce_audio_basics, juce_audio_processors, juce_dsp

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once
#define HOUDINII_CORE_H_INCLUDED

// Standard library includes
#include <atomic>
#include <memory>
#include <vector>

// JUCE dependencies
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

// Module includes
namespace houdinii {

    // Module version - use semantic versioning
    static const char* houdinii_coreVersion = "1.0.0";

    // End module namespace
}

// Include implementation files
#include "effects/EffectSection.h"