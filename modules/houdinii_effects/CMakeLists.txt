# Create the module library
juce_add_module(houdinii_effects
        SOURCES
        houdinii_effects.cpp
        houdinii_effects.h
        distortion/SoftClipperSection.cpp
        distortion/SoftClipperSection.h
        dynamics/CompressorSection.cpp
        dynamics/CompressorSection.h
        filter/DCBlockerSection.cpp
        filter/DCBlockerSection.h
        filter/LowPassFilterSection.cpp
        filter/LowPassFilterSection.h
        gain/GainSection.cpp
        gain/GainSection.h
        modulation/StereoWidenerSection.cpp
        modulation/StereoWidenerSection.h
)

# Set include directories
target_include_directories(houdinii_effects
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link JUCE dependencies
target_link_libraries(houdinii_effects
        PUBLIC
        houdinii_core
        juce::juce_audio_basics
        juce::juce_audio_processors
        juce::juce_dsp
)