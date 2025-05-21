#include "CompressorSectionUI.h"

//==============================================================================
// GainReductionMeter implementation
//==============================================================================

GainReductionMeter::GainReductionMeter()
{
    startTimerHz(30);
}

GainReductionMeter::~GainReductionMeter()
{
    stopTimer();
}

void GainReductionMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Draw background
    g.setColour(juce::Colours::black.withAlpha(0.8f));
    g.fillRoundedRectangle(bounds, 2.0f);

    // Draw outline
    g.setColour(juce::Colours::white.withAlpha(0.5f));
    g.drawRoundedRectangle(bounds, 2.0f, 1.0f);

    // Draw gain reduction meter (horizontal, right to left)
    if (displayGainReduction < 0.0f)
    {
        const float meterWidth = bounds.getWidth() * (std::abs(displayGainReduction) / 20.0f); // Max 20dB display

        g.setColour(juce::Colours::orange);
        g.fillRect(bounds.getRight() - meterWidth, bounds.getY() + 2, meterWidth, bounds.getHeight() - 4);
    }

    // Draw scale marks
    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.setFont(9.0f);

    // 0dB mark
    g.drawText("0", bounds.getRight() - 15, bounds.getY(), 12, bounds.getHeight(), juce::Justification::centred);

    // -10dB mark
    float x10 = bounds.getX() + bounds.getWidth() * 0.5f;
    g.drawText("-10", x10 - 12, bounds.getY(), 24, bounds.getHeight(), juce::Justification::centred);

    // -20dB mark
    g.drawText("-20", bounds.getX() + 2, bounds.getY(), 24, bounds.getHeight(), juce::Justification::centred);
}

void GainReductionMeter::setGainReduction(float gainReductionDb)
{
    gainReduction = gainReductionDb;
}

void GainReductionMeter::timerCallback()
{
    // Smooth meter display
    constexpr float smoothingFactor = 0.8f;
    displayGainReduction = smoothingFactor * displayGainReduction + (1.0f - smoothingFactor) * gainReduction;

    // Reset to zero with slow release
    if (gainReduction > displayGainReduction)
        displayGainReduction += 0.5f; // Slow release

    repaint();
}

//==============================================================================
// CompressorSectionUI implementation
//==============================================================================

CompressorSectionUI::CompressorSectionUI(juce::AudioProcessorValueTreeState& valueTreeState,
                                         CompressorSection& compSection)
    : EffectSectionUI("Compressor", CompressorSection::COMP_ENABLED_ID, valueTreeState),
      compressorSection(compSection)
{
}

CompressorSectionUI::~CompressorSectionUI()
{
    stopTimer();
}

void CompressorSectionUI::initialize()
{
    // Threshold
    addAndMakeVisible(thresholdSlider);
    thresholdSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    thresholdSlider.setTextValueSuffix(" dB");

    addAndMakeVisible(thresholdLabel);
    thresholdLabel.setText("Thresh", juce::dontSendNotification);
    thresholdLabel.setJustificationType(juce::Justification::centred);
    thresholdLabel.attachToComponent(&thresholdSlider, false);

    // Ratio
    addAndMakeVisible(ratioSlider);
    ratioSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    ratioSlider.setTextValueSuffix(":1");

    addAndMakeVisible(ratioLabel);
    ratioLabel.setText("Ratio", juce::dontSendNotification);
    ratioLabel.setJustificationType(juce::Justification::centred);
    ratioLabel.attachToComponent(&ratioSlider, false);

    // Attack
    addAndMakeVisible(attackSlider);
    attackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    attackSlider.setTextValueSuffix(" ms");

    addAndMakeVisible(attackLabel);
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centred);
    attackLabel.attachToComponent(&attackSlider, false);

    // Release
    addAndMakeVisible(releaseSlider);
    releaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    releaseSlider.setTextValueSuffix(" ms");

    addAndMakeVisible(releaseLabel);
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centred);
    releaseLabel.attachToComponent(&releaseSlider, false);

    // Makeup
    addAndMakeVisible(makeupSlider);
    makeupSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    makeupSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    makeupSlider.setTextValueSuffix(" dB");

    addAndMakeVisible(makeupLabel);
    makeupLabel.setText("Makeup", juce::dontSendNotification);
    makeupLabel.setJustificationType(juce::Justification::centred);
    makeupLabel.attachToComponent(&makeupSlider, false);

    // Gain reduction meter
    addAndMakeVisible(grMeter);
    addAndMakeVisible(grLabel);
    grLabel.setText("Gain Reduction", juce::dontSendNotification);
    grLabel.setJustificationType(juce::Justification::centred);

    // Create parameter attachments
    thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, CompressorSection::COMP_THRESHOLD_ID, thresholdSlider);
    ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, CompressorSection::COMP_RATIO_ID, ratioSlider);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, CompressorSection::COMP_ATTACK_ID, attackSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, CompressorSection::COMP_RELEASE_ID, releaseSlider);
    makeupAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTreeState, CompressorSection::COMP_MAKEUP_ID, makeupSlider);

    // Start metering timer
    startTimerHz(30);
}

void CompressorSectionUI::layoutControls(juce::Rectangle<int> area)
{
    area.removeFromTop(15); // Space for labels

    // Top row: Threshold, Ratio, Makeup
    auto topRow = area.removeFromTop (area.getHeight() / 2);
    constexpr int knobSize = 50;

    auto threshArea = topRow.removeFromLeft(topRow.getWidth() / 3);
    threshArea.removeFromTop(15);
    thresholdSlider.setBounds(threshArea.withSizeKeepingCentre(knobSize, knobSize));

    auto ratioArea = topRow.removeFromLeft(topRow.getWidth() / 2);
    ratioArea.removeFromTop(15);
    ratioSlider.setBounds(ratioArea.withSizeKeepingCentre(knobSize, knobSize));

    auto makeupArea = topRow;
    makeupArea.removeFromTop(15);
    makeupSlider.setBounds(makeupArea.withSizeKeepingCentre(knobSize, knobSize));

    // Bottom section
    area.removeFromTop(5); // Gap

    // Attack and Release on the left
    auto bottomLeft = area.removeFromLeft(area.getWidth() * 2 / 3);

    auto attackArea = bottomLeft.removeFromLeft(bottomLeft.getWidth() / 2);
    attackArea.removeFromTop(15);
    attackSlider.setBounds(attackArea.withSizeKeepingCentre(knobSize, knobSize));

    auto releaseArea = bottomLeft;
    releaseArea.removeFromTop(15);
    releaseSlider.setBounds(releaseArea.withSizeKeepingCentre(knobSize, knobSize));

    // GR meter on right
    auto meterArea = area.reduced(5);
    grLabel.setBounds(meterArea.removeFromTop(15));
    grMeter.setBounds(meterArea.reduced(0, 5));
}

void CompressorSectionUI::timerCallback()
{
    grMeter.setGainReduction(compressorSection.getCurrentGainReduction());
}