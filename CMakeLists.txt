cmake_minimum_required(VERSION 3.25)

# — Project metadata
project(Houdiniijuce VERSION 0.0.1 LANGUAGES CXX)

# — Helpers
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(PamplejuceVersion)
include(CPM)
include(PamplejuceMacOS)
include(JUCEDefaults)

# — Optional: custom VST3 install path
if (WIN32)
    set(CMAKE_INSTALL_PREFIX "E:/Audio/vsts/VST3" CACHE PATH "" FORCE)
endif()

# — JUCE + CLAP
add_subdirectory(JUCE)
add_subdirectory(modules/clap-juce-extensions EXCLUDE_FROM_ALL)

# — Your modules
add_subdirectory(modules/melatonin_inspector)
add_subdirectory(modules/houdinii_core)
add_subdirectory(modules/houdinii_effects)
add_subdirectory(modules/houdinii_gui)

# — PROJECT-WIDE Embed binary assets (Add individual plugin assets in plugins section):
include(Assets)

# PLUGINS SECTION - This is where you add new plugins!!

# region HoudiniiJuce
# — Gather plugin sources
file(GLOB_RECURSE PluginSources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Houdiniijuce/source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Houdiniijuce/source/*.h"
)

# — Compile them into an OBJECT lib
add_library(SharedCode OBJECT ${PluginSources})

# — Link the OBJECT lib against your INTERFACE modules
#    so their include-dirs (and any other usage requirements) get applied.
target_link_libraries(SharedCode
        PRIVATE
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
)

# — Embed binary assets - Only if used!
# include(pluginDir/Assets)

# — Define your plugin
juce_add_plugin(Houdiniijuce
        ICON_BIG      "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
        COMPANY_NAME  "Houdinii"
        BUNDLE_ID     "com.houdinii.houdiniijuce"
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Houd
        PLUGIN_CODE            H001
        FORMATS               Standalone AU VST3 AUv3
        PRODUCT_NAME          "Houdiniijuce Demo"
)

# — Register CLAP (after the JUCE target exists!)
clap_juce_extensions_plugin(
        TARGET       Houdiniijuce
        CLAP_ID      "com.houdinii.houdiniijuce"
        CLAP_FEATURES audio-effect
)

# — Link everything into your plugin target
target_link_libraries(Houdiniijuce
        PRIVATE
        $<TARGET_OBJECTS:SharedCode>
        Assets
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
# endregion HoudiniiJuce

# region HoudiniiCompressor

# — Gather HoudiniiCompressor plugin sources
file(GLOB_RECURSE CompressorPluginSources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiCompressor/source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiCompressor/source/*.h"
)

# — Compile them into an OBJECT lib for HoudiniiCompressor
add_library(CompressorSharedCode OBJECT ${CompressorPluginSources})

# — Link the OBJECT lib against dependencies
target_link_libraries(CompressorSharedCode
        PRIVATE
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
)

# — Define your plugin
juce_add_plugin(HoudiniiCompressor
        ICON_BIG      "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
        COMPANY_NAME  "Houdinii"
        BUNDLE_ID     "com.houdinii.houdiniicompressor"
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Houd
        PLUGIN_CODE            H002
        FORMATS               Standalone AU VST3 AUv3
        PRODUCT_NAME          "Houdinii Compressor"
)

# — Register CLAP for HoudiniiCompressor
clap_juce_extensions_plugin(
        TARGET       HoudiniiCompressor
        CLAP_ID      "com.houdinii.houdiniicompressor"
        CLAP_FEATURES audio-effect compression
)

# — Link everything into your second plugin target
target_link_libraries(HoudiniiCompressor
        PRIVATE
        $<TARGET_OBJECTS:CompressorSharedCode>
        Assets
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
# endregion HoudiniiCompressor

# region DCBlocker
# — Gather plugin sources
file(GLOB_RECURSE DCBlockerPluginSources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiDCBlocker/source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiDCBlocker/source/*.h"
)

# — Compile them into an OBJECT lib
add_library(DCBlockerSharedCode OBJECT ${DCBlockerPluginSources})

# — Link the OBJECT lib against your INTERFACE modules
#    so their include-dirs (and any other usage requirements) get applied.
target_link_libraries(DCBlockerSharedCode
        PRIVATE
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
)

# — Embed binary assets - Only if used!
# include(pluginDir/Assets)

# — Define your plugin (same info as CMakeLists from plugin dir)
juce_add_plugin(HoudiniiDCBlocker
        ICON_BIG      "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
        COMPANY_NAME  "Houdinii"
        BUNDLE_ID     "com.houdinii.houdiniidcblocker"
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Houd
        PLUGIN_CODE            H002
        FORMATS               Standalone AU VST3 AUv3
        PRODUCT_NAME          "Houdinii DCBlocker"
)

# — Register CLAP for HoudiniiDCBlocker
clap_juce_extensions_plugin(
        TARGET       HoudiniiDCBlocker
        CLAP_ID      "com.houdinii.houdiniiDCBlocker"
        CLAP_FEATURES audio-effect filter
)

# — Link everything into your plugin target
target_link_libraries(HoudiniiDCBlocker
        PRIVATE
        $<TARGET_OBJECTS:DCBlockerSharedCode>
        Assets
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
# endregion DCBlocker

# region Gain
# — Gather plugin sources
file(GLOB_RECURSE GainPluginSources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiGain/source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiGain/source/*.h"
)

# — Compile them into an OBJECT lib
add_library(GainSharedCode OBJECT ${GainPluginSources})

# — Link the OBJECT lib against your INTERFACE modules
#    so their include-dirs (and any other usage requirements) get applied.
target_link_libraries(GainSharedCode
        PRIVATE
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
)

# — Embed binary assets - Only if used!
# include(pluginDir/Assets)

# — Define your plugin (same info as CMakeLists from plugin dir)
juce_add_plugin(HoudiniiGain
        ICON_BIG      "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
        COMPANY_NAME  "Houdinii"
        BUNDLE_ID     "com.houdinii.houdiniigain"
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Houd
        PLUGIN_CODE            H002
        FORMATS               Standalone AU VST3 AUv3
        PRODUCT_NAME          "Houdinii Gain"
)

# — Register CLAP for HoudiniiGain
clap_juce_extensions_plugin(
        TARGET       HoudiniiGain
        CLAP_ID      "com.houdinii.houdiniigain"
        CLAP_FEATURES audio-effect gain
)

# — Link everything into your plugin target
target_link_libraries(HoudiniiGain
        PRIVATE
        $<TARGET_OBJECTS:GainSharedCode>
        Assets
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
# endregion Gain

# region SoftClipper
# — Gather plugin sources
file(GLOB_RECURSE SoftClipperPluginSources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiSoftClipper/source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiSoftClipper/source/*.h"
)

# — Compile them into an OBJECT lib
add_library(SoftClipperSharedCode OBJECT ${SoftClipperPluginSources})

# — Link the OBJECT lib against your INTERFACE modules
#    so their include-dirs (and any other usage requirements) get applied.
target_link_libraries(SoftClipperSharedCode
        PRIVATE
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
)

# — Embed binary assets - Only if used!
# include(pluginDir/Assets)

# — Define your plugin (same info as CMakeLists from plugin dir)
juce_add_plugin(HoudiniiSoftClipper
        ICON_BIG      "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
        COMPANY_NAME  "Houdinii"
        BUNDLE_ID     "com.houdinii.houdiniisoftclipper"
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Houd
        PLUGIN_CODE            H002
        FORMATS               Standalone AU VST3 AUv3
        PRODUCT_NAME          "Houdinii Soft Clipper"
)

# — Register CLAP for HoudiniiSoftClipper
clap_juce_extensions_plugin(
        TARGET       HoudiniiSoftClipper
        CLAP_ID      "com.houdinii.houdiniisoftclipper"
        CLAP_FEATURES audio-effect clipper
)

# — Link everything into your plugin target
target_link_libraries(HoudiniiSoftClipper
        PRIVATE
        $<TARGET_OBJECTS:SoftClipperSharedCode>
        Assets
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
# endregion SoftClipper

# region LowPassFilter
# — Gather plugin sources
file(GLOB_RECURSE LowPassFilterPluginSources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiLowPassFilter/source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiLowPassFilter/source/*.h"
)

# — Compile them into an OBJECT lib
add_library(LowPassFilterSharedCode OBJECT ${LowPassFilterPluginSources})

# — Link the OBJECT lib against your INTERFACE modules
#    so their include-dirs (and any other usage requirements) get applied.
target_link_libraries(LowPassFilterSharedCode
        PRIVATE
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
)

# — Embed binary assets - Only if used!
# include(pluginDir/Assets)

# — Define your plugin (same info as CMakeLists from plugin dir)
juce_add_plugin(HoudiniiLowPassFilter
        ICON_BIG      "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
        COMPANY_NAME  "Houdinii"
        BUNDLE_ID     "com.houdinii.houdiniilowpassfilter"
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Houd
        PLUGIN_CODE            H002
        FORMATS               Standalone AU VST3 AUv3
        PRODUCT_NAME          "Houdinii Low Pass Filter"
)

# — Register CLAP for HoudiniiLowPassFilter
clap_juce_extensions_plugin(
        TARGET       HoudiniiLowPassFilter
        CLAP_ID      "com.houdinii.houdiniilowpassfilter"
        CLAP_FEATURES audio-effect LowPassFilter
)

# — Link everything into your plugin target
target_link_libraries(HoudiniiLowPassFilter
        PRIVATE
        $<TARGET_OBJECTS:LowPassFilterSharedCode>
        Assets
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
# endregion LowPassFilter

# region StereoWidener
# — Gather plugin sources
file(GLOB_RECURSE StereoWidenerPluginSources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiStereoWidener/source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiStereoWidener/source/*.h"
)

# — Compile them into an OBJECT lib
add_library(StereoWidenerSharedCode OBJECT ${StereoWidenerPluginSources})

# — Link the OBJECT lib against your INTERFACE modules
#    so their include-dirs (and any other usage requirements) get applied.
target_link_libraries(StereoWidenerSharedCode
        PRIVATE
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
)

# — Embed binary assets - Only if used!
# include(pluginDir/Assets)

# — Define your plugin (same info as CMakeLists from plugin dir)
juce_add_plugin(HoudiniiStereoWidener
        ICON_BIG      "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
        COMPANY_NAME  "Houdinii"
        BUNDLE_ID     "com.houdinii.houdiniistereowidener"
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Houd
        PLUGIN_CODE            H002
        FORMATS               Standalone AU VST3 AUv3
        PRODUCT_NAME          "Houdinii Stereo Widener"
)

# — Register CLAP for HoudiniiStereoWidener
clap_juce_extensions_plugin(
        TARGET       HoudiniiStereoWidener
        CLAP_ID      "com.houdinii.houdiniistereowidener"
        CLAP_FEATURES audio-effect StereoWidener
)

# — Link everything into your plugin target
target_link_libraries(HoudiniiStereoWidener
        PRIVATE
        $<TARGET_OBJECTS:StereoWidenerSharedCode>
        Assets
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
# endregion StereoWidener

# — Extras
include(PamplejuceIPP)
include(Tests)
include(Benchmarks)
include(GitHubENV)
