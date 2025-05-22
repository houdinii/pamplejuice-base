#include "EffectSectionUI.h"
namespace houdinii
{
    EffectSectionUI::EffectSectionUI (const juce::String& name,
        const juce::String& enableParamId,
        juce::AudioProcessorValueTreeState& vts)
        : sectionName (name), valueTreeState (vts)
    {
        addAndMakeVisible (enableButton);
        enableButton.setButtonText ("Enable " + sectionName);

        enableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
            valueTreeState, enableParamId, enableButton);

        // No virtual function calls in constructor
    }

    void EffectSectionUI::paint (juce::Graphics& g)
    {
        auto bounds = getLocalBounds();

        // Draw section background
        g.setColour (juce::Colours::darkgrey.withAlpha (0.6f));
        g.fillRoundedRectangle (bounds.toFloat(), 8.0f); // Slightly smaller radius

        // Draw section title
        g.setColour (juce::Colours::white);
        g.setFont (14.0f); // Smaller font for title
        auto titleArea = bounds.removeFromTop (25); // Reduced from 30
        g.drawText (sectionName, titleArea, juce::Justification::centred, true);
    }

    void EffectSectionUI::resized()
    {
        auto bounds = getLocalBounds().reduced (5); // Reduced from 10
        bounds.removeFromTop (25); // Reduced from 30 for the title

        // Enable button at top
        enableButton.setBounds (bounds.removeFromTop (25).reduced (2)); // Reduced height and padding
        bounds.removeFromTop (3); // Smaller gap

        // Let subclass handle the rest
        layoutControls (bounds);
    }
}