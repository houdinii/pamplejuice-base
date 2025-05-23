#pragma once

#include "TemplatePluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <melatonin_inspector/melatonin_inspector.h>

class TemplatePluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit TemplatePluginEditor(TemplatePluginProcessor&);
    ~TemplatePluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    TemplatePluginProcessor& processorRef;

    juce::TextButton inspectButton { "Inspect the UI" };
    std::unique_ptr<melatonin::Inspector> inspector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TemplatePluginEditor)
};