#include "DCBlockerSectionUI.h"

DCBlockerSectionUI::DCBlockerSectionUI(juce::AudioProcessorValueTreeState& valueTreeState)
    : EffectSectionUI("DC Blocker", DCBlockerSection::DC_BLOCKER_ENABLED_ID, valueTreeState)
{
}

void DCBlockerSectionUI::addControls()
{
    addAndMakeVisible(rSlider);
    rSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);

    addAndMakeVisible(rLabel);
    rLabel.setText("R Value", juce::dontSendNotification);
    rLabel.setJustificationType(juce::Justification::centred);
    rLabel.attachToComponent(&rSlider, false);

    rAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, DCBlockerSection::DC_BLOCKER_R_ID, rSlider);
}

void DCBlockerSectionUI::layoutControls(juce::Rectangle<int> area)
{
    area.removeFromTop(20); // Space for label
    rSlider.setBounds(area.withSizeKeepingCentre(100, 100));
}