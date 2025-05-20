#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // UI Inspector setup
    addAndMakeVisible (inspectButton);
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }
        inspector->setVisible (true);
    };

    // Input gain slider setup
    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    inputGainSlider.setTextValueSuffix(" dB");

    addAndMakeVisible(inputGainLabel);
    inputGainLabel.setText("Input Gain", juce::dontSendNotification);
    inputGainLabel.setJustificationType(juce::Justification::centred);
    inputGainLabel.attachToComponent(&inputGainSlider, false);

    // Output gain slider setup
    addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    outputGainSlider.setTextValueSuffix(" dB");

    addAndMakeVisible(outputGainLabel);
    outputGainLabel.setText("Output Gain", juce::dontSendNotification);
    outputGainLabel.setJustificationType(juce::Justification::centred);
    outputGainLabel.attachToComponent(&outputGainSlider, false);

    // Create parameter attachments (this is what makes automation work!)
    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getValueTreeState(), "inputGain", inputGainSlider);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getValueTreeState(), "outputGain", outputGainSlider);

    setSize (400, 300);
}

PluginEditor::~PluginEditor()
= default;

void PluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (18.0f);
    g.drawText ("Bass Gain Stager", getLocalBounds().removeFromTop(40), juce::Justification::centred, true);
}

void PluginEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(40); // Space for title

    // Inspector button in the bottom corner
    inspectButton.setBounds(bounds.removeFromBottom(30).removeFromRight(120).reduced(5));

    // Split the remaining area for the two knobs
    auto knobArea = bounds.reduced (20);
    auto knobWidth = knobArea.getWidth() / 2;

    auto inputArea = knobArea.removeFromLeft(knobWidth).reduced(10);
    auto outputArea = knobArea.reduced(10);

    // Leave space for labels
    inputArea.removeFromTop(20);
    outputArea.removeFromTop(20);

    inputGainSlider.setBounds(inputArea);
    outputGainSlider.setBounds(outputArea);
}