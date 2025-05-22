#pragma once

#include "../../houdinii_core/effects/EffectSection.h"

namespace houdinii
{
    class MeterSection : public EffectSection
    {
    public:
        MeterSection();
        ~MeterSection() override = default;

        void prepareToPlay (double sampleRate, int samplesPerBlock) override;
        void processBlock (juce::AudioBuffer<float>& buffer) override;
        void addParametersToLayout (juce::AudioProcessorValueTreeState::ParameterLayout& layout) override;
        void setParameterPointers (juce::AudioProcessorValueTreeState& valueTreeState) override;

        [[nodiscard]] juce::String getSectionName() const override { return "Level Meter"; }
        [[nodiscard]] juce::String getEnableParameterId() const override { return METER_ENABLED_ID; }

        // Parameter IDs
        static constexpr const char* METER_ENABLED_ID = "meterEnabled";

        // Peak level getters (0.0 to 1.0)
        float getLeftPeakLevel() const { return leftPeakLevel.load(); }
        float getRightPeakLevel() const { return rightPeakLevel.load(); }

    private:
        std::atomic<float>* enabledParam = nullptr;

        // Peak levels
        std::atomic<float> leftPeakLevel { 0.0f };
        std::atomic<float> rightPeakLevel { 0.0f };

        // Fall-back rate
        float meterFallback = 0.0f;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterSection)
    };
}