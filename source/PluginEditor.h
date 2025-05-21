#pragma once

#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"
#include "GainSectionUI.h"
#include "DCBlockerSectionUI.h"

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};