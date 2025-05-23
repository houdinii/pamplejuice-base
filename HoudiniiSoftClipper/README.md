1. Copy This Folder
    - Rename the folder from `HoudiniiPluginTemplate` to `Houdinii
    {pluginNameHere}` in SnakeCased.
2.  Edit `CMakeLists.txt`
3. - Change the project() line to the folder name exactly
   - Change the `BUNDLE_ID` to match folder name in lowercase 
     (`HoudiniiDistort` becomes `houdiniidistort`).
   - Change the `PRODUCT_NAME` to match folder name, but in Title Case. 
     (`HoudiniiDistort` becomes `Houdinii Distort).
   - Make sure and add ALL sources to the `target_sources`. Only including the 
     cpp file is fine as long as there is an associated header inside the 
     cpp file. Otherwise, you need to include the header as well.
   - If you use any juce libraries, add them to the `target_link_libraries` 
     section. 
   - Replate the words `HoudiniiPluginTemplate` with the folder name. (TODO: 
     Add variables instead)
4. Duplicate existing source files, replacing `TemplatePluginEditor` and 
   `TemplatePluginProcessor` with your plugin names. So `HoudiniiDistort` 
   becomes `HoudiniiPluginEditor(Processor)`. The word 'template' should no 
   longer exist in the plugin directory. 
   - I generally rename the files first, then in each, search and replace 
     `TemplatePlugin` with `{pluginName}Plugin`.
5. In the base `CMakeLists.txt`, search for the words 'PLUGINS SECTION' and 
   add the following:
```cmake

# — Gather plugin sources
file(GLOB_RECURSE TemplatePluginSources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiPluginTemplate/source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/HoudiniiPluginTemplate/source/*.h"
)

# — Compile them into an OBJECT lib
add_library(TemplateSharedCode OBJECT ${TemplatePluginSources})

# — Link the OBJECT lib against your INTERFACE modules
#    so their include-dirs (and any other usage requirements) get applied.
target_link_libraries(TemplateSharedCode
        PRIVATE
        melatonin_inspector
        houdinii_core
        houdinii_effects
        houdinii_gui
)

# — Embed binary assets - Only if used!
# include(pluginDir/Assets)

# — Define your plugin (same info as CMakeLists from plugin dir)
juce_add_plugin(HoudiniiPluginTemplate
        ICON_BIG      "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
        COMPANY_NAME  "Houdinii"
        BUNDLE_ID     "com.houdinii.houdiniiplugintemplate"
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Houd
        PLUGIN_CODE            H002
        FORMATS               Standalone AU VST3 AUv3
        PRODUCT_NAME          "Houdinii Plugin Template"
)

# — Register CLAP for HoudiniiPluginTemplate
clap_juce_extensions_plugin(
        TARGET       HoudiniiPluginTemplate
        CLAP_ID      "com.houdinii.houdiniiplugintemplate"
        CLAP_FEATURES audio-effect template
)

# — Link everything into your plugin target
target_link_libraries(HoudiniiPluginTemplate
        PRIVATE
        $<TARGET_OBJECTS:TemplateSharedCode>
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
```

List of search/replace targets from the above. Open the above into a notepad 
and do search/replace:
```text
TemplatePluginSources               -> {PluginName}PluginSources
HoudiniiPluginTemplate              -> Houdinii{PluginID}
TemplateSharedCode                  -> {PluginID}SharedCode
com.houdinii.houdiniiplugintemplate -> {BUNDLE_ID}
Houdinii Plugin Template            -> "Houdinii {Plugin Name}"
CLAP_FEATURES audio-effect template -> CLAP_FEATURES {actual tags?}
```

To make the VST, go up to the run picker, and choose {PluginName}_VST3 and 
build it!

### FINALLY
Edit this file. Don't be an ass to your future self! Leave notes when possible.
The plugin won't be replaced if Reaper is open, and it was recently used, so 
just close Reaper and try again if you get permission errors.