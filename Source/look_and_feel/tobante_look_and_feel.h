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
class TobanteLookAndFeel : public LookAndFeel_V4
{
public:
    TobanteLookAndFeel()
    {
        // Slider (excluding Rotary)
        setColour(Slider::thumbColourId, Colours::blue);
        setColour(Slider::backgroundColourId, Colours::aliceblue);
        setColour(Slider::trackColourId, Colours::grey);

        // ComboBox
        setColour(ComboBox::backgroundColourId, Colours::aliceblue);
        setColour(ComboBox::textColourId, Colours::black);
        setColour(ComboBox::arrowColourId, Colours::black);
        setColour(ComboBox::buttonColourId, Colours::blue);

        // PopupMenu (in ComboxBox)
        setColour(PopupMenu::backgroundColourId, Colours::aliceblue);
        setColour(PopupMenu::highlightedBackgroundColourId, Colours::whitesmoke);
        setColour(PopupMenu::textColourId, Colours::black);
        setColour(PopupMenu::highlightedTextColourId, Colours::blue);
    }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle,
                          Slider& slider) override
    {
        ignoreUnused(slider);

        auto radius  = jmin(width / 2, height / 2) - 4.0f;
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx      = centreX - radius;
        auto ry      = centreY - radius;
        auto rw      = radius * 2.0f;
        auto angle   = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        g.setColour(Colours::aliceblue);
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(Colours::darkblue);
        g.drawEllipse(rx, ry, rw, rw, 2.0f);

        Rectangle<int> textArea {50, 50};
        textArea.setCentre(static_cast<int>(centreX), static_cast<int>(centreY));

        g.setFont(18.f);
        g.drawText(slider.getName(), textArea, Justification::centred);

        Path p;
        auto pointerLength    = radius * 0.33f;
        auto pointerThickness = 8.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

        // pointer
        g.setColour(Colours::black);
        g.fillPath(p);
    }

    // layout slider and value label
    Slider::SliderLayout getSliderLayout(Slider& slider) override
    {
        Slider::SliderLayout layout;

        Rectangle<int> area = slider.getLocalBounds();
        // area.removeFromBottom(static_cast<int>(area.getHeight() / 8 * 1)); //
        // empty space on bottom

        layout.textBoxBounds = area.removeFromBottom(static_cast<int>(area.getHeight() / 8 * 1));
        layout.sliderBounds  = area;

        return layout;
    }
};

}  // namespace tobanteAudio