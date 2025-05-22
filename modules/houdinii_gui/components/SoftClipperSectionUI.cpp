#include "SoftClipperSectionUI.h"
#include "WaveShapeVisualizerComponent.h"

namespace houdinii
{
    SoftClipperSectionUI::SoftClipperSectionUI (juce::AudioProcessorValueTreeState& valueTreeState)
        : EffectSectionUI ("Soft Clipper", SoftClipperSection::SOFTCLIP_ENABLED_ID, valueTreeState)
    {
    }

    SoftClipperSectionUI::~SoftClipperSectionUI()
    {
        stopTimer();
    }

    void SoftClipperSectionUI::initialize()
    {
        // Input boost slider
        addAndMakeVisible (inputBoostSlider);
        inputBoostSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        inputBoostSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
        inputBoostSlider.setTextValueSuffix (" dB");

        addAndMakeVisible (inputBoostLabel);
        inputBoostLabel.setText ("Input Boost", juce::dontSendNotification);
        inputBoostLabel.setJustificationType (juce::Justification::centred);
        inputBoostLabel.attachToComponent (&inputBoostSlider, false);

        // Drive slider
        addAndMakeVisible (driveSlider);
        driveSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        driveSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);

        addAndMakeVisible (driveLabel);
        driveLabel.setText ("Drive", juce::dontSendNotification);
        driveLabel.setJustificationType (juce::Justification::centred);
        driveLabel.attachToComponent (&driveSlider, false);

        // Type combo box
        addAndMakeVisible (typeComboBox);
        typeComboBox.addItem ("Tanh", 1);
        typeComboBox.addItem ("Cubic", 2);
        typeComboBox.addItem ("Arctan", 3);

        addAndMakeVisible (typeLabel);
        typeLabel.setText ("Type", juce::dontSendNotification);
        typeLabel.setJustificationType (juce::Justification::centred);
        typeLabel.attachToComponent (&typeComboBox, false);

        // Mix slider
        addAndMakeVisible (mixSlider);
        mixSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        mixSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
        mixSlider.setTextValueSuffix ("%");

        addAndMakeVisible (mixLabel);
        mixLabel.setText ("Mix", juce::dontSendNotification);
        mixLabel.setJustificationType (juce::Justification::centred);
        mixLabel.attachToComponent (&mixSlider, false);

        // Level meter
        addAndMakeVisible (levelMeter);
        addAndMakeVisible (levelLabel);
        levelLabel.setText ("Level", juce::dontSendNotification);
        levelLabel.setJustificationType (juce::Justification::centred);
        levelLabel.attachToComponent (&levelMeter, false);

        // Create parameter attachments
        driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            valueTreeState, SoftClipperSection::SOFTCLIP_DRIVE_ID, driveSlider);
        typeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
            valueTreeState, SoftClipperSection::SOFTCLIP_TYPE_ID, typeComboBox);
        mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            valueTreeState, SoftClipperSection::SOFTCLIP_MIX_ID, mixSlider);
        inputBoostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            valueTreeState, SoftClipperSection::SOFTCLIP_INPUT_BOOST_ID, inputBoostSlider);

        // Custom display for mix as percentage
        mixSlider.textFromValueFunction = [] (double value) {
            return juce::String (static_cast<int> (value * 100)) + "%";
        };

        mixSlider.valueFromTextFunction = [] (const juce::String& text) {
            return text.getDoubleValue() / 100.0;
        };

        // Start timer for updating visualizer
        startTimerHz (30);
    }

    void SoftClipperSectionUI::layoutControls (juce::Rectangle<int> area)
    {
        area.removeFromTop (15); // Space for labels

        // Create wave shape visualizer if we have a reference to the processor
        if (softClipperSection && !waveShaper)
        {
            waveShaper = std::make_unique<WaveShapeVisualizerComponent> (*softClipperSection);
            addAndMakeVisible (*waveShaper);
        }

        // Split into horizontal regions
        auto upperArea = area.removeFromTop (area.getHeight() * 0.6f); // NOLINT(*-narrowing-conversions)

        // Place waveshaper display in the upper area
        if (waveShaper)
        {
            waveShaper->setBounds (upperArea.reduced (5));
        }

        // Calculate control placement in the lower area
        constexpr int numControls = 5; // Input boost, drive, type, mix, level meter
        const int controlWidth = area.getWidth() / numControls;

        // Input boost
        auto boostArea = area.removeFromLeft (controlWidth).reduced (5);
        boostArea.removeFromTop (15); // Space for label
        inputBoostSlider.setBounds (boostArea.withSizeKeepingCentre (60, 60));

        // Drive
        auto driveArea = area.removeFromLeft (controlWidth).reduced (5);
        driveArea.removeFromTop (15); // Space for label
        driveSlider.setBounds (driveArea.withSizeKeepingCentre (60, 60));

        // Type
        auto typeArea = area.removeFromLeft (controlWidth).reduced (5);
        typeArea.removeFromTop (15); // Space for label
        typeComboBox.setBounds (typeArea.removeFromTop (25).reduced (5, 0));

        // Mix
        auto mixArea = area.removeFromLeft (controlWidth).reduced (5);
        mixArea.removeFromTop (15); // Space for label
        mixSlider.setBounds (mixArea.withSizeKeepingCentre (60, 60));

        // Level meter
        auto meterArea = area.reduced (5);
        meterArea.removeFromTop (15); // Space for label
        levelMeter.setBounds (meterArea.withSizeKeepingCentre (20, 60));
    }

    void SoftClipperSectionUI::timerCallback()
    {
        if (softClipperSection)
        {
            levelMeter.setLevel (softClipperSection->getCurrentInputLevel());
        }
    }
}