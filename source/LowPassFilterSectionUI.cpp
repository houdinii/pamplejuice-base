#include "LowPassFilterSectionUI.h"

LowPassFilterSectionUI::LowPassFilterSectionUI(juce::AudioProcessorValueTreeState& valueTreeState)
    : EffectSectionUI("Low Pass Filter", LowPassFilterSection::LOWPASS_ENABLED_ID, valueTreeState)
{
    // No virtual function calls in constructor
}

void LowPassFilterSectionUI::initialize()
{
    addAndMakeVisible(cutoffSlider);
    cutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    cutoffSlider.setTextValueSuffix(" Hz");

    addAndMakeVisible(cutoffLabel);
    cutoffLabel.setText("Cutoff", juce::dontSendNotification);
    cutoffLabel.setJustificationType(juce::Justification::centred);
    cutoffLabel.attachToComponent(&cutoffSlider, false);

    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, LowPassFilterSection::LOWPASS_CUTOFF_ID, cutoffSlider);
}

void LowPassFilterSectionUI::layoutControls(juce::Rectangle<int> area)
{
    area.removeFromTop(20); // Space for label
    cutoffSlider.setBounds(area.withSizeKeepingCentre(100, 100));
}