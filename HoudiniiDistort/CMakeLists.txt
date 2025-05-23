cmake_minimum_required(VERSION 3.25)
project(HoudiniiDistort VERSION 1.0.0)

# Pull in JUCE (already via add_subdirectory(JUCE) in root)
# Pull in your Houdinii modules (via add_subdirectory or FetchContent/CPM)

juce_add_plugin(HoudiniiDistort
        COMPANY_NAME    "Houdinii"
        BUNDLE_ID       "com.houdinii.houdiniidistort"
        FORMATS         VST3 AU Standalone
        PRODUCT_NAME    "Houdinii Distort"
)

# Tell CMake which files belong in this plugin
target_sources(HoudiniiDistort PRIVATE
        source/DistortPluginProcessor.cpp
        source/DistortPluginEditor.cpp
)

# Link against your shared modules
target_link_libraries(HoudiniiDistort PRIVATE
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
)
