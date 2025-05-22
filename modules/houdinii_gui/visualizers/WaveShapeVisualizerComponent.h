#pragma once

#include "../../houdinii_effects/distortion/SoftClipperSection.h"
#include "juce_core/system/juce_PlatformDefs.h"
#include <juce_gui_basics/juce_gui_basics.h>

namespace houdinii
{
    class WaveShapeVisualizerComponent : public juce::Component, private juce::Timer
    {
    public:
        explicit WaveShapeVisualizerComponent (SoftClipperSection& section)
            : softClipperSection (section)
        {
            startTimerHz (30); // Update at 30 fps
        }

        ~WaveShapeVisualizerComponent() override
        {
            stopTimer();
        }

        void paint (juce::Graphics& g) override
        {
            auto bounds = getLocalBounds().toFloat().reduced (2.0f);

            // Draw background
            g.setColour (juce::Colours::black.withAlpha (0.8f));
            g.fillRoundedRectangle (bounds, 4.0f);

            // Draw border
            g.setColour (juce::Colours::white.withAlpha (0.3f));
            g.drawRoundedRectangle (bounds, 4.0f, 1.0f);

            // Draw axes
            g.setColour (juce::Colours::grey.withAlpha (0.5f));

            // Horizontal axis (y=0)
            float centerY = bounds.getCentreY();
            g.drawLine (bounds.getX(), centerY, bounds.getRight(), centerY, 0.5f);

            // Vertical axis (x=0)
            float centerX = bounds.getCentreX();
            g.drawLine (centerX, bounds.getY(), centerX, bounds.getBottom(), 0.5f);

            // Get transfer function points
            auto transferPoints = softClipperSection.getTransferFunctionPoints (100);

            // Create path for curve
            juce::Path curve;

            if (!transferPoints.empty())
            {
                const float width = bounds.getWidth();
                const float height = bounds.getHeight();

                // Map first point
                curve.startNewSubPath (
                    bounds.getX() + width * 0.5f * (transferPoints[0] + 1.0f),
                    bounds.getY() + height * 0.5f * (1.0f - transferPoints[0]));

                // Map remaining points
                for (size_t i = 1; i < transferPoints.size(); ++i)
                {
                    float normX = static_cast<float> (i) / static_cast<float> (transferPoints.size() - 1);
                    float mappedX = bounds.getX() + normX * width;
                    float mappedY = bounds.getCentreY() - (transferPoints[i] * height * 0.5f);

                    curve.lineTo (mappedX, mappedY);
                }
            }

            // Draw curve
            g.setColour (juce::Colours::orange);
            g.strokePath (curve, juce::PathStrokeType (2.0f));

            // Draw reference lines representing input level
            float inputLevel = softClipperSection.getCurrentInputLevel();

            if (inputLevel > 0.01f)
            {
                // Map input level to x-position
                float levelX = centerX + (bounds.getWidth() * 0.5f * inputLevel);
                g.setColour (juce::Colours::lightgreen.withAlpha (0.8f));
                g.drawLine (levelX, bounds.getY(), levelX, bounds.getBottom(), 1.0f);

                // Draw line from (-)level to match
                float negativeX = centerX - (bounds.getWidth() * 0.5f * inputLevel);
                g.setColour (juce::Colours::lightgreen.withAlpha (0.4f));
                g.drawLine (negativeX, bounds.getY(), negativeX, bounds.getBottom(), 1.0f);
            }
        }

        void timerCallback() override
        {
            repaint();
        }

    private:
        SoftClipperSection& softClipperSection;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveShapeVisualizerComponent)
    };
}