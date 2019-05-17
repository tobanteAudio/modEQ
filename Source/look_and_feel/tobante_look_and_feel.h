/* Copyright 2018-2019 Tobias Hienzsch
 *
 * modEQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * modEQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with modEQ. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// JUCE
#include "../../JuceLibraryCode/JuceHeader.h"

namespace tobanteAudio
{
/**
 * @brief Look and feel (theme) for all level meters.
 *
 * Inherites from ff_meters look & feel.
 */
class TobanteMetersLookAndFeel : public FFAU::LevelMeterLookAndFeel
{
    /**
     * @brief Override this to draw background and if wanted a frame. If the frame takes
     * space away, it should return the reduced bounds.
     */
    juce::Rectangle<float> drawBackground(juce::Graphics& g,
                                          const FFAU::LevelMeter::MeterFlags meterType,
                                          const juce::Rectangle<float> bounds) override
    {
        ignoreUnused(g);
        ignoreUnused(meterType);
        return bounds;
    }

    /**
     * @brief This draws the static background of the whole level meter group with all
     * channels.
     */
    void drawMeterBarsBackground(juce::Graphics& g,
                                 const FFAU::LevelMeter::MeterFlags meterType,
                                 const juce::Rectangle<float> bounds,
                                 const int numChannels,
                                 const int fixedNumChannels) override
    {
        ignoreUnused(g);
        ignoreUnused(meterType);
        ignoreUnused(bounds);
        ignoreUnused(numChannels);
        ignoreUnused(fixedNumChannels);
    }

    /**
     * @brief This callback draws the clip indicator. The background has an extra
     * callback.
     */
    void drawClipIndicator(juce::Graphics& g,
                           const FFAU::LevelMeter::MeterFlags meterType,
                           const juce::Rectangle<float> bounds,
                           const bool hasClipped) override
    {
        ignoreUnused(g);
        ignoreUnused(meterType);
        ignoreUnused(bounds);
        ignoreUnused(hasClipped);
    }

    /**
     * @brief This callback draws the number of maximum level. The background has an extra
     * callback.
     */
    void drawMaxNumber(juce::Graphics& g, const FFAU::LevelMeter::MeterFlags meterType,
                       const juce::Rectangle<float> bounds, const float maxGain) override
    {
        ignoreUnused(g);
        ignoreUnused(meterType);
        ignoreUnused(bounds);
        ignoreUnused(maxGain);
    }

    /*
     * @brief Override this callback to define the placement of the tickmarks. To disable
     * this feature return an empty rectangle.
     */
    juce::Rectangle<float>
    getMeterTickmarksBounds(const juce::Rectangle<float> bounds,
                            const FFAU::LevelMeter::MeterFlags meterType) const override
    {
        ignoreUnused(meterType);
        ignoreUnused(bounds);
        return juce::Rectangle<float>();
    }

    /*
     * @brief Override this callback to define the placement of the actual meter bar.
     */
    juce::Rectangle<float>
    getMeterBarBounds(const juce::Rectangle<float> bounds,
                      const FFAU::LevelMeter::MeterFlags meterType) const override
    {
        ignoreUnused(meterType);
        return bounds;
    }
};

/**
 * @brief Look and feel (theme) for all widgets.
 *
 * Every JUCE widget has a method which can be overwritten to
 * change it's apperance.
 */
class TobanteLookAndFeel : public LookAndFeel_V4
{
public:
    /**
     * @brief Constructor. Sets up the color scheme.
     */
    TobanteLookAndFeel()
    {
        // General
        setColour(ResizableWindow::backgroundColourId, Colour(55, 71, 79));
        // Slider
        setColour(Slider::thumbColourId, tobanteAudio::ORANGE);
    }

    Label* createSliderTextBox(Slider& slider) override
    {
        Label* l = LookAndFeel_V4::createSliderTextBox(slider);
        l->setBorderSize(BorderSize<int>(2, 2, 40, 2));
        l->setColour(Label::outlineColourId, Colours::red);
        l->setColour(Label::outlineWhenEditingColourId, Colours::pink);
        return l;
    }

    /**
     * @brief Draws a rotary slider.
     */
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                          float sliderPos, const float rotaryStartAngle,
                          const float rotaryEndAngle, Slider& slider) override
    {
        auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
        auto fill    = slider.findColour(Slider::rotarySliderFillColourId);

        auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(5);

        auto radius  = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW   = jmin(8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;

        Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius,
                                    arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle,
                                    true);

        const auto label = [&]() -> String {
            if (slider.getName() == String("Frequency"))
            {
                return String(slider.getValue());
            }
            return String::charToString(slider.getName()[0]);
        }();

        const auto stroke
            = PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded);
        g.setColour(outline);
        g.strokePath(backgroundArc, stroke);
        g.setFont(16.0f);
        g.drawText(label, bounds, Justification::centred, true);

        if (slider.isEnabled())
        {
            Path valueArc;
            valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius,
                                   arcRadius, 0.0f, rotaryStartAngle, toAngle, true);

            g.setColour(fill);
            g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved,
                                                  PathStrokeType::rounded));
        }

        auto thumbWidth = lineW * 1.5f;
        Point<float> thumbPoint(
            bounds.getCentreX()
                + arcRadius * std::cos(toAngle - MathConstants<float>::halfPi),
            bounds.getCentreY()
                + arcRadius * std::sin(toAngle - MathConstants<float>::halfPi));

        g.setColour(slider.findColour(Slider::thumbColourId));
        g.fillEllipse(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));
    }

    /**
     * @brief Draws a ComboBox.
     */
    void drawComboBox(Graphics& g, int width, int height, bool isButtonDown, int buttonX,
                      int buttonY, int buttonW, int buttonH, ComboBox& box) override
    {
        ignoreUnused(buttonH);
        ignoreUnused(buttonW);
        ignoreUnused(buttonX);
        ignoreUnused(buttonY);
        ignoreUnused(isButtonDown);

        const auto* parent    = box.findParentComponentOfClass<ChoicePropertyComponent>();
        const auto cornerSize = parent != nullptr ? 0.0f : 1.0f;
        const auto boxBounds  = Rectangle<int>(0, 0, width, height);

        // Background
        g.setColour(box.findColour(ComboBox::backgroundColourId));
        g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

        // Arrow
        Path path;
        const auto arrow = Rectangle<int>(width - 30, 0, 20, height);

        path.startNewSubPath(arrow.getX() + 3.0f, arrow.getCentreY() - 2.0f);
        path.lineTo(static_cast<float>(arrow.getCentreX()), arrow.getCentreY() + 3.0f);
        path.lineTo(arrow.getRight() - 3.0f, arrow.getCentreY() - 2.0f);

        const auto alpha = box.isEnabled() ? 0.9f : 0.2f;
        g.setColour(box.findColour(ComboBox::arrowColourId).withAlpha(alpha));
        g.strokePath(path, PathStrokeType(2.0f));
    }

    /**
     * @brief Sets the PopupMenuItem size.
     */
    void getIdealPopupMenuItemSize(const String& text, bool isSeparator,
                                   int standardItemHeight, int& idealWidth,
                                   int& idealHeight) override
    {
        // This was added compared to the JUCE impl. The rest is the same
        standardItemHeight = static_cast<int>(standardItemHeight * 1.25);

        if (isSeparator)
        {
            idealWidth  = 50;
            idealHeight = standardItemHeight > 0 ? standardItemHeight / 10 : 10;
        }
        else
        {
            auto font = getPopupMenuFont();

            if (standardItemHeight > 0 && font.getHeight() > standardItemHeight / 1.3f)
            {
                font.setHeight(standardItemHeight / 1.3f);
            }

            idealHeight = standardItemHeight > 0 ? standardItemHeight
                                                 : roundToInt(font.getHeight() * 1.3f);
            idealWidth = font.getStringWidth(text) + idealHeight * 2;
        }
    }

    /**
     * @brief Draws a PopupMenu background.
     */
    void drawPopupMenuBackground(Graphics& g, int width, int height) override
    {
        ignoreUnused(width, height);
        g.fillAll(findColour(PopupMenu::backgroundColourId));
    }
};

}  // namespace tobanteAudio