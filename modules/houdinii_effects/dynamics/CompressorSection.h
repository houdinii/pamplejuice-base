#pragma once

#include "../../houdinii_core/effects/EffectSection.h"
#include <vector>

namespace houdinii
{
    class CompressorSection : public EffectSection
    {
    public:
        CompressorSection();
        ~CompressorSection() override = default;

        void prepareToPlay (double sampleRate, int samplesPerBlock) override;
        void processBlock (juce::AudioBuffer<float>& buffer) override;
        void addParametersToLayout (juce::AudioProcessorValueTreeState::ParameterLayout& layout) override;
        void setParameterPointers (juce::AudioProcessorValueTreeState& valueTreeState) override;

        [[nodiscard]] juce::String getSectionName() const override { return "Compressor"; }
        [[nodiscard]] juce::String getEnableParameterId() const override { return COMP_ENABLED_ID; }

        // Parameter IDs
        static constexpr const char* COMP_THRESHOLD_ID = "compThreshold";
        static constexpr const char* COMP_RATIO_ID = "compRatio";
        static constexpr const char* COMP_ATTACK_ID = "compAttack";
        static constexpr const char* COMP_RELEASE_ID = "compRelease";
        static constexpr const char* COMP_MAKEUP_ID = "compMakeup";
        static constexpr const char* COMP_ENABLED_ID = "compEnabled";

        // Get current gain reduction for metering (in dB)
        float getCurrentGainReduction() const { return currentGainReduction.load(); }

    private:
        // Parameter pointers
        std::atomic<float>* thresholdParam = nullptr;
        std::atomic<float>* ratioParam = nullptr;
        std::atomic<float>* attackParam = nullptr;
        std::atomic<float>* releaseParam = nullptr;
        std::atomic<float>* makeupParam = nullptr;
        std::atomic<float>* enabledParam = nullptr;

        // Envelope follower state per channel
        std::vector<float> envelopeState;

        // Time constants
        float attackCoeff = 0.0f;
        float releaseCoeff = 0.0f;
        double sampleRate = 44100.0;

        // For gain reduction metering
        std::atomic<float> currentGainReduction { 0.0f };

        // Helper functions
        void updateTimeConstants();
        static float computeGainReduction (float envelope, float threshold, float ratio);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorSection)
    };
}
