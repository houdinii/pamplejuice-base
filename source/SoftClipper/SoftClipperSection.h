#pragma once

#include "../Common/EffectSection.h"

class SoftClipperSection : public EffectSection
{
public:
    SoftClipperSection();
    ~SoftClipperSection() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;
    void addParametersToLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout) override;
    void setParameterPointers(juce::AudioProcessorValueTreeState& valueTreeState) override;

    [[nodiscard]] juce::String getSectionName() const override { return "Soft Clipper"; }
    [[nodiscard]] juce::String getEnableParameterId() const override { return SOFTCLIP_ENABLED_ID; }

    // Parameter IDs
    static constexpr const char* SOFTCLIP_DRIVE_ID = "softClipDrive";
    static constexpr const char* SOFTCLIP_TYPE_ID = "softClipType";
    static constexpr const char* SOFTCLIP_MIX_ID = "softClipMix";
    static constexpr const char* SOFTCLIP_ENABLED_ID = "softClipEnabled";

    enum ClipType
    {
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

    // Clipping functions
    static inline float processTanh (float input);
    static inline float processCubic (float input);
    static inline float processArctan(float input);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoftClipperSection)
};