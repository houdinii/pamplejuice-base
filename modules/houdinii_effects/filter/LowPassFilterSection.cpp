#include "LowPassFilterSection.h"

namespace houdinii
{
    LowPassFilterSection::LowPassFilterSection() = default;

    void LowPassFilterSection::prepareToPlay (double newSampleRate, int samplesPerBlock)
    {
        sampleRate = newSampleRate;

        // Set up the filter
        juce::dsp::ProcessSpec spec{};
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = 2; // Assuming stereo

        lowPassFilter.prepare (spec);

        // Set up parameter smoothing
        smoothedCutoff.reset (sampleRate, 0.05);

        // Initialize with the default values
        float defaultCutoff = cutoffParam ? cutoffParam->load() : 20000.0f;
        smoothedCutoff.setCurrentAndTargetValue (defaultCutoff);
    }

    void LowPassFilterSection::processBlock (juce::AudioBuffer<float>& buffer)
    {
        if (!enabledParam || *enabledParam < 0.5f)
            return; // Bypass if disabled

        const float cutoff = cutoffParam ? cutoffParam->load() : 20000.0f;
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        if (numChannels <= 0 || numSamples <= 0)
            return;

        smoothedCutoff.setTargetValue (cutoff);

        // Create an audio block to wrap the buffer
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> context (block);

        // Update filter coefficients for each block
        *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, smoothedCutoff.getNextValue());

        // Process the filter
        lowPassFilter.process (context);
    }

    void LowPassFilterSection::addParametersToLayout (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
    {
        layout.add (std::make_unique<juce::AudioParameterBool> (
            juce::ParameterID (LOWPASS_ENABLED_ID, 1),
            "Low Pass Filter",
            true));

        layout.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (LOWPASS_CUTOFF_ID, 1),
            "Cutoff",
            juce::NormalisableRange<float> (20.0f, 20000.0f, 1.0f, 0.2f), // Logarithmic mapping with the skew factor
            20000.0f,
            juce::AudioParameterFloatAttributes().withLabel ("Hz")));
    }

    void LowPassFilterSection::setParameterPointers (juce::AudioProcessorValueTreeState& valueTreeState)
    {
        enabledParam = valueTreeState.getRawParameterValue (LOWPASS_ENABLED_ID);
        cutoffParam = valueTreeState.getRawParameterValue (LOWPASS_CUTOFF_ID);
    }
}