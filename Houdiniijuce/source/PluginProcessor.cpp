#include "../include/PluginProcessor.h"
#include "../include/PluginEditor.h"

using namespace houdinii;

//==============================================================================
PluginProcessor::PluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif-
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       valueTreeState(*this, &undoManager, "PARAMETERS", createParameterLayout())
{
    // Create sections AFTER the valueTreeState is constructed
    gainSection = std::make_unique<GainSection>();
    dcBlockerSection = std::make_unique<DCBlockerSection>();
    lowPassFilterSection = std::make_unique<LowPassFilterSection>();
    stereoWidenerSection = std::make_unique<StereoWidenerSection>();
    meterSection = std::make_unique<MeterSection>();
    softClipperSection = std::make_unique<SoftClipperSection>();
    compressorSection = std::make_unique<CompressorSection>();

    // Set parameter pointers
    gainSection->setParameterPointers(valueTreeState);
    dcBlockerSection->setParameterPointers(valueTreeState);
    lowPassFilterSection->setParameterPointers(valueTreeState);
    stereoWidenerSection->setParameterPointers(valueTreeState);
    meterSection->setParameterPointers(valueTreeState);
    softClipperSection->setParameterPointers(valueTreeState);
    compressorSection->setParameterPointers(valueTreeState);

    sectionsInitialized = true;
}

PluginProcessor::~PluginProcessor()
= default;

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    GainSection tempGainSection;
    DCBlockerSection tempDcBlockerSection;
    LowPassFilterSection tempLowPassFilterSection;
    StereoWidenerSection tempStereoWidenerSection;
    MeterSection tempMeterSection;
    SoftClipperSection tempSoftClipperSection;
    CompressorSection tempCompressorSection;

    tempGainSection.addParametersToLayout(layout);
    tempDcBlockerSection.addParametersToLayout(layout);
    tempLowPassFilterSection.addParametersToLayout(layout);
    tempStereoWidenerSection.addParametersToLayout(layout);
    tempMeterSection.addParametersToLayout(layout);
    tempSoftClipperSection.addParametersToLayout(layout);
    tempCompressorSection.addParametersToLayout(layout);
    return layout;
}


//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return "JucePlugin_Name";
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
    if (sectionsInitialized) {
        gainSection->prepareToPlay(sampleRate, samplesPerBlock);
        dcBlockerSection->prepareToPlay(sampleRate, samplesPerBlock);
        lowPassFilterSection->prepareToPlay(sampleRate, samplesPerBlock);
        stereoWidenerSection->prepareToPlay(sampleRate, samplesPerBlock);
        meterSection->prepareToPlay(sampleRate, samplesPerBlock);
        softClipperSection->prepareToPlay(sampleRate, samplesPerBlock);
        compressorSection->prepareToPlay(sampleRate, samplesPerBlock);
    }
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

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Process through each section in order if initialized
    if (sectionsInitialized) {
        gainSection->processBlock(buffer);
        dcBlockerSection->processBlock(buffer);
        lowPassFilterSection->processBlock(buffer);
        softClipperSection->processBlock(buffer);
        compressorSection->processBlock(buffer);
        stereoWidenerSection->processBlock(buffer);
        meterSection->processBlock(buffer);

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

void PluginProcessor::savePresetToFile(const juce::File& presetFile)
{
    juce::MemoryBlock data;
    getStateInformation(data);
    presetFile.replaceWithData(data.getData(), data.getSize());
}

void PluginProcessor::loadPresetFromFile(const juce::File& presetFile)
{
    if (presetFile.existsAsFile())
    {
        juce::MemoryBlock data;
        if (presetFile.loadFileAsData(data))
        {
            setStateInformation(data.getData(), static_cast<int>(data.getSize()));
        }
    }
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}