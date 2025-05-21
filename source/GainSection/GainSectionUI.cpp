#include "GainSectionUI.h"

GainSectionUI::GainSectionUI(juce::AudioProcessorValueTreeState& valueTreeState)
    : EffectSectionUI("Gain Stage", GainSection::GAIN_ENABLED_ID, valueTreeState)
{
}

void GainSectionUI::initialize()
{
    // Input gain slider
    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    inputGainSlider.setTextValueSuffix(" dB");

    addAndMakeVisible(inputGainLabel);
    inputGainLabel.setText("Input Gain", juce::dontSendNotification);
    inputGainLabel.setJustificationType(juce::Justification::centred);
    inputGainLabel.attachToComponent(&inputGainSlider, false);

    // Output gain slider
    addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    outputGainSlider.setTextValueSuffix(" dB");

    addAndMakeVisible(outputGainLabel);
    outputGainLabel.setText("Output Gain", juce::dontSendNotification);
    outputGainLabel.setJustificationType(juce::Justification::centred);
    outputGainLabel.attachToComponent(&outputGainSlider, false);

    // Create parameter attachments
    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, GainSection::INPUT_GAIN_ID, inputGainSlider);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, GainSection::OUTPUT_GAIN_ID, outputGainSlider);
}

void GainSectionUI::layoutControls(juce::Rectangle<int> area)
{
    // Remove less space for labels
    area.removeFromTop(15); // Reduced from 20

    auto knobSize = juce::jmin(80, area.getWidth() / 2 - 10); // Smaller knobs
    auto knobHeight = area.getHeight() - 20; // Leave some padding

    auto inputArea = area.removeFromLeft(area.getWidth() / 2);
    auto outputArea = area;

    inputGainSlider.setBounds(inputArea.withSizeKeepingCentre(knobSize, knobHeight));
    outputGainSlider.setBounds(outputArea.withSizeKeepingCentre(knobSize, knobHeight));
}