#pragma once

#include "LowPassFilterPluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <melatonin_inspector/melatonin_inspector.h>

class LowPassFilterPluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit LowPassFilterPluginEditor(LowPassFilterPluginProcessor&);
    ~LowPassFilterPluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    LowPassFilterPluginProcessor& processorRef;

    juce::TextButton inspectButton { "Inspect the UI" };
    std::unique_ptr<melatonin::Inspector> inspector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LowPassFilterPluginEditor)
};