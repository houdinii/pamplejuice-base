#pragma once

#include "../Common/EffectSection.h"
#include <juce_dsp/juce_dsp.h>

class StereoWidenerSection : public EffectSection
{
public:
    StereoWidenerSection();
    ~StereoWidenerSection() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;
    void addParametersToLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout) override;
    void setParameterPointers(juce::AudioProcessorValueTreeState& valueTreeState) override;

    [[nodiscard]] juce::String getSectionName() const override { return "Stereo Widener"; }
    [[nodiscard]] juce::String getEnableParameterId() const override { return WIDENER_ENABLED_ID; }

    // Parameter IDs
    static constexpr const char* WIDENER_AMOUNT_ID = "widenerAmount";
    static constexpr const char* WIDENER_ENABLED_ID = "widenerEnabled";

private:
    // Parameter pointers
    std::atomic<float>* widthParam = nullptr;
    std::atomic<float>* enabledParam = nullptr;

    // Delay line for Haas effect
    juce::dsp::DelayLine<float> delayLine;

    double sampleRate = 44100.0;
    int maxDelaySamples = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoWidenerSection)
};