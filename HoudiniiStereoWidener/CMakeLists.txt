cmake_minimum_required(VERSION 3.25)
project(HoudiniiStereoWidener VERSION 1.0.0)

juce_add_plugin(HoudiniiStereoWidener
        COMPANY_NAME    "Houdinii"
        BUNDLE_ID       "com.houdinii.houdiniistereowidener"
        FORMATS         VST3 AU Standalone
        PRODUCT_NAME    "Houdinii Stereo Widener"
)

# Tell CMake which files belong in this plugin
target_sources(HoudiniiStereoWidener PRIVATE
        source/StereoWidenerPluginProcessor.cpp
        source/StereoWidenerPluginEditor.cpp
)

# Link against your shared modules
target_link_libraries(HoudiniiStereoWidener PRIVATE
        houdinii_c
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
)
