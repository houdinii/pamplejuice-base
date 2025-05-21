#include "SoftClipperSection.h"
#include <cmath>

SoftClipperSection::SoftClipperSection()
= default;

void SoftClipperSection::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);
    sampleRate = newSampleRate;

    // Calculate fall-back rate for meter
    meterFallback = static_cast<float>(20.0 / (1.0 * sampleRate));
}

float SoftClipperSection::processTanh(float input) const
{
    return std::tanh(input);
}

float SoftClipperSection::processCubic(float input) const
{
    // Cubic soft clipping: y = x - (x³/3)
    const float absInput = std::abs(input);
    const float sign = input < 0.0f ? -1.0f : 1.0f;

    if (absInput < 1.732f)  // sqrt(3)
    {
        return sign * (absInput - std::pow(absInput, 3) / 3.0f);  // NOLINT(*-narrowing-conversions)
    }
    return sign * 2.0f / 3.0f;  // Maximum output
}

float SoftClipperSection::processArctan(float input) const
{
    // atan() soft clipping, scaled to match unity gain at low levels
    return (2.0f / juce::MathConstants<float>::pi) * std::atan(input);
}

float SoftClipperSection::applyWaveshaper(float input, int type) const
{
    switch (type) {
        case Tanh:
            return processTanh(input);
        case Cubic:
            return processCubic(input);
        case Arctan:
            return processArctan(input);
        default:
            return processTanh(input);
    }
}

void SoftClipperSection::processBlock(juce::AudioBuffer<float>& buffer)
{
    if (!enabledParam || *enabledParam < 0.5f)
        return;

    const float drive = driveParam ? driveParam->load() : 1.0f;
    const int clipType = typeParam ? static_cast<int>(typeParam->load()) : 0;
    const float mix = mixParam ? mixParam->load() : 1.0f;
    const float inputBoost = inputBoostParam ? juce::Decibels::decibelsToGain(inputBoostParam->load()) : 1.0f;

    float maxLevel = 0.0f;

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float input = channelData[sample];

            // Apply input boost (pre-amplification)
            const float boostedInput = input * inputBoost;

            // Track maximum level for metering
            maxLevel = std::max(maxLevel, std::abs(boostedInput));

            // Apply drive to the boosted signal
            const float driven = boostedInput * drive;

            // Apply selected waveshaper
            const float clipped = applyWaveshaper(driven, clipType);

            // Apply makeup gain to compensate for level loss
            const float makeupGain = 1.0f / std::max(0.1f, drive);
            const float processed = clipped * makeupGain;

            // Mix dry and wet signals
            channelData[sample] = (input * (1.0f - mix)) + (processed * mix);
        }
    }

    // Update metering with fallback
    float oldLevel = inputLevel.load();
    float newLevel = std::max(maxLevel, oldLevel - (meterFallback * buffer.getNumSamples()));  // NOLINT(*-narrowing-conversions)
    inputLevel.store(newLevel);
}

void SoftClipperSection::addParametersToLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID(SOFTCLIP_ENABLED_ID, 1),
        "Soft Clipper",
        true));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(SOFTCLIP_INPUT_BOOST_ID, 1),
        "Input Boost",
        juce::NormalisableRange<float>(0.0f, 36.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(SOFTCLIP_DRIVE_ID, 1),
        "Drive",
        juce::NormalisableRange<float>(1.0f, 10.0f, 0.01f, 0.5f),
        1.0f,
        juce::AudioParameterFloatAttributes().withLabel("x")));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID(SOFTCLIP_TYPE_ID, 1),
        "Type",
        juce::StringArray("Tanh", "Cubic", "Arctan"),
        0));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(SOFTCLIP_MIX_ID, 1),
        "Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        1.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
}

void SoftClipperSection::setParameterPointers(juce::AudioProcessorValueTreeState& valueTreeState)
{
    enabledParam = valueTreeState.getRawParameterValue(SOFTCLIP_ENABLED_ID);
    driveParam = valueTreeState.getRawParameterValue(SOFTCLIP_DRIVE_ID);
    typeParam = valueTreeState.getRawParameterValue(SOFTCLIP_TYPE_ID);
    mixParam = valueTreeState.getRawParameterValue(SOFTCLIP_MIX_ID);
    inputBoostParam = valueTreeState.getRawParameterValue(SOFTCLIP_INPUT_BOOST_ID);
}

std::vector<float> SoftClipperSection::getTransferFunctionPoints(int numPoints) const
{
    std::vector<float> points;
    points.reserve(numPoints);

    const float drive = driveParam ? driveParam->load() : 1.0f;
    const int clipType = typeParam ? static_cast<int>(typeParam->load()) : 0;
    const float makeupGain = 1.0f / std::max(0.1f, drive);

    for (int i = 0; i < numPoints; ++i)
    {
        // Generate input points from -1.0 to 1.0
        float inputValue = (i / static_cast<float>(numPoints - 1)) * 2.0f - 1.0f; // NOLINT(*-narrowing-conversions)

        // Apply drive
        float driven = inputValue * drive;

        // Apply selected waveshaper
        float clipped = applyWaveshaper(driven, clipType);

        // Apply makeup gain
        points.push_back(clipped * makeupGain);
    }

    return points;
}