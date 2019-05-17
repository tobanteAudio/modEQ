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

// JUCE
#include "../../JuceLibraryCode/JuceHeader.h"

// tobanteAudio
#include "analyser_view.h"

namespace tobanteAudio
{
void AnalyserView::paint(Graphics& g)
{
    // Save graphics state
    Graphics::ScopedSaveState state(g);

    // Background
    const auto bgColor = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    g.fillAll(bgColor.brighter().withAlpha(0.5f));

    // Vertical lines & frequency labels
    g.setFont(15.0f);
    for (int i = 0; i < 10; ++i)
    {
        g.setColour(Colours::silver.withAlpha(0.4f));
        auto x = plotFrame.getX() + plotFrame.getWidth() * i * 0.1f;
        if (i > 0)
        {
            const auto y      = static_cast<float>(plotFrame.getY());
            const auto bottom = static_cast<float>(plotFrame.getBottom());
            g.drawVerticalLine(roundToInt(x), y, bottom);
        }

        const auto freq = get_frequency_for_position(i * 0.1f);
        g.setColour(Colour(0xffb9f6ca));
        g.drawFittedText((freq < 1000) ? String(freq) + " Hz"
                                       : String(freq / 1000, 1) + " kHz",
                         roundToInt(x + 3), plotFrame.getBottom() - 18, 50, 15,
                         Justification::left, 1);
    }

    // Horizontal lines
    g.setColour(Colours::silver.withAlpha(0.4f));
    g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.25 * plotFrame.getHeight()),
                         static_cast<float>(plotFrame.getX()),
                         static_cast<float>(plotFrame.getRight()));
    g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.75 * plotFrame.getHeight()),
                         static_cast<float>(plotFrame.getX()),
                         static_cast<float>(plotFrame.getRight()));

    // dB labels
    g.setColour(Colours::silver);
    g.drawFittedText(String(MAX_DB) + " dB", plotFrame.getX() + 3, plotFrame.getY() + 2,
                     50, 14, Justification::left, 1);
    g.drawFittedText(String(MAX_DB / 2) + " dB", plotFrame.getX() + 3,
                     roundToInt(plotFrame.getY() + 2 + 0.25 * plotFrame.getHeight()), 50,
                     14, Justification::left, 1);
    g.drawFittedText(" 0 dB", plotFrame.getX() + 3,
                     roundToInt(plotFrame.getY() + 2 + 0.5 * plotFrame.getHeight()), 50,
                     14, Justification::left, 1);
    g.drawFittedText(String(-MAX_DB / 2) + " dB", plotFrame.getX() + 3,
                     roundToInt(plotFrame.getY() + 2 + 0.75 * plotFrame.getHeight()), 50,
                     14, Justification::left, 1);

    g.reduceClipRegion(plotFrame);

    // Analysers
    const Colour inputColour  = Colours::yellow;
    const Colour outputColour = Colours::purple;
    g.setFont(18.0f);

    const float corner_radius = 10.0f;

    // Input Analyser
    g.setColour(inputColour);
    g.drawFittedText("Input", plotFrame.reduced(8), Justification::topRight, 1);
    g.strokePath(in_analyser.createPathWithRoundedCorners(corner_radius),
                 PathStrokeType(1.0));

    // Output Analyser
    g.setColour(outputColour);
    g.drawFittedText("Output", plotFrame.reduced(8, 28), Justification::topRight, 1);
    g.strokePath(out_analyser.createPathWithRoundedCorners(corner_radius),
                 PathStrokeType(2.0));

    // Frequency Response
    g.setColour(Colour(0xff00ff08).withMultipliedAlpha(0.9f).brighter());
    g.strokePath(frequencyResponse.createPathWithRoundedCorners(corner_radius),
                 PathStrokeType(3.5f));

    for (const auto& handle : handles)
    {
        const int size{30};
        g.setColour(handle.color);
        // Label
        g.drawFittedText(String(handle.id), static_cast<int>(handle.label_x),
                         static_cast<int>(handle.label_y), size, size,
                         Justification::left, 1);

        // Handle
        g.drawEllipse(static_cast<float>(handle.x), static_cast<float>(handle.y), 8.0f,
                      8.0f, 5.0f);
    }
}

void AnalyserView::resized()
{
    auto area = getLocalBounds();
    plotFrame = area.reduced(3, 3);
    sendChangeMessage();
}
}  // namespace tobanteAudio