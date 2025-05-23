#include "../include/LowPassFilterPluginEditor.h"

LowPassFilterPluginEditor::LowPassFilterPluginEditor(LowPassFilterPluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    addAndMakeVisible(inspectButton);
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector>(*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }
        inspector->setVisible(true);
    };

    setSize(400, 300);
}

LowPassFilterPluginEditor::~LowPassFilterPluginEditor() = default;

void LowPassFilterPluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("HoudiniiDistort", getLocalBounds().removeFromTop(50),
               juce::Justification::centred, true);

    g.setFont(16.0f);
    g.drawText("Empty plugin ready for distortion effects!",
               getLocalBounds().withTrimmedTop(50).withTrimmedBottom(50),
               juce::Justification::centred, true);
}

void LowPassFilterPluginEditor::resized()
{
    inspectButton.setBounds(getLocalBounds().removeFromBottom(30).removeFromRight(120).reduced(5));
}