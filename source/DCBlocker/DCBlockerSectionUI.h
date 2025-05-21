#pragma once

#include "../Common/EffectSectionUI.h"
#include "DCBlockerSection.h"

class DCBlockerSectionUI : public EffectSectionUI
{
public:
    DCBlockerSectionUI(juce::AudioProcessorValueTreeState& valueTreeState);
    ~DCBlockerSectionUI() override = default;

    void initialize() override;

protected:
    void layoutControls(juce::Rectangle<int> area) override;

private:
    juce::Slider rSlider;
    juce::Label rLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DCBlockerSectionUI)
};