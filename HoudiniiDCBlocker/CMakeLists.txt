cmake_minimum_required(VERSION 3.25)
project(HoudiniiDCBlocker VERSION 1.0.0)

juce_add_plugin(HoudiniiDCBlocker
        COMPANY_NAME    "Houdinii"
        BUNDLE_ID       "com.houdinii.houdiniidcblocker"
        FORMATS         VST3 AU Standalone
        PRODUCT_NAME    "Houdinii DC Blocker"
)

# Tell CMake which files belong in this plugin
target_sources(HoudiniiDCBlocker PRIVATE
        source/DCBlockerPluginProcessor.cpp
        source/DCBlockerPluginEditor.cpp
)

# Link against your shared modules
target_link_libraries(HoudiniiDCBlocker PRIVATE
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
)
