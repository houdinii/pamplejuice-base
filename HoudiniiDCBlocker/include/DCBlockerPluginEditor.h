#pragma once

#include "DCBlockerPluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <melatonin_inspector/melatonin_inspector.h>

class DCBlockerPluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit DCBlockerPluginEditor(DCBlockerPluginProcessor&);
    ~DCBlockerPluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    DCBlockerPluginProcessor& processorRef;

    juce::TextButton inspectButton { "Inspect the UI" };
    std::unique_ptr<melatonin::Inspector> inspector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DCBlockerPluginEditor)
};