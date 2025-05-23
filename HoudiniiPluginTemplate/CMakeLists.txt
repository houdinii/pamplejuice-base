cmake_minimum_required(VERSION 3.25)
project(HoudiniiPluginTemplate VERSION 1.0.0)

juce_add_plugin(HoudiniiPluginTemplate
        COMPANY_NAME    "Houdinii"
        BUNDLE_ID       "com.houdinii.houdiniiplugintemplate"
        FORMATS         VST3 AU Standalone
        PRODUCT_NAME    "Houdinii Plugin Template"
)

# Tell CMake which files belong in this plugin
target_sources(HoudiniiPluginTemplate PRIVATE
        source/TemplatePluginProcessor.cpp
        source/TemplatePluginEditor.cpp
)

# Link against your shared modules
target_link_libraries(HoudiniiPluginTemplate PRIVATE
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
)
