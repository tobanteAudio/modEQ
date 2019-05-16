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
        setColour(Slider::thumbColourId, Colour(255, 87, 34));
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

        const auto* parent     = box.findParentComponentOfClass<ChoicePropertyComponent>();
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
                                   int standardMenuItemHeight, int& idealWidth,
                                   int& idealHeight) override
    {
        // This was added compared to the JUCE impl. The rest is the same
        standardMenuItemHeight = static_cast<int>(standardMenuItemHeight * 1.25);

        if (isSeparator)
        {
            idealWidth  = 50;
            idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight / 10 : 10;
        }
        else
        {
            auto font = getPopupMenuFont();

            if (standardMenuItemHeight > 0
                && font.getHeight() > standardMenuItemHeight / 1.3f)
                font.setHeight(standardMenuItemHeight / 1.3f);

            idealHeight = standardMenuItemHeight > 0
                              ? standardMenuItemHeight
                              : roundToInt(font.getHeight() * 1.3f);
            idealWidth = font.getStringWidth(text) + idealHeight * 2;
        }
    }

    /**
     * @brief Draws a PopupMenu background.
     */
    void drawPopupMenuBackground(Graphics& g, int width, int height)
    {
        ignoreUnused(width, height);
        g.fillAll(findColour(PopupMenu::backgroundColourId));
    }
};

}  // namespace tobanteAudio