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
    lowPassFilterSectionUI = std::make_unique<LowPassFilterSectionUI>(processorRef.getValueTreeState());
    stereoWidenerSectionUI = std::make_unique<StereoWidenerSectionUI>(processorRef.getValueTreeState());
    meterSectionUI = std::make_unique<MeterSectionUI>(processorRef.getValueTreeState(),
                                                     *dynamic_cast<MeterSection*>(processorRef.getMeterSection()));

    // Initialize after construction
    gainSectionUI->initialize();
    dcBlockerSectionUI->initialize();
    lowPassFilterSectionUI->initialize();
    stereoWidenerSectionUI->initialize();
    meterSectionUI->initialize();

    addAndMakeVisible(*gainSectionUI);
    addAndMakeVisible(*dcBlockerSectionUI);
    addAndMakeVisible(*lowPassFilterSectionUI);
    addAndMakeVisible(*stereoWidenerSectionUI);
    addAndMakeVisible(*meterSectionUI);

    setSize(500, 900); // Taller to fit both sections
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

    // Increase the DCBlocker section height
    dcBlockerSectionUI->setBounds(bounds.removeFromTop(200)); // Changed from 150 to 200

    // Add other sections here
    bounds.removeFromTop(10); // Gap between sections
    lowPassFilterSectionUI->setBounds(bounds.removeFromTop(200));

    bounds.removeFromTop(10); // Gap between sections
    stereoWidenerSectionUI->setBounds(bounds.removeFromTop(200));

    bounds.removeFromTop(10); // Gap between sections
    meterSectionUI->setBounds(bounds.removeFromTop(200));
}