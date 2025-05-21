#include "MeterSectionUI.h"

//==============================================================================
// LevelMeterComponent implementation
//==============================================================================

LevelMeterComponent::LevelMeterComponent()
{
    startTimerHz(24); // Update at 24 fps
}

LevelMeterComponent::~LevelMeterComponent()
{
    stopTimer();
}

void LevelMeterComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Draw background
    g.setColour(juce::Colours::black.withAlpha(0.8f));
    g.fillRoundedRectangle(bounds, 3.0f);

    // Draw outline
    g.setColour(juce::Colours::white.withAlpha(0.5f));
    g.drawRoundedRectangle(bounds, 3.0f, 1.0f);

    // Calculate meter height based on level
    float meterHeight = bounds.getHeight() * level;

    // Create the gradient for meter
    juce::ColourGradient gradient(
        juce::Colours::green,
        bounds.getBottomLeft(),
        juce::Colours::red,
        bounds.getTopLeft(),
        false);

    gradient.addColour(0.7, juce::Colours::yellow); // Add mid-color at 70% up

    g.setGradientFill(gradient);

    // Draw meter bar (from bottom to level height)
    auto meterBounds = bounds.reduced(2.0f);
    meterBounds.removeFromTop(bounds.getHeight() - meterHeight);
    g.fillRect(meterBounds);

    // Draw dB markings
    g.setColour(juce::Colours::white);
    g.setFont(10.0f);

    // Draw markers for 0dB, -6dB, -12dB, -18dB
    float db0 = juce::Decibels::decibelsToGain(0.0f);
    float db6 = juce::Decibels::decibelsToGain(-6.0f);
    float db12 = juce::Decibels::decibelsToGain(-12.0f);
    float db18 = juce::Decibels::decibelsToGain(-18.0f);

    float y0 = bounds.getHeight() * (1.0f - db0);
    float y6 = bounds.getHeight() * (1.0f - db6);
    float y12 = bounds.getHeight() * (1.0f - db12);
    float y18 = bounds.getHeight() * (1.0f - db18);

    g.drawText("0", bounds.getRight() - 18, y0 - 5, 16, 10, juce::Justification::left);
    g.drawText("-6", bounds.getRight() - 18, y6 - 5, 16, 10, juce::Justification::left);
    g.drawText("-12", bounds.getRight() - 23, y12 - 5, 22, 10, juce::Justification::left);
    g.drawText("-18", bounds.getRight() - 23, y18 - 5, 22, 10, juce::Justification::left);
}

void LevelMeterComponent::resized()
{
    // Nothing to do here
}

void LevelMeterComponent::setLevel(float newLevel)
{
    level = newLevel;
}

void LevelMeterComponent::timerCallback()
{
    repaint();
}

//==============================================================================
// MeterSectionUI implementation
//==============================================================================

MeterSectionUI::MeterSectionUI(juce::AudioProcessorValueTreeState& valueTreeState, MeterSection& section)
    : EffectSectionUI("Level Meter", MeterSection::METER_ENABLED_ID, valueTreeState),
      meterSection(section)
{
    // No virtual function calls in constructor
}

MeterSectionUI::~MeterSectionUI()
{
    stopTimer();  // Make sure to stop the timer
}


void MeterSectionUI::initialize()
{
    addAndMakeVisible(leftMeter);
    addAndMakeVisible(rightMeter);

    addAndMakeVisible(leftLabel);
    leftLabel.setText("L", juce::dontSendNotification);
    leftLabel.setJustificationType(juce::Justification::centredTop);

    addAndMakeVisible(rightLabel);
    rightLabel.setText("R", juce::dontSendNotification);
    rightLabel.setJustificationType(juce::Justification::centredTop);

    // Start the timer to update meters
    startTimerHz(24);  // 24Hz update rate using JUCE's helper method
}

void MeterSectionUI::layoutControls(juce::Rectangle<int> area)
{
    constexpr int meterWidth = 30;
    constexpr int meterSpacing = 5;
    constexpr int labelHeight = 15;

    area.removeFromTop(10); // Top spacing

    // Calculate centers for meters
    auto centerX = area.getCentreX();

    // Left meter positioning
    auto leftMeterBounds = area.withWidth(meterWidth).withX(centerX - meterWidth - meterSpacing / 2);
    leftMeterBounds.removeFromTop(labelHeight);
    leftMeter.setBounds(leftMeterBounds);
    leftLabel.setBounds(leftMeterBounds.getX(), area.getY(), meterWidth, labelHeight);

    // Right meter positioning
    auto rightMeterBounds = area.withWidth(meterWidth).withX(centerX + meterSpacing / 2);
    rightMeterBounds.removeFromTop(labelHeight);
    rightMeter.setBounds(rightMeterBounds);
    rightLabel.setBounds(rightMeterBounds.getX(), area.getY(), meterWidth, labelHeight);
}

void MeterSectionUI::timerCallback()
{
    // Retrieve meter levels
    leftMeter.setLevel(meterSection.getLeftPeakLevel());
    rightMeter.setLevel(meterSection.getRightPeakLevel());

    // Optional: Convert to dB for display
    // float leftDb = juce::Decibels::gainToDecibels(meterSection.getLeftPeakLevel());
    // float rightDb = juce::Decibels::gainToDecibels(meterSection.getRightPeakLevel());
}