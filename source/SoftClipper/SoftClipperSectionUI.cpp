#include "SoftClipperSectionUI.h"

SoftClipperSectionUI::SoftClipperSectionUI(juce::AudioProcessorValueTreeState& valueTreeState)
    : EffectSectionUI("Soft Clipper", SoftClipperSection::SOFTCLIP_ENABLED_ID, valueTreeState)
{
}

void SoftClipperSectionUI::initialize()
{
    // Drive slider
    addAndMakeVisible(driveSlider);
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    addAndMakeVisible(driveLabel);
    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.attachToComponent(&driveSlider, false);

    // Type combo box
    addAndMakeVisible(typeComboBox);
    typeComboBox.addItem("Tanh", 1);
    typeComboBox.addItem("Cubic", 2);
    typeComboBox.addItem("Arctan", 3);

    addAndMakeVisible(typeLabel);
    typeLabel.setText("Type", juce::dontSendNotification);
    typeLabel.setJustificationType(juce::Justification::centred);
    typeLabel.attachToComponent(&typeComboBox, false);

    // Mix slider
    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    mixSlider.setTextValueSuffix("%");

    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.attachToComponent(&mixSlider, false);

    // Create parameter attachments
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, SoftClipperSection::SOFTCLIP_DRIVE_ID, driveSlider);
    typeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        valueTreeState, SoftClipperSection::SOFTCLIP_TYPE_ID, typeComboBox);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, SoftClipperSection::SOFTCLIP_MIX_ID, mixSlider);

    // Custom display for mix as percentage
    mixSlider.textFromValueFunction = [](double value) {
        return juce::String(static_cast<int>(value * 100)) + "%";
    };
    mixSlider.valueFromTextFunction = [](const juce::String& text) {
        return text.getDoubleValue() / 100.0;
    };
}

void SoftClipperSectionUI::layoutControls(juce::Rectangle<int> area)
{
    area.removeFromTop(15); // Space for labels

    // Arrange controls in a row
    const int controlWidth = area.getWidth() / 3;

    auto driveArea = area.removeFromLeft(controlWidth).reduced(5);
    driveArea.removeFromTop(15);
    driveSlider.setBounds(driveArea.withSizeKeepingCentre(60, 60));

    auto typeArea = area.removeFromLeft(controlWidth).reduced(5);
    typeArea.removeFromTop(15);
    typeComboBox.setBounds(typeArea.removeFromTop(25).reduced(10, 0));

    auto mixArea = area.reduced(5);
    mixArea.removeFromTop(15);
    mixSlider.setBounds(mixArea.withSizeKeepingCentre(60, 60));
}