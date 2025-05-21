#pragma once

#include "EffectSectionUI.h"
#include "GainSection.h"

class GainSectionUI : public EffectSectionUI
{
public:
    GainSectionUI(juce::AudioProcessorValueTreeState& valueTreeState);
    ~GainSectionUI() override = default;

    void initialize() override;

protected:
    void layoutControls(juce::Rectangle<int> area) override;

private:
    juce::Slider inputGainSlider;
    juce::Slider outputGainSlider;
    juce::Label inputGainLabel;
    juce::Label outputGainLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainSectionUI)
};