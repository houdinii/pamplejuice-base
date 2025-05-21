#pragma once

#include "EffectSection.h"

class GainSection : public EffectSection
{
public:
    GainSection();
    ~GainSection() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;
    void addParametersToLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout) override;
    void setParameterPointers(juce::AudioProcessorValueTreeState& valueTreeState) override;

    [[nodiscard]] juce::String getSectionName() const override { return "Gain Stage"; }
    [[nodiscard]] juce::String getEnableParameterId() const override { return GAIN_ENABLED_ID; }

    // Parameter IDs
    static constexpr const char* INPUT_GAIN_ID = "inputGain";
    static constexpr const char* OUTPUT_GAIN_ID = "outputGain";
    static constexpr const char* GAIN_ENABLED_ID = "gainEnabled";

private:
    // Parameter pointers
    std::atomic<float>* inputGainParam = nullptr;
    std::atomic<float>* outputGainParam = nullptr;
    std::atomic<float>* enabledParam = nullptr;

    // Smoothed parameters
    juce::SmoothedValue<float> smoothedInputGain;
    juce::SmoothedValue<float> smoothedOutputGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainSection)
};