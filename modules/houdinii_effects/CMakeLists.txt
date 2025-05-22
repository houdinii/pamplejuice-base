add_library(houdinii_effects INTERFACE)

target_sources(houdinii_effects INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/houdinii_effects.h
        ${CMAKE_CURRENT_SOURCE_DIR}/distortion/SoftClipperSection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/distortion/SoftClipperSection.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/CompressorSection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/CompressorSection.h
        ${CMAKE_CURRENT_SOURCE_DIR}/filter/DCBlockerSection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filter/DCBlockerSection.h
        ${CMAKE_CURRENT_SOURCE_DIR}/filter/LowPassFilterSection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filter/LowPassFilterSection.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gain/GainSection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gain/GainSection.h
        ${CMAKE_CURRENT_SOURCE_DIR}/modulation/StereoWidenerSection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/modulation/StereoWidenerSection.h
)

target_include_directories(houdinii_effects
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(houdinii_effects
        INTERFACE
        houdinii_core
        juce::juce_audio_basics
        juce::juce_audio_processors
        juce::juce_dsp
)