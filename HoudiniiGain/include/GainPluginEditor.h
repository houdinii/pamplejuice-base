#pragma once

#include "GainPluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <melatonin_inspector/melatonin_inspector.h>

class GainPluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit GainPluginEditor(GainPluginProcessor&);
    ~GainPluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    GainPluginProcessor& processorRef;

    juce::TextButton inspectButton { "Inspect the UI" };
    std::unique_ptr<melatonin::Inspector> inspector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainPluginEditor)
};