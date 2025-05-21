#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

class EffectSectionUI : public juce::Component
{
public:
    EffectSectionUI(const juce::String& sectionName,
                    const juce::String& enableParamId,
                    juce::AudioProcessorValueTreeState& valueTreeState);
    ~EffectSectionUI() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;
    virtual void addControls() = 0;

protected:
    // Subclasses override this to add their specific controls
    virtual void layoutControls(juce::Rectangle<int> area) = 0;

    juce::String sectionName;
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::ToggleButton enableButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enableAttachment;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectSectionUI)
};