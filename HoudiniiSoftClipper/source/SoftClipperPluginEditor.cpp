#include "../include/SoftClipperPluginEditor.h"

SoftClipperPluginEditor::SoftClipperPluginEditor(SoftClipperPluginProcessor& p)
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

SoftClipperPluginEditor::~SoftClipperPluginEditor() = default;

void SoftClipperPluginEditor::paint(juce::Graphics& g)
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

void SoftClipperPluginEditor::resized()
{
    inspectButton.setBounds(getLocalBounds().removeFromBottom(30).removeFromRight(120).reduced(5));
}