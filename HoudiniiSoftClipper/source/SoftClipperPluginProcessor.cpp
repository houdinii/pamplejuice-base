#include "../include/SoftClipperPluginProcessor.h"
#include "../include/SoftClipperPluginEditor.h"

SoftClipperPluginProcessor::SoftClipperPluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       valueTreeState(*this, &undoManager, "DISTORT_PARAMETERS", createParameterLayout())
{
}

SoftClipperPluginProcessor::~SoftClipperPluginProcessor() = default;

juce::AudioProcessorValueTreeState::ParameterLayout SoftClipperPluginProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Add a simple gain parameter for now
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("gain", 1),
        "Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    return layout;
}

const juce::String SoftClipperPluginProcessor::getName() const
{
    return "HoudiniiDistort";
}

bool SoftClipperPluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SoftClipperPluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SoftClipperPluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SoftClipperPluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SoftClipperPluginProcessor::getNumPrograms()
{
    return 1;
}

int SoftClipperPluginProcessor::getCurrentProgram()
{
    return 0;
}

void SoftClipperPluginProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String SoftClipperPluginProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void SoftClipperPluginProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void SoftClipperPluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void SoftClipperPluginProcessor::releaseResources()
{
}

bool SoftClipperPluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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

void SoftClipperPluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // For now, pass audio through unchanged
    // We'll add distortion effects later
}

bool SoftClipperPluginProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SoftClipperPluginProcessor::createEditor()
{
    return new SoftClipperPluginEditor(*this);
}

void SoftClipperPluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SoftClipperPluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName(valueTreeState.state.getType()))
            valueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoftClipperPluginProcessor();
}