#include "PluginEditor.h"

#include "houdinii_gui/components/CompressorSectionUI.h"
#include "houdinii_gui/components/DCBlockerSectionUI.h"
#include "houdinii_gui/components/GainSectionUI.h"
#include "houdinii_gui/components/LowPassFilterSectionUI.h"
#include "houdinii_gui/components/MeterSectionUI.h"
#include "houdinii_gui/components/SoftClipperSectionUI.h"
#include "houdinii_gui/components/StereoWidenerSectionUI.h"

using namespace houdinii;

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
    softClipperSectionUI = std::make_unique<SoftClipperSectionUI>(processorRef.getValueTreeState());
    softClipperSectionUI->setSoftClipperSection(dynamic_cast<SoftClipperSection*>(processorRef.getSoftClipperSection()));
    compressorSectionUI = std::make_unique<CompressorSectionUI>(processorRef.getValueTreeState(),
                                                                 *dynamic_cast<CompressorSection*>(processorRef.getCompressorSection()));


    // Initialize after construction
    gainSectionUI->initialize();
    dcBlockerSectionUI->initialize();
    lowPassFilterSectionUI->initialize();
    stereoWidenerSectionUI->initialize();
    meterSectionUI->initialize();
    softClipperSectionUI->initialize();
    compressorSectionUI->initialize();

    addAndMakeVisible(*gainSectionUI);
    addAndMakeVisible(*dcBlockerSectionUI);
    addAndMakeVisible(*lowPassFilterSectionUI);
    addAndMakeVisible(*stereoWidenerSectionUI);
    addAndMakeVisible(*meterSectionUI);
    addAndMakeVisible(*softClipperSectionUI);
    addAndMakeVisible(*compressorSectionUI);

    // Wider format: 3 columns, 2 rows
    setSize(750, 500);
    setResizable(true, true);
    setResizeLimits(600, 400, 1200, 800);
}

PluginEditor::~PluginEditor() = default;

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("Bass Effect Chain", getLocalBounds().removeFromTop(30),
               juce::Justification::centred, true);
}

void PluginEditor::resized()
{
    auto bounds = getLocalBounds();

    // Reserve space for title
    bounds.removeFromTop(35);

    // Inspector button in the bottom corner
    inspectButton.setBounds(bounds.removeFromBottom(25).removeFromRight(100).reduced(2));

    // Main content area with reduced padding
    bounds.reduce(10, 5); // Much tighter padding

    // Calculate dimensions for grid layout
    constexpr int numColumnsRow1 = 3;
    constexpr int numColumnsRow2 = 4;
    const int rowHeight = bounds.getHeight() / 2;
    const int colWidthRow1 = bounds.getWidth() / numColumnsRow1;
    const int colWidthRow2 = bounds.getWidth() / numColumnsRow2;
    constexpr int sectionPadding = 5; // Small gaps between sections

    // First row: Gain, DC Blocker, Low Pass Filter (3 sections)
    auto row1 = bounds.removeFromTop(rowHeight);

    auto gainBounds = row1.removeFromLeft(colWidthRow1).reduced(sectionPadding);
    gainSectionUI->setBounds(gainBounds);

    auto dcBlockerBounds = row1.removeFromLeft(colWidthRow1).reduced(sectionPadding);
    dcBlockerSectionUI->setBounds(dcBlockerBounds);

    auto lowPassBounds = row1.reduced(sectionPadding); // Remaining space
    lowPassFilterSectionUI->setBounds(lowPassBounds);

    // Add some vertical spacing between rows
    bounds.removeFromTop(10);

    // Second row: Stereo Widener, Meter (2 sections)
    auto row2 = bounds.removeFromTop(rowHeight);

    auto stereoWidenerBounds = row2.removeFromLeft(colWidthRow2).reduced(sectionPadding);
    stereoWidenerSectionUI->setBounds(stereoWidenerBounds);

    auto meterBounds = row2.removeFromLeft(colWidthRow2).reduced(sectionPadding);
    meterSectionUI->setBounds(meterBounds);

    auto compressionBounds = row2.removeFromLeft(colWidthRow2).reduced(sectionPadding);
    compressorSectionUI->setBounds(compressionBounds);

    auto softClipperBounds = row2.reduced(sectionPadding);
    softClipperSectionUI->setBounds(softClipperBounds);
}