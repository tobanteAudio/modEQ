/* Copyright 2018-2020 Tobias Hienzsch
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
                                          const juce::Rectangle<float> bounds) override;

    /**
     * @brief This draws the static background of the whole level meter group with all
     * channels.
     */
    void drawMeterBarsBackground(juce::Graphics& g,
                                 const FFAU::LevelMeter::MeterFlags meterType,
                                 const juce::Rectangle<float> bounds,
                                 const int numChannels,
                                 const int fixedNumChannels) override;

    /**
     * @brief This callback draws the clip indicator. The background has an extra
     * callback.
     */
    void drawClipIndicator(juce::Graphics& g,
                           const FFAU::LevelMeter::MeterFlags meterType,
                           const juce::Rectangle<float> bounds,
                           const bool hasClipped) override;

    /**
     * @brief This callback draws the number of maximum level. The background has an extra
     * callback.
     */
    void drawMaxNumber(juce::Graphics& g, const FFAU::LevelMeter::MeterFlags meterType,
                       const juce::Rectangle<float> bounds, const float maxGain) override;

    /*
     * @brief Override this callback to define the placement of the tickmarks. To disable
     * this feature return an empty rectangle.
     */
    juce::Rectangle<float>
    getMeterTickmarksBounds(const juce::Rectangle<float> bounds,
                            const FFAU::LevelMeter::MeterFlags meterType) const override;

    /*
     * @brief Override this callback to define the placement of the actual meter bar.
     */
    juce::Rectangle<float>
    getMeterBarBounds(const juce::Rectangle<float> bounds,
                      const FFAU::LevelMeter::MeterFlags meterType) const override;
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
    TobanteLookAndFeel();

    Label* createSliderTextBox(Slider& slider) override;

    /**
     * @brief Draws a rotary slider.
     */
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                          float sliderPos, const float rotaryStartAngle,
                          const float rotaryEndAngle, Slider& slider) override;

    /**
     * @brief Draws a ComboBox.
     */
    void drawComboBox(Graphics& g, int width, int height, bool isButtonDown, int buttonX,
                      int buttonY, int buttonW, int buttonH, ComboBox& box) override;

    /**
     * @brief Sets the PopupMenuItem size.
     */
    void getIdealPopupMenuItemSize(const String& text, bool isSeparator,
                                   int standardItemHeight, int& idealWidth,
                                   int& idealHeight) override;

    /**
     * @brief Draws a PopupMenu background.
     */
    void drawPopupMenuBackground(Graphics& g, int width, int height) override;
};

}  // namespace tobanteAudio