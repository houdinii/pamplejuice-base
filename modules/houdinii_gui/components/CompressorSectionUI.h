#pragma once

#include "../../houdinii_effects/dynamics/CompressorSection.h"
#include "EffectSectionUI.h"

namespace houdinii
{
    // Simple gain reduction meter
    class GainReductionMeter : public juce::Component, private juce::Timer
    {
    public:
        GainReductionMeter();
        ~GainReductionMeter() override;

        void paint (juce::Graphics& g) override;
        void setGainReduction (float gainReductionDb);

    private:
        void timerCallback() override;
        float gainReduction = 0.0f;
        float displayGainReduction = 0.0f;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainReductionMeter)
    };

    class CompressorSectionUI : public EffectSectionUI, private juce::Timer
    {
    public:
        CompressorSectionUI (juce::AudioProcessorValueTreeState& valueTreeState, CompressorSection& compSection);
        ~CompressorSectionUI() override;

        void initialize() override;

    protected:
        void layoutControls (juce::Rectangle<int> area) override;

    private:
        void timerCallback() override;

        CompressorSection& compressorSection;

        juce::Slider thresholdSlider;
        juce::Slider ratioSlider;
        juce::Slider attackSlider;
        juce::Slider releaseSlider;
        juce::Slider makeupSlider;

        juce::Label thresholdLabel;
        juce::Label ratioLabel;
        juce::Label attackLabel;
        juce::Label releaseLabel;
        juce::Label makeupLabel;
        juce::Label grLabel;

        GainReductionMeter grMeter;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> makeupAttachment;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorSectionUI)
    };
}