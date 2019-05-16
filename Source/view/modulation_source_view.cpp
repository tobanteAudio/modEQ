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

#include "modulation_source_view.h"

namespace tobanteAudio
{
ModulationSourceView::ModulationSourceView(int i)
    : index(i)
    , frequency(Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox)
    , gain(Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox)
    , toggleConnectView(translate("Connect"))
    , modConnect1(i)
    , modConnect2(i + 1)
{
    // Slider
    addAndMakeVisible(frequency);
    addAndMakeVisible(gain);

    // Label
    freqLabel.setJustificationType(Justification::centred);
    gainLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(freqLabel);
    addAndMakeVisible(gainLabel);

    // Button
    addAndMakeVisible(toggleConnectView);

    // Connect View
    addAndMakeVisible(modConnect1);
    addAndMakeVisible(modConnect2);
}

void ModulationSourceView::paint(Graphics& g)
{
    // Save graphics state
    Graphics::ScopedSaveState state(g);

    // Background
    const auto backgroundColour
        = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    g.setColour(backgroundColour.darker());
    g.fillRect(plotFrame.toFloat());

    // Frame
    g.setFont(12.0f);
    g.setColour(Colours::silver);
    g.drawRoundedRectangle(plotFrame.toFloat(), 6, 5);

    // Vertical lines
    for (int i = 0; i < 10; ++i)
    {
        g.setColour(Colours::silver.withAlpha(0.4f));
        auto x = plotFrame.getX() + plotFrame.getWidth() * i * 0.1f;
        if (i > 0)
            g.drawVerticalLine(roundToInt(x),
                               static_cast<float>(plotFrame.getY()),
                               static_cast<float>(plotFrame.getBottom()));
    }

    // Horizontal lines
    g.setColour(Colours::silver.withAlpha(0.4f));
    g.drawHorizontalLine(
        roundToInt(plotFrame.getY() + 0.25 * plotFrame.getHeight()),
        static_cast<float>(plotFrame.getX()),
        static_cast<float>(plotFrame.getRight()));
    g.drawHorizontalLine(
        roundToInt(plotFrame.getY() + 0.75 * plotFrame.getHeight()),
        static_cast<float>(plotFrame.getX()),
        static_cast<float>(plotFrame.getRight()));

    g.reduceClipRegion(plotFrame);

    // Label
    g.setFont(16.0f);
    g.setColour(Colour(0xff00ff08));
    g.drawFittedText("LFO", plotFrame.reduced(12), Justification::topRight, 1);

    // LFO path
    g.setColour(Colour(0xff00ff08).withMultipliedAlpha(0.9f).brighter());
    g.strokePath(modulationPath, PathStrokeType(3.0));
}

void ModulationSourceView::resized()
{
    auto area       = getLocalBounds();
    auto sliderArea = area.removeFromRight(area.getWidth() / 4);

    int const labelHeight = sliderArea.getHeight() / 2 / 5;

    // Labels
    freqLabel.setBounds(sliderArea.removeFromTop(labelHeight));
    gainLabel.setBounds(sliderArea.removeFromBottom(labelHeight));

    // Sliders
    frequency.setBounds(sliderArea.removeFromTop(sliderArea.getHeight() / 2));
    gain.setBounds(sliderArea);

    // Button
    auto button_area = area.removeFromBottom(area.getHeight() / 6).reduced(1);
    toggleConnectView.setBounds(
        button_area.removeFromLeft(button_area.getWidth() / 2));

    // LFO plot
    auto reduced_area = area.reduced(3, 3);
    plotFrame         = reduced_area;

    // Connect
    modConnect1.setBounds(reduced_area.removeFromTop(area.getHeight() / 2));
    modConnect2.setBounds(reduced_area);
}

}  // namespace tobanteAudio
