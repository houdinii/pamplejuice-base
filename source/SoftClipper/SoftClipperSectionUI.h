#pragma once

#include "../Common/EffectSectionUI.h"
#include "SoftClipperSection.h"

class SoftClipperSectionUI : public EffectSectionUI
{
public:
    SoftClipperSectionUI(juce::AudioProcessorValueTreeState& valueTreeState);
    ~SoftClipperSectionUI() override = default;

    void initialize() override;

protected:
    void layoutControls(juce::Rectangle<int> area) override;

private:
    juce::Slider driveSlider;
    juce::Label driveLabel;

    juce::ComboBox typeComboBox;
    juce::Label typeLabel;

    juce::Slider mixSlider;
    juce::Label mixLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoftClipperSectionUI)
};