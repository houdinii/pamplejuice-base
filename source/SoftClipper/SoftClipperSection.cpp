#include "SoftClipperSection.h"
#include <cmath>

SoftClipperSection::SoftClipperSection()
= default;

void SoftClipperSection::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

inline float SoftClipperSection::processTanh(float input)
{
    // tanh() naturally saturates at ±1
    // We scale it slightly to preserve more headroom
    return std::tanh(input * 0.7f) * 1.42857f;  // 1/0.7 = 1.42857
}

inline float SoftClipperSection::processCubic(float input)
{
    // Cubic soft clipping: y = x - (x³/3)
    // Only valid for |x| <= sqrt(3), saturates beyond
    const float absInput = std::abs(input);
    const float sign = (input < 0.0f) ? -1.0f : 1.0f;

    if (absInput < 1.732f)  // sqrt(3)
    {
        return sign * (absInput - (absInput * absInput * absInput) / 3.0f);
    }
    return sign * 1.1547f;  // 2/sqrt(3) - maximum output
}

inline float SoftClipperSection::processArctan(float input)
{
    // atan() soft clipping, scaled to roughly match unity gain
    return (2.0f / juce::MathConstants<float>::pi) * std::atan(input * juce::MathConstants<float>::pi * 0.5f);
}

void SoftClipperSection::processBlock(juce::AudioBuffer<float>& buffer)
{
    if (!enabledParam || *enabledParam < 0.5f)
        return;

    const float drive = driveParam ? driveParam->load() : 1.0f;
    const int clipType = typeParam ? static_cast<int>(typeParam->load()) : 0;
    const float mix = mixParam ? mixParam->load() : 1.0f;

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float input = channelData[sample];
            const float driven = input * drive;

            float clipped = 0.0f;
            switch (clipType)
            {
                case Tanh:
                    clipped = processTanh(driven);
                    break;
                case Cubic:
                    clipped = processCubic(driven);
                    break;
                case Arctan:
                    clipped = processArctan(driven);
                    break;
                default:
                    clipped = processTanh(driven);
                    break;
            }

            // Apply makeup gain to compensate for level loss
            const float makeupGain = 1.0f / std::max(0.1f, drive);
            clipped *= makeupGain;

            // Mix dry and wet signals
            channelData[sample] = (input * (1.0f - mix)) + (clipped * mix);
        }
    }
}

void SoftClipperSection::addParametersToLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID(SOFTCLIP_ENABLED_ID, 1),
        "Soft Clipper",
        true));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(SOFTCLIP_DRIVE_ID, 1),
        "Drive",
        juce::NormalisableRange<float>(1.0f, 10.0f, 0.01f, 0.5f), // Skew for more control at lower values
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
}