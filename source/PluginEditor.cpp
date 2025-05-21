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

    // Create section UIs
    gainSectionUI = std::make_unique<GainSectionUI>(processorRef.getValueTreeState());
    dcBlockerSectionUI = std::make_unique<DCBlockerSectionUI>(processorRef.getValueTreeState());

    addAndMakeVisible(*gainSectionUI);
    addAndMakeVisible(*dcBlockerSectionUI);

    setSize(500, 600); // Taller to fit both sections
}

PluginEditor::~PluginEditor() = default;

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("Bass Effect Chain", getLocalBounds().removeFromTop(40),
               juce::Justification::centred, true);
}

void PluginEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(50); // Space for title

    // Inspector button in the bottom corner
    inspectButton.setBounds(bounds.removeFromBottom(30).removeFromRight(120).reduced(5));

    // Stack sections vertically
    bounds.reduce(20, 10);

    gainSectionUI->setBounds(bounds.removeFromTop(200));
    bounds.removeFromTop(10); // Gap between sections
    dcBlockerSectionUI->setBounds(bounds.removeFromTop(150));
}