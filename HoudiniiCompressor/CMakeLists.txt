cmake_minimum_required(VERSION 3.25)
project(HoudiniiCompressor VERSION 1.0.0)

juce_add_plugin(HoudiniiCompressor
        COMPANY_NAME    "Houdinii"
        BUNDLE_ID       "com.houdinii.houdiniicompressor"
        FORMATS         VST3 AU Standalone
        PRODUCT_NAME    "Houdinii Compressor"
)

# Tell CMake which files belong in this plugin
target_sources(HoudiniiCompressor PRIVATE
        source/CompressorPluginProcessor.cpp
        source/CompressorPluginEditor.cpp
)

# Link against your shared modules
target_link_libraries(HoudiniiCompressor PRIVATE
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
)
