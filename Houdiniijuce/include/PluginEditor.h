#pragma once

#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"

namespace houdinii
{
    class GainSectionUI;
    class DCBlockerSectionUI;
    class LowPassFilterSectionUI;
    class StereoWidenerSectionUI;
    class MeterSectionUI;
    class CompressorSectionUI;
    class SoftClipperSectionUI;
}
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (houdinii::PluginProcessor&);
    ~PluginEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    houdinii::PluginProcessor& processorRef;

    // UI Inspector
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };

    // Effect section UIs
    std::unique_ptr<houdinii::GainSectionUI> gainSectionUI;
    std::unique_ptr<houdinii::DCBlockerSectionUI> dcBlockerSectionUI;
    std::unique_ptr<houdinii::LowPassFilterSectionUI> lowPassFilterSectionUI;
    std::unique_ptr<houdinii::StereoWidenerSectionUI> stereoWidenerSectionUI;
    std::unique_ptr<houdinii::MeterSectionUI> meterSectionUI;
    std::unique_ptr<houdinii::CompressorSectionUI> compressorSectionUI;
    std::unique_ptr<houdinii::SoftClipperSectionUI> softClipperSectionUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};