#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // UI Inspector setup
    addAndMakeVisible (inspectButton);
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }
        inspector->setVisible (true);
    };

    // Gain stage enable button
    addAndMakeVisible(gainEnabledButton);
    gainEnabledButton.setButtonText("Enable Gain Stage");
    gainEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.getValueTreeState(), PluginProcessor::GAIN_ENABLED_ID, gainEnabledButton);

    // Input gain slider setup
    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    inputGainSlider.setTextValueSuffix(" dB");

    addAndMakeVisible(inputGainLabel);
    inputGainLabel.setText("Input Gain", juce::dontSendNotification);
    inputGainLabel.setJustificationType(juce::Justification::centred);
    inputGainLabel.attachToComponent(&inputGainSlider, false);

    // Output gain slider setup
    addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    outputGainSlider.setTextValueSuffix(" dB");

    addAndMakeVisible(outputGainLabel);
    outputGainLabel.setText("Output Gain", juce::dontSendNotification);
    outputGainLabel.setJustificationType(juce::Justification::centred);
    outputGainLabel.attachToComponent(&outputGainSlider, false);

    // Create parameter attachments using the public constants
    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getValueTreeState(), PluginProcessor::INPUT_GAIN_ID, inputGainSlider);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.getValueTreeState(), PluginProcessor::OUTPUT_GAIN_ID, outputGainSlider);

    setSize (500, 400);
}

PluginEditor::~PluginEditor()
= default;


void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("Bass Gain Stager", getLocalBounds().removeFromTop(40),
               juce::Justification::centred, true);

    // Draw a section frame around the gain stage controls
    auto gainSectionBounds = getLocalBounds().reduced(20);
    gainSectionBounds.removeFromTop(60); // Leave room for the title
    gainSectionBounds.setHeight(200);    // Fixed height for the gain section

    g.setColour(juce::Colours::darkgrey.withAlpha(0.6f));
    g.fillRoundedRectangle(gainSectionBounds.toFloat(), 10.0f);

    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    g.drawText("Gain Stage", gainSectionBounds.removeFromTop(30),
               juce::Justification::centred, true);
}

void PluginEditor::resized()
{
    // Allow space for title
    auto bounds = getLocalBounds();
    bounds.removeFromTop(50);

    // Inspector button in the bottom corner
    inspectButton.setBounds(bounds.removeFromBottom(30).removeFromRight(120).reduced(5));

    // Gain section
    auto gainSection = bounds.reduced(25);
    gainSection.setHeight(200);

    // Enable button at the top of the section
    gainEnabledButton.setBounds(gainSection.removeFromTop(30));

    // Split the remaining area for the two knobs
    auto knobArea = gainSection.reduced(10);
    auto knobWidth = knobArea.getWidth() / 2;

    auto inputArea = knobArea.removeFromLeft(knobWidth).reduced(10);
    auto outputArea = knobArea.reduced(10);

    // Leave space for labels
    inputArea.removeFromTop(20);
    outputArea.removeFromTop(20);

    inputGainSlider.setBounds(inputArea);
    outputGainSlider.setBounds(outputArea);
}

// Helper method for future effect sections
void PluginEditor::createEffectSection(const juce::String& title,
                                       juce::Component& container,
                                       juce::ToggleButton& enableButton)
{
    // For future use when we add more sections,
    // This will be a pattern for creating standardized section containers
    juce::ignoreUnused(title, container, enableButton);
}