#include "GainSection.h"
namespace houdinii
{
    GainSection::GainSection() = default;

    void GainSection::prepareToPlay (double sampleRate, int samplesPerBlock)
    {
        juce::ignoreUnused (samplesPerBlock);

        smoothedInputGain.reset (sampleRate, 0.05);
        smoothedOutputGain.reset (sampleRate, 0.05);

        smoothedInputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (0.0f));
        smoothedOutputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (0.0f));
    }

    void GainSection::processBlock (juce::AudioBuffer<float>& buffer)
    {
        if (!enabledParam || *enabledParam < 0.5f)
            return; // Bypass if disabled

        const float inputGainDb = inputGainParam ? inputGainParam->load() : 0.0f;
        const float outputGainDb = outputGainParam ? outputGainParam->load() : 0.0f;
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        if (numChannels <= 0 || numSamples <= 0)
            return;

        smoothedInputGain.setTargetValue (juce::Decibels::decibelsToGain (inputGainDb));
        smoothedOutputGain.setTargetValue (juce::Decibels::decibelsToGain (outputGainDb));

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                const float currentInputGain = smoothedInputGain.getNextValue();
                const float currentOutputGain = smoothedOutputGain.getNextValue();

                channelData[sample] *= currentInputGain;
                channelData[sample] *= currentOutputGain;
            }
        }
    }

    void GainSection::addParametersToLayout (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
    {
        layout.add (std::make_unique<juce::AudioParameterBool> (
            juce::ParameterID (GAIN_ENABLED_ID, 1),
            "Gain Stage",
            true));

        layout.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (INPUT_GAIN_ID, 1),
            "Input Gain",
            juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
            0.0f,
            juce::AudioParameterFloatAttributes().withLabel ("dB")));

        layout.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (OUTPUT_GAIN_ID, 1),
            "Output Gain",
            juce::NormalisableRange<float> (-36.0f, 12.0f, 0.1f),
            0.0f,
            juce::AudioParameterFloatAttributes().withLabel ("dB")));
    }

    void GainSection::setParameterPointers (juce::AudioProcessorValueTreeState& valueTreeState)
    {
        enabledParam = valueTreeState.getRawParameterValue (GAIN_ENABLED_ID);
        inputGainParam = valueTreeState.getRawParameterValue (INPUT_GAIN_ID);
        outputGainParam = valueTreeState.getRawParameterValue (OUTPUT_GAIN_ID);
    }
}