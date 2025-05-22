/*
==============================================================================

    EffectSectionUI.h
    Part of the houdinii_gui module

  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace houdinii
{
    class EffectSectionUI : public juce::Component
    {
    public:
        EffectSectionUI (const juce::String& name,
            const juce::String& enableParamId,
            juce::AudioProcessorValueTreeState& vts);
        ~EffectSectionUI() override = default;

        // Initialize should be called after construction
        virtual void initialize() = 0;

        void paint (juce::Graphics& g) override;
        void resized() override;

    protected:
        // Subclasses override this to add their specific controls
        virtual void layoutControls (juce::Rectangle<int> area) = 0;

        juce::String sectionName;
        juce::AudioProcessorValueTreeState& valueTreeState;

        juce::ToggleButton enableButton;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enableAttachment;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectSectionUI)
    };

}