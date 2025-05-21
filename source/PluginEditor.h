#pragma once

#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"
#include "GainSectionUI.h"
#include "DCBlockerSectionUI.h"
#include "LowPassFilterSectionUI.h"
#include "StereoWidenerSectionUI.h"
#include "MeterSectionUI.h"

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};