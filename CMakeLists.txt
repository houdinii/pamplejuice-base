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

# — Embed binary assets
include(Assets)

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

# — Extras
include(PamplejuceIPP)
include(Tests)
include(Benchmarks)
include(GitHubENV)
