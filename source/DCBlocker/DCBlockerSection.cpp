#include "DCBlockerSection.h"

DCBlockerSection::DCBlockerSection()
= default;

void DCBlockerSection::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);

    // We'll resize these when we know how many channels we have
    x1.clear();
    y1.clear();
}

void DCBlockerSection::processBlock(juce::AudioBuffer<float>& buffer)
{
    if (!enabledParam || *enabledParam < 0.5f)
        return; // Bypass if disabled

    const float R = rParam ? rParam->load() : 0.995f;
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();


    if (numChannels <= 0 || numSamples <= 0)
        return;

    // Resize state arrays if needed
    if (static_cast<int> (x1.size()) != numChannels || static_cast<int> (y1.size()) != numChannels)
    {
        x1.resize(numChannels, 0.0f);
        y1.resize(numChannels, 0.0f);
    }

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            const float x_n = channelData[sample];

            // y[n] = x[n] - x[n-1] + R * y[n-1]
            const float y_n = x_n - x1[channel] + R * y1[channel];

            jassert(channel < buffer.getNumChannels());
            jassert(x1.size() >= (size_t)channel);
            jassert(y1.size() >= (size_t)channel);

            channelData[sample] = y_n;

            // Update state
            x1[channel] = x_n;
            y1[channel] = y_n;
        }
    }
}

void DCBlockerSection::addParametersToLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID(DC_BLOCKER_ENABLED_ID, 1),
        "DC Blocker",
        true));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(DC_BLOCKER_R_ID, 1),
        "DC Blocker R",
        juce::NormalisableRange<float>(0.990f, 0.999f, 0.001f),
        0.995f,
        juce::AudioParameterFloatAttributes().withLabel("")));
}

void DCBlockerSection::setParameterPointers(juce::AudioProcessorValueTreeState& valueTreeState)
{
    enabledParam = valueTreeState.getRawParameterValue(DC_BLOCKER_ENABLED_ID);
    rParam = valueTreeState.getRawParameterValue(DC_BLOCKER_R_ID);
}