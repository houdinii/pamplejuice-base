cmake_minimum_required(VERSION 3.25)
project(HoudiniiGain VERSION 1.0.0)

juce_add_plugin(HoudiniiGain
        COMPANY_NAME    "Houdinii"
        BUNDLE_ID       "com.houdinii.houdiniigain"
        FORMATS         VST3 AU Standalone
        PRODUCT_NAME    "Houdinii Gain"
)

# Tell CMake which files belong in this plugin
target_sources(HoudiniiGain PRIVATE
        source/GainPluginProcessor.cpp
        source/GainPluginEditor.cpp
)

# Link against your shared modules
target_link_libraries(HoudiniiGain PRIVATE
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
)
