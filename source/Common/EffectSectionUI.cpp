#include "EffectSectionUI.h"

EffectSectionUI::EffectSectionUI(const juce::String& name,
                                 const juce::String& enableParamId,
                                 juce::AudioProcessorValueTreeState& vts)
    : sectionName(name), valueTreeState(vts)
{
    addAndMakeVisible(enableButton);
    enableButton.setButtonText("Enable " + sectionName);

    enableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        valueTreeState, enableParamId, enableButton);

    // No virtual function calls in constructor
}

void EffectSectionUI::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Draw section background
    g.setColour(juce::Colours::darkgrey.withAlpha(0.6f));
    g.fillRoundedRectangle(bounds.toFloat(), 10.0f);

    // Draw section title
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    auto titleArea = bounds.removeFromTop(30);
    g.drawText(sectionName, titleArea, juce::Justification::centred, true);
}

void EffectSectionUI::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    bounds.removeFromTop(30); // Space for title

    // Enable button at top
    enableButton.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(5); // Small gap

    // Let subclass handle the rest
    layoutControls(bounds);
}