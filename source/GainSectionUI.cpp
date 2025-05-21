#include "GainSectionUI.h"

GainSectionUI::GainSectionUI(juce::AudioProcessorValueTreeState& valueTreeState)
    : EffectSectionUI("Gain Stage", GainSection::GAIN_ENABLED_ID, valueTreeState)
{
    // No addControls call here!
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
    auto knobWidth = area.getWidth() / 2;

    auto inputArea = area.removeFromLeft(knobWidth).reduced(10);
    auto outputArea = area.reduced(10);

    // Leave space for labels
    inputArea.removeFromTop(20);
    outputArea.removeFromTop(20);

    inputGainSlider.setBounds(inputArea);
    outputGainSlider.setBounds(outputArea);
}