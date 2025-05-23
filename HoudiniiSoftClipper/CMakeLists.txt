cmake_minimum_required(VERSION 3.25)
project(HoudiniiSoftClipper VERSION 1.0.0)

juce_add_plugin(HoudiniiSoftClipper
        COMPANY_NAME    "Houdinii"
        BUNDLE_ID       "com.houdinii.houdiniisoftclipper"
        FORMATS         VST3 AU Standalone
        PRODUCT_NAME    "Houdinii Soft Clipper"
)

# Tell CMake which files belong in this plugin
target_sources(HoudiniiSoftClipper PRIVATE
        source/SoftClipperProcessor.cpp
        source/SoftClipperEditor.cpp
)

# Link against your shared modules
target_link_libraries(HoudiniiSoftClipper PRIVATE
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
)
