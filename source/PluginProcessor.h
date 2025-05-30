#pragma once

#include "Meter/MeterSection.h"
#include "GainSection/GainSection.h"
#include "DCBlocker/DCBlockerSection.h"
#include "SoftClipper/SoftClipperSection.h"
#include "Compressor/CompressorSection.h"
#include "LowPassFilter/LowPassFilterSection.h"
#include "StereoWidener/StereoWidenerSection.h"

#include <juce_audio_processors/juce_audio_processors.h>

#if (MSVC)
    #include "ipps.h"
#endif

class PluginProcessor final : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    EffectSection* getMeterSection() const { return meterSection.get(); }
    EffectSection* getCompressorSection() const { return compressorSection.get(); }
    EffectSection* getSoftClipperSection() const { return softClipperSection.get(); }


    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Add this for parameter access
    juce::AudioProcessorValueTreeState& getValueTreeState() { return valueTreeState; }

    // Parameter IDs - making these public so the editor can access them
    static constexpr const char* INPUT_GAIN_ID = "inputGain";
    static constexpr const char* OUTPUT_GAIN_ID = "outputGain";
    static constexpr const char* GAIN_ENABLED_ID = "gainEnabled";

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState valueTreeState;

    // Effect sections
    std::unique_ptr<GainSection> gainSection;
    std::unique_ptr<DCBlockerSection> dcBlockerSection;
    std::unique_ptr<LowPassFilterSection> lowPassFilterSection;
    std::unique_ptr<StereoWidenerSection> stereoWidenerSection;
    std::unique_ptr<MeterSection> meterSection;
    std::unique_ptr<SoftClipperSection> softClipperSection;
    std::unique_ptr<CompressorSection> compressorSection;

    // Flag to track if we've initialized sections
    bool sectionsInitialized = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};