#pragma once

#include "../../houdinii_effects/distortion/SoftClipperSection.h"
#include "EffectSectionUI.h"
#include "MeterSectionUI.h"

// Forward declaration
class WaveShapeVisualizerComponent;

namespace houdinii
{
    class SoftClipperSectionUI : public EffectSectionUI, private juce::Timer
    {
    public:
        explicit SoftClipperSectionUI (juce::AudioProcessorValueTreeState& valueTreeState);
        ~SoftClipperSectionUI() override;

        void initialize() override;
        SoftClipperSection* getSoftClipperSection() const { return softClipperSection; }
        void setSoftClipperSection (SoftClipperSection* section) { softClipperSection = section; }

    protected:
        void layoutControls (juce::Rectangle<int> area) override;

    private:
        void timerCallback() override;

        SoftClipperSection* softClipperSection = nullptr;

        juce::Slider driveSlider;
        juce::Label driveLabel;

        juce::Slider inputBoostSlider;
        juce::Label inputBoostLabel;

        juce::ComboBox typeComboBox;
        juce::Label typeLabel;

        juce::Slider mixSlider;
        juce::Label mixLabel;

        LevelMeterComponent levelMeter {};
        juce::Label levelLabel;

        std::unique_ptr<WaveShapeVisualizerComponent> waveShaper;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputBoostAttachment;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftClipperSectionUI)
    };
}