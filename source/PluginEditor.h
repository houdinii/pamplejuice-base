#pragma once

#include "../modules/houdinii_effects/filter/DCBlockerSectionUI.h"
#include "../modules/houdinii_effects/modulation/StereoWidenerSectionUI.h"
#include "../modules/houdinii_gui/components/CompressorSectionUI.h"
#include "../modules/houdinii_gui/components/GainSectionUI.h"
#include "../modules/houdinii_gui/components/MeterSectionUI.h"
#include "../modules/houdinii_gui/components/SoftClipperSectionUI.h"
#include "LowPassFilter/LowPassFilterSectionUI.h"
#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PluginProcessor& processorRef;

    // UI Inspector
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };

    // Effect section UIs
    std::unique_ptr<GainSectionUI> gainSectionUI;
    std::unique_ptr<DCBlockerSectionUI> dcBlockerSectionUI;
    std::unique_ptr<LowPassFilterSectionUI> lowPassFilterSectionUI;
    std::unique_ptr<StereoWidenerSectionUI> stereoWidenerSectionUI;
    std::unique_ptr<MeterSectionUI> meterSectionUI;
    std::unique_ptr<CompressorSectionUI> compressorSectionUI;
    std::unique_ptr<SoftClipperSectionUI> softClipperSectionUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};