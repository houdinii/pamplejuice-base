cmake_minimum_required(VERSION 3.25)
project(HoudiniiLowPassFilter VERSION 1.0.0)

juce_add_plugin(HoudiniiLowPassFilter
        COMPANY_NAME    "Houdinii"
        BUNDLE_ID       "com.houdinii.houdiniilowpassfilter"
        FORMATS         VST3 AU Standalone
        PRODUCT_NAME    "Houdinii Low Pass Filter"
)

# Tell CMake which files belong in this plugin
target_sources(HoudiniiLowPassFilter PRIVATE
        source/LowPassFilterPluginProcessor.cpp
        source/LowPassFilterPluginEditor.cpp
)

# Link against your shared modules
target_link_libraries(HoudiniiLowPassFilter PRIVATE
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
)
