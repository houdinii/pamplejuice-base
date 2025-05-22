#pragma once

#include "../../houdinii_core/effects/EffectSection.h"
#include <juce_dsp/juce_dsp.h>

namespace houdinii
{
    class LowPassFilterSection : public EffectSection
    {
    public:
        LowPassFilterSection();
        ~LowPassFilterSection() override = default;

        void prepareToPlay (double sampleRate, int samplesPerBlock) override;
        void processBlock (juce::AudioBuffer<float>& buffer) override;
        void addParametersToLayout (juce::AudioProcessorValueTreeState::ParameterLayout& layout) override;
        void setParameterPointers (juce::AudioProcessorValueTreeState& valueTreeState) override;

        [[nodiscard]] juce::String getSectionName() const override { return "Low Pass Filter"; }
        [[nodiscard]] juce::String getEnableParameterId() const override { return LOWPASS_ENABLED_ID; }

        // Parameter IDs
        static constexpr const char* LOWPASS_CUTOFF_ID = "lowpassCutoff";
        static constexpr const char* LOWPASS_ENABLED_ID = "lowpassEnabled";

    private:
        // Parameter pointers
        std::atomic<float>* cutoffParam = nullptr;
        std::atomic<float>* enabledParam = nullptr;

        // Filter processor
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowPassFilter;

        double sampleRate = 44100.0;

        // Smoothed parameter
        juce::SmoothedValue<float> smoothedCutoff;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowPassFilterSection)
    };
}