#pragma once

#include "EffectSection.h"

class DCBlockerSection : public EffectSection
{
public:
    DCBlockerSection();
    ~DCBlockerSection() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;
    void addParametersToLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout) override;
    void setParameterPointers(juce::AudioProcessorValueTreeState& valueTreeState) override;

    [[nodiscard]] juce::String getSectionName() const override { return "DC Blocker"; }
    [[nodiscard]] juce::String getEnableParameterId() const override { return DC_BLOCKER_ENABLED_ID; }

    // Parameter IDs
    static constexpr const char* DC_BLOCKER_R_ID = "dcBlockerR";
    static constexpr const char* DC_BLOCKER_ENABLED_ID = "dcBlockerEnabled";

private:
    // Parameter pointers
    std::atomic<float>* rParam = nullptr;
    std::atomic<float>* enabledParam = nullptr;

    // Filter state (separate for each channel)
    std::vector<float> x1; // x[n-1]
    std::vector<float> y1; // y[n-1]

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DCBlockerSection)
};