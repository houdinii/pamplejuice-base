add_library(houdinii_gui INTERFACE)

target_sources(houdinii_gui INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/houdinii_gui.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/EffectSectionUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/EffectSectionUI.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/GainSectionUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/GainSectionUI.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/DCBlockerSectionUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/DCBlockerSectionUI.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/LowPassFilterSectionUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/LowPassFilterSectionUI.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/CompressorSectionUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/CompressorSectionUI.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/SoftClipperSectionUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/SoftClipperSectionUI.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/StereoWidenerSectionUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/StereoWidenerSectionUI.h
        ${CMAKE_CURRENT_SOURCE_DIR}/components/MeterSectionUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/MeterSectionUI.h
        ${CMAKE_CURRENT_SOURCE_DIR}/visualizers/WaveShapeVisualizerComponent.h
        ${CMAKE_CURRENT_SOURCE_DIR}/visualizers/WaveShapeVisualizerComponent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/visualizers/MeterSection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/visualizers/MeterSection.h
)

target_include_directories(houdinii_gui
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(houdinii_gui
        INTERFACE
        houdinii_core
        houdinii_effects
        juce::juce_gui_basics
        juce::juce_audio_processors
)