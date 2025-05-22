#pragma once

#include "../../houdinii_core/effects/EffectSection.h"
#include <vector>

class WaveShaper;

namespace houdinii
{
    class SoftClipperSection : public EffectSection
    {
    public:
        SoftClipperSection();
        ~SoftClipperSection() override = default;

        void prepareToPlay (double sampleRate, int samplesPerBlock) override;
        void processBlock (juce::AudioBuffer<float>& buffer) override;
        void addParametersToLayout (juce::AudioProcessorValueTreeState::ParameterLayout& layout) override;
        void setParameterPointers (juce::AudioProcessorValueTreeState& valueTreeState) override;

        [[nodiscard]] juce::String getSectionName() const override { return "Soft Clipper"; }
        [[nodiscard]] juce::String getEnableParameterId() const override { return SOFTCLIP_ENABLED_ID; }

        // Parameter IDs
        static constexpr const char* SOFTCLIP_DRIVE_ID = "softClipDrive";
        static constexpr const char* SOFTCLIP_TYPE_ID = "softClipType";
        static constexpr const char* SOFTCLIP_MIX_ID = "softClipMix";
        static constexpr const char* SOFTCLIP_ENABLED_ID = "softClipEnabled";
        static constexpr const char* SOFTCLIP_INPUT_BOOST_ID = "softClipBoost";

        // Get current input level for metering (0.0 to 1.0)
        float getCurrentInputLevel() const { return inputLevel.load(); }

        // Get transfer function points for visualization
        std::vector<float> getTransferFunctionPoints (int numPoints) const;

        enum ClipType {
            Tanh = 0,
            Cubic,
            Arctan
        };

    private:
        // Parameter pointers
        std::atomic<float>* driveParam = nullptr;
        std::atomic<float>* typeParam = nullptr;
        std::atomic<float>* mixParam = nullptr;
        std::atomic<float>* enabledParam = nullptr;
        std::atomic<float>* inputBoostParam = nullptr;

        // For level metering
        std::atomic<float> inputLevel { 0.0f };
        double sampleRate = 44100.0;
        float meterFallback = 0.0f;

        // Clipping functions
        float processTanh (float input) const;
        float processCubic (float input) const;
        float processArctan (float input) const;

        // Apply the currently selected waveshaper
        float applyWaveshaper (float input, int type) const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftClipperSection)
    };
}