#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class EffectSection
{
public:
    EffectSection() = default;
    virtual ~EffectSection() = default;

    // Called once when audio starts
    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) = 0;

    // Process audio buffer
    virtual void processBlock(juce::AudioBuffer<float>& buffer) = 0;

    // Add parameters to the layout
    virtual void addParametersToLayout(std::vector<std::unique_ptr<juce::RangedAudioParameter>>& parameters) = 0;

    // Set parameter pointers after ValueTreeState is created
    virtual void setParameterPointers(juce::AudioProcessorValueTreeState& valueTreeState) = 0;

    // Get the section name
    virtual juce::String getSectionName() const = 0;

    // Get enable parameter ID
    virtual juce::String getEnableParameterId() const = 0;

protected:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectSection)
};