/* Copyright 2018 Tobias Hienzsch
 *
 * modEQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * modEQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with modEQ.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modulation_source_view.h"

namespace TA
{
//==============================================================================
ModulationSourceView::ModulationSourceView()
    : frequency(Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox)
    , gain(Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox)
{
    addAndMakeVisible(frequency);
    addAndMakeVisible(gain);
}

ModulationSourceView::~ModulationSourceView() {}

void ModulationSourceView::paint(Graphics& g)
{
    const Colour inputColour  = Colours::greenyellow;
    const Colour outputColour = Colours::red;

    Graphics::ScopedSaveState state(g);

    g.setFont(12.0f);
    g.setColour(Colours::silver);
    g.drawRoundedRectangle(plotFrame.toFloat(), 6, 5);
    for (int i = 0; i < 10; ++i)
    {
        g.setColour(Colours::silver.withAlpha(0.4f));
        auto x = plotFrame.getX() + plotFrame.getWidth() * i * 0.1f;
        if (i > 0)
            g.drawVerticalLine(roundToInt(x), (float)plotFrame.getY(), (float)plotFrame.getBottom());

        //  // g.setColour(Colour(0xffb9f6ca));
        //  // auto freq = getFrequencyForPosition(i * 0.1f);
        //  // g.drawFittedText((freq < 1000) ? String(freq) + " Hz" :
        //  String(freq / 1000, 1) + " kHz", roundToInt(x + 3),
        //  //                 plotFrame.getBottom() - 18, 50, 15,
        //  Justification::left, 1);
    }

    g.setColour(Colours::silver.withAlpha(0.4f));
    g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.25 * plotFrame.getHeight()),
                         (float)plotFrame.getX(), (float)plotFrame.getRight());
    g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.75 * plotFrame.getHeight()),
                         (float)plotFrame.getX(), (float)plotFrame.getRight());

    g.reduceClipRegion(plotFrame);

    // Input Analyser
    g.setFont(16.0f);
    g.drawFittedText("LFO", plotFrame.reduced(8), Justification::topRight, 1);
    g.setColour(Colour(0xff00ff08));
    g.strokePath(modulationPath, PathStrokeType(3.0));
}

void ModulationSourceView::resized()
{
    auto area       = getLocalBounds();
    auto sliderArea = area.removeFromRight(area.getWidth() / 4);

    frequency.setBounds(sliderArea.removeFromTop(sliderArea.getHeight() / 2));
    gain.setBounds(sliderArea);
    plotFrame = area.reduced(3, 3);
}

}  // namespace TA