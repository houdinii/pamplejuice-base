#pragma once

#include "../../houdinii_effects/filter/DCBlockerSection.h"
#include "EffectSectionUI.h"

namespace houdinii
{
    class DCBlockerSectionUI : public EffectSectionUI
    {
    public:
        explicit DCBlockerSectionUI (juce::AudioProcessorValueTreeState& valueTreeState);
        ~DCBlockerSectionUI() override = default;

        void initialize() override;

    protected:
        void layoutControls (juce::Rectangle<int> area) override;

    private:
        juce::Slider rSlider;
        juce::Label rLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rAttachment;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DCBlockerSectionUI)
    };
}