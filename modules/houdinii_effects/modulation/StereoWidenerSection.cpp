#include "StereoWidenerSection.h"

namespace houdinii
{
    StereoWidenerSection::StereoWidenerSection()
    = default;

    void StereoWidenerSection::prepareToPlay(double newSampleRate, int samplesPerBlock)
    {
        sampleRate = newSampleRate;

        // Calculate max delay in samples (30ms)
        maxDelaySamples = static_cast<int>(0.03 * sampleRate);

        // Initialize delay line
        delayLine.reset();
        delayLine.prepare({ sampleRate, static_cast<uint32_t>(samplesPerBlock), 1 });
        delayLine.setMaximumDelayInSamples(maxDelaySamples);
    }

    void StereoWidenerSection::processBlock(juce::AudioBuffer<float>& buffer)
    {
        if (!enabledParam || *enabledParam < 0.5f)
            return; // Bypass if disabled

        const int numChannels = buffer.getNumChannels();

        // This effect only works in stereo
        if (numChannels < 2)
            return;

        const int numSamples = buffer.getNumSamples();

        // Get width parameter (0.0 - 1.0)
        const float width = widthParam ? widthParam->load() : 0.5f;

        // Calculate delay time in samples (0-30ms based on width)
        const float delaySamples = width * maxDelaySamples;

        // Set delay time
        delayLine.setDelay(delaySamples);

        // Get channel data
        [[maybe_unused]] auto* leftChannel = buffer.getWritePointer(0);
        auto* rightChannel = buffer.getWritePointer(1);

        // Make a copy of the right channel for processing
        juce::HeapBlock<float> rightChannelCopy(numSamples);
        std::memcpy(rightChannelCopy, rightChannel, sizeof(float) * numSamples);

        // Process the right channel through the delay line
        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Push the right channel sample into the delay line
            delayLine.pushSample(0, rightChannelCopy[sample]);

            // Get the delayed sample for the right channel
            rightChannel[sample] = delayLine.popSample(0);
        }
    }

    void StereoWidenerSection::addParametersToLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
    {
        layout.add(std::make_unique<juce::AudioParameterBool>(
            juce::ParameterID(WIDENER_ENABLED_ID, 1),
            "Stereo Widener",
            true));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID(WIDENER_AMOUNT_ID, 1),
            "Width",
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
            0.3f,
            juce::AudioParameterFloatAttributes().withLabel("%")));
    }

    void StereoWidenerSection::setParameterPointers(juce::AudioProcessorValueTreeState& valueTreeState)
    {
        enabledParam = valueTreeState.getRawParameterValue(WIDENER_ENABLED_ID);
        widthParam = valueTreeState.getRawParameterValue(WIDENER_AMOUNT_ID);
    }
}