#include "StereoWidenerSectionUI.h"

StereoWidenerSectionUI::StereoWidenerSectionUI(juce::AudioProcessorValueTreeState& valueTreeState)
    : EffectSectionUI("Stereo Widener", StereoWidenerSection::WIDENER_ENABLED_ID, valueTreeState)
{
    // No virtual function calls here
}

void StereoWidenerSectionUI::initialize()
{
    addAndMakeVisible(widthSlider);
    widthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    widthSlider.setTextValueSuffix("%");
    widthSlider.setRange(0.0, 100.0, 1.0);

    addAndMakeVisible(widthLabel);
    widthLabel.setText("Width", juce::dontSendNotification);
    widthLabel.setJustificationType(juce::Justification::centred);
    widthLabel.attachToComponent(&widthSlider, false);

    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, StereoWidenerSection::WIDENER_AMOUNT_ID, widthSlider);

    // Custom value conversion for percent display
    widthSlider.textFromValueFunction = [](double value) {
        return juce::String(static_cast<int>(value * 100)) + "%";
    };

    widthSlider.valueFromTextFunction = [](const juce::String& text) {
        return text.getDoubleValue() / 100.0;
    };
}

void StereoWidenerSectionUI::layoutControls(juce::Rectangle<int> area)
{
    area.removeFromTop(15); // Space for label
    widthSlider.setBounds(area.withSizeKeepingCentre(80, juce::jmin(80, area.getHeight())));
}