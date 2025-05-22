# Create the module library
juce_add_module(houdinii_core
        SOURCES
        houdinii_core.cpp
        houdinii_core.h
        effects/EffectSection.h
        effects/EffectSection.cpp
)

# Set include directories
target_include_directories(houdinii_core
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link JUCE dependencies
target_link_libraries(houdinii_core
        PUBLIC
        juce::juce_audio_basics
        juce::juce_audio_processors
        juce::juce_dsp
)