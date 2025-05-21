#pragma once

#include "../Common/EffectSectionUI.h"
#include "MeterSection.h"

class LevelMeterComponent : public juce::Component, private juce::Timer
{
public:
    LevelMeterComponent();
    ~LevelMeterComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Set the level to display (0.0 to 1.0)
    void setLevel(float level);

private:
    void timerCallback() override;
    float level = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterComponent)
};

class MeterSectionUI : public EffectSectionUI, private juce::Timer
{
public:
    MeterSectionUI(juce::AudioProcessorValueTreeState& valueTreeState, MeterSection& meterSection);
    ~MeterSectionUI() override;  // Add destructor to stop the timer

    void initialize() override;

protected:
    void layoutControls(juce::Rectangle<int> area) override;

private:
    MeterSection& meterSection;
    LevelMeterComponent leftMeter;
    LevelMeterComponent rightMeter;
    juce::Label leftLabel;
    juce::Label rightLabel;

    void timerCallback() override;  // Override to properly implement Timer

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MeterSectionUI)
};