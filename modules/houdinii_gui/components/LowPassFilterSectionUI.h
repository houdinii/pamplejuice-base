#pragma once

#include "../../houdinii_effects/filter/LowPassFilterSection.h"
#include "EffectSectionUI.h"

namespace houdinii
{
    class LowPassFilterSectionUI : public EffectSectionUI
    {
    public:
        explicit LowPassFilterSectionUI (juce::AudioProcessorValueTreeState& valueTreeState);
        ~LowPassFilterSectionUI() override = default;

        void initialize() override;

    protected:
        void layoutControls (juce::Rectangle<int> area) override;

    private:
        juce::Slider cutoffSlider;
        juce::Label cutoffLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowPassFilterSectionUI)
    };
}
