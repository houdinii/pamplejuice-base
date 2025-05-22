#include "MeterSection.h"

namespace houdinii
{
    MeterSection::MeterSection() = default;

    void MeterSection::prepareToPlay (double sampleRate, int samplesPerBlock)
    {
        juce::ignoreUnused (samplesPerBlock);

        // Calculate fall-back rate (controls how quickly meters fall)
        // This is approximately -20dB per 1.5 seconds
        meterFallback = static_cast<float> (20.0 / (1.5 * sampleRate));
    }

    void MeterSection::processBlock (juce::AudioBuffer<float>& buffer)
    {
        if (!enabledParam || *enabledParam < 0.5f)
            return;

        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        if (numChannels <= 0 || numSamples <= 0)
            return;

        // Handle mono or stereo
        float leftPeak = 0.0f;
        float rightPeak = 0.0f;

        // Find peak for left channel (channel 0)
        if (numChannels >= 1)
        {
            leftPeak = buffer.getMagnitude (0, 0, numSamples);
        }

        // Find peak for right channel (channel 1)
        if (numChannels >= 2)
        {
            rightPeak = buffer.getMagnitude (1, 0, numSamples);
        }
        else
        {
            // If mono, use left channel value for right
            rightPeak = leftPeak;
        }

        // Decay existing peak values if new values are lower
        float oldLeftPeak = leftPeakLevel.load();
        float oldRightPeak = rightPeakLevel.load();

        float newLeftPeak = juce::jmax (leftPeak, oldLeftPeak - meterFallback * numSamples);
        float newRightPeak = juce::jmax (rightPeak, oldRightPeak - meterFallback * numSamples);

        leftPeakLevel.store (newLeftPeak);
        rightPeakLevel.store (newRightPeak);
    }

    void MeterSection::addParametersToLayout (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
    {
        layout.add (std::make_unique<juce::AudioParameterBool> (
            juce::ParameterID (METER_ENABLED_ID, 1),
            "Level Meter",
            true));
    }

    void MeterSection::setParameterPointers (juce::AudioProcessorValueTreeState& valueTreeState)
    {
        enabledParam = valueTreeState.getRawParameterValue (METER_ENABLED_ID);
    }
}