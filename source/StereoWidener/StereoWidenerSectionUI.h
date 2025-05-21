#pragma once

#include "../Common/EffectSectionUI.h"
#include "StereoWidenerSection.h"

class StereoWidenerSectionUI : public EffectSectionUI
{
public:
    StereoWidenerSectionUI(juce::AudioProcessorValueTreeState& valueTreeState);
    ~StereoWidenerSectionUI() override = default;

    void initialize() override;

protected:
    void layoutControls(juce::Rectangle<int> area) override;

private:
    juce::Slider widthSlider;
    juce::Label widthLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoWidenerSectionUI)
};