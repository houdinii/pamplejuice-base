#include "CompressorSection.h"

CompressorSection::CompressorSection()
= default;

void CompressorSection::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);
    sampleRate = newSampleRate;

    // Reset envelope states
    envelopeState.clear();
    envelopeState.resize(2, 0.0f); // Support up to stereo

    updateTimeConstants();
}

void CompressorSection::updateTimeConstants()
{
    // Convert attack/release times to coefficients
    // Using traditional analog modeling approach
    const float attackMs = attackParam ? attackParam->load() : 10.0f;
    const float releaseMs = releaseParam ? releaseParam->load() : 100.0f;

    // Convert to time constants (RC filter style)
    attackCoeff = std::exp(-1.0f / (attackMs * 0.001f * sampleRate));
    releaseCoeff = std::exp(-1.0f / (releaseMs * 0.001f * sampleRate));
}

float CompressorSection::computeGainReduction(float envelope, float threshold, float ratio)
{
    // Convert to dB
    const float envelopeDb = juce::Decibels::gainToDecibels(envelope);

    // Calculate gain reduction in dB
    if (envelopeDb > threshold)
    {
        const float excess = envelopeDb - threshold;
        const float compressedExcess = excess / ratio;
        return -(excess - compressedExcess); // Negative for gain reduction
    }

    return 0.0f; // No reduction below threshold
}

void CompressorSection::processBlock(juce::AudioBuffer<float>& buffer)
{
    if (!enabledParam || *enabledParam < 0.5f)
        return;

    const float threshold = thresholdParam ? thresholdParam->load() : -20.0f;
    const float ratio = ratioParam ? ratioParam->load() : 4.0f;
    const float makeupGain = makeupParam ? juce::Decibels::decibelsToGain(makeupParam->load()) : 1.0f;

    updateTimeConstants();

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    // Ensure we have enough envelope states
    if (envelopeState.size() < static_cast<size_t>(numChannels))
        envelopeState.resize(numChannels, 0.0f);

    // Process each channel
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        float& envelope = envelopeState[channel];

        for (int sample = 0; sample < numSamples; ++sample)
        {
            const float inputSample = channelData[sample];
            const float inputLevel = std::abs(inputSample);

            // Update envelope (peak detector with attack/release)
            if (inputLevel > envelope)
            {
                // Attack phase
                envelope = inputLevel + attackCoeff * (envelope - inputLevel);
            }
            else
            {
                // Release phase
                envelope = inputLevel + releaseCoeff * (envelope - inputLevel);
            }

            // Compute gain reduction
            const float gainReductionDb = computeGainReduction(envelope, threshold, ratio);
            const float gainReduction = juce::Decibels::decibelsToGain(gainReductionDb);

            // Apply compression and makeup gain
            channelData[sample] = inputSample * gainReduction * makeupGain;

            // Update metering (use maximum reduction across channels)
            if (channel == 0 || gainReductionDb < currentGainReduction.load())
                currentGainReduction.store(gainReductionDb);
        }
    }
}

void CompressorSection::addParametersToLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID(COMP_ENABLED_ID, 1),
        "Compressor",
        true));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(COMP_THRESHOLD_ID, 1),
        "Threshold",
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f),
        -20.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(COMP_RATIO_ID, 1),
        "Ratio",
        juce::NormalisableRange<float>(1.0f, 20.0f, 0.1f, 0.5f), // Skew for better control
        4.0f,
        juce::AudioParameterFloatAttributes().withLabel(":1")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(COMP_ATTACK_ID, 1),
        "Attack",
        juce::NormalisableRange<float>(0.1f, 100.0f, 0.1f, 0.5f), // Log scale
        10.0f,
        juce::AudioParameterFloatAttributes().withLabel("ms")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(COMP_RELEASE_ID, 1),
        "Release",
        juce::NormalisableRange<float>(10.0f, 1000.0f, 1.0f, 0.5f), // Log scale
        100.0f,
        juce::AudioParameterFloatAttributes().withLabel("ms")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(COMP_MAKEUP_ID, 1),
        "Makeup Gain",
        juce::NormalisableRange<float>(0.0f, 24.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));
}

void CompressorSection::setParameterPointers(juce::AudioProcessorValueTreeState& valueTreeState)
{
    enabledParam = valueTreeState.getRawParameterValue(COMP_ENABLED_ID);
    thresholdParam = valueTreeState.getRawParameterValue(COMP_THRESHOLD_ID);
    ratioParam = valueTreeState.getRawParameterValue(COMP_RATIO_ID);
    attackParam = valueTreeState.getRawParameterValue(COMP_ATTACK_ID);
    releaseParam = valueTreeState.getRawParameterValue(COMP_RELEASE_ID);
    makeupParam = valueTreeState.getRawParameterValue(COMP_MAKEUP_ID);
}