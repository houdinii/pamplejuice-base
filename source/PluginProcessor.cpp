#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       valueTreeState(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

PluginProcessor::~PluginProcessor()
= default;

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    // Add an on/off switch for the gain stage
    parameters.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID(GAIN_ENABLED_ID, 1),
        "Gain Stage",
        true));  // default to enabled

    // Input gain: -24dB to +24dB, default 0dB (increased range for more pronounced effect)
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(INPUT_GAIN_ID, 1),
        "Input Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")
    ));

    // Output gain: -36dB to +12dB, default 0dB (increased range)
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(OUTPUT_GAIN_ID, 1),
        "Output Gain",
        juce::NormalisableRange<float>(-36.0f, 12.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")
    ));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize smoothed parameters
    // 50 ms ramp time should eliminate clicking without being noticeable
    smoothedInputGain.reset(sampleRate, 0.05);  // 50ms ramp
    smoothedOutputGain.reset(sampleRate, 0.05); // 50ms ramp

    // Set initial values
    smoothedInputGain.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(0.0f));
    smoothedOutputGain.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(0.0f));

    juce::ignoreUnused(samplesPerBlock);
}

void PluginProcessor::releaseResources()
{
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                  juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any extra output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Get parameter values and convert to linear gain
    const float inputGainDb = *valueTreeState.getRawParameterValue(INPUT_GAIN_ID);
    const float outputGainDb = *valueTreeState.getRawParameterValue(OUTPUT_GAIN_ID);

    // Check if the gain stage is enabled
    bool gainEnabled = *valueTreeState.getRawParameterValue(GAIN_ENABLED_ID) > 0.5f;

    // Set target values for smooth parameters
    smoothedInputGain.setTargetValue(gainEnabled ?
        juce::Decibels::decibelsToGain(inputGainDb) :
        1.0f);  // 0dB when disabled

    smoothedOutputGain.setTargetValue(gainEnabled ?
        juce::Decibels::decibelsToGain(outputGainDb) :
        1.0f);  // 0dB when disabled

    // Process each channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Get smoothed gain values for this sample
            const float currentInputGain = smoothedInputGain.getNextValue();
            const float currentOutputGain = smoothedOutputGain.getNextValue();

            // Apply input gain
            channelData[sample] *= currentInputGain;

            // Here's where you'd insert other processing later
            // (e.g., compressor, EQ, distortion, etc.)

            // Apply output gain/trim
            channelData[sample] *= currentOutputGain;
        }
    }
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save parameters to host
    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore parameters from host
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (valueTreeState.state.getType()))
            valueTreeState.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}