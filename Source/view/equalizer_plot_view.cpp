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

#include "equalizer_plot_view.h"
#include "../utils/constants.h"

namespace tobanteAudio
{
static int clickRadius = 10;
static float maxDB     = 24.0f;

//==============================================================================
EqualizerPlotView::EqualizerPlotView(tobanteAudio::EqualizerProcessor& p,
                                     OwnedArray<tobanteAudio::BandController>& bands)
    : processor(p), bandControllers(bands)
{
    processor.addChangeListener(this);
    startTimerHz(GLOBAL_REFRESH_RATE_HZ);
}

void EqualizerPlotView::paint(Graphics& g)
{
    // Save graphics state
    Graphics::ScopedSaveState state(g);

    // Frame
    g.setColour(Colours::silver);
    g.drawRoundedRectangle(plotFrame.toFloat(), 5, 4);

    // Vertical lines & frequency labels
    g.setFont(12.0f);
    for (int i = 0; i < 10; ++i)
    {
        g.setColour(Colours::silver.withAlpha(0.4f));
        auto x = plotFrame.getX() + plotFrame.getWidth() * i * 0.1f;
        if (i > 0)
            g.drawVerticalLine(roundToInt(x), (float)plotFrame.getY(), (float)plotFrame.getBottom());

        g.setColour(Colour(0xffb9f6ca));
        auto freq = get_frequency_for_position(i * 0.1f);
        g.drawFittedText((freq < 1000) ? String(freq) + " Hz" : String(freq / 1000, 1) + " kHz",
                         roundToInt(x + 3), plotFrame.getBottom() - 18, 50, 15, Justification::left,
                         1);
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
    g.drawFittedText(String(maxDB) + " dB", plotFrame.getX() + 3, plotFrame.getY() + 2, 50, 14,
                     Justification::left, 1);
    g.drawFittedText(String(maxDB / 2) + " dB", plotFrame.getX() + 3,
                     roundToInt(plotFrame.getY() + 2 + 0.25 * plotFrame.getHeight()), 50, 14,
                     Justification::left, 1);
    g.drawFittedText(" 0 dB", plotFrame.getX() + 3,
                     roundToInt(plotFrame.getY() + 2 + 0.5 * plotFrame.getHeight()), 50, 14,
                     Justification::left, 1);
    g.drawFittedText(String(-maxDB / 2) + " dB", plotFrame.getX() + 3,
                     roundToInt(plotFrame.getY() + 2 + 0.75 * plotFrame.getHeight()), 50, 14,
                     Justification::left, 1);

    g.reduceClipRegion(plotFrame);

    // Analysers
    const Colour inputColour  = Colours::greenyellow;
    const Colour outputColour = Colours::red;
    Path analyser;
    g.setFont(16.0f);

    //
    // Input Analyser
    //
    processor.createAnalyserPlot(analyser, plotFrame, 20.0f, true);
    g.setColour(inputColour);
    g.drawFittedText("Input", plotFrame.reduced(8), Justification::topRight, 1);
    g.strokePath(analyser, PathStrokeType(1.0));

    //
    // Output Analyser
    //
    processor.createAnalyserPlot(analyser, plotFrame, 20.0f, false);
    g.setColour(outputColour);
    g.drawFittedText("Output", plotFrame.reduced(8, 28), Justification::topRight, 1);
    g.strokePath(analyser, PathStrokeType(2.0));

    //
    // Frequency Response
    //
    g.setColour(Colour(0xff00ff08));
    g.strokePath(frequencyResponse, PathStrokeType(3.0));

    // Handles for each band
    for (int i = 0; i < bandControllers.size(); ++i)
    {
        const auto* band = processor.getBand(i);
        if (band == nullptr) return;

        // X
        const auto plotFrameX     = static_cast<float>(plotFrame.getX());
        const auto plotFrameWidth = static_cast<float>(plotFrame.getWidth());
        const auto bandPosition   = get_position_for_frequency(float(band->frequency));
        const auto x              = plotFrameX + bandPosition * plotFrameWidth;

        // Y
        const auto frameY      = static_cast<float>(plotFrame.getY());
        const auto frameBottom = static_cast<float>(plotFrame.getBottom());
        const auto gain        = static_cast<float>(band->gain);
        const auto y           = get_position_for_gain(gain, frameY, frameBottom);

        // Color (active || bypass)
        if (auto* param = processor.state.getParameter(processor.getActiveParamID(i)))
        {
            param->getValue() < 0.5f ? g.setColour(Colours::grey) : g.setColour(band->colour);
        }

        // Label
        const int size{30};
        const int offset{-20};
        g.drawFittedText(String(i), x + offset, y + offset, size, size, Justification::left, 1);

        // Handle
        g.drawEllipse(x, y, 8, 8, 5);
    }
}

void EqualizerPlotView::resized()
{
    auto area = getLocalBounds();
    plotFrame = area.reduced(3, 3);
    updateFrequencyResponses();
}

void EqualizerPlotView::changeListenerCallback(ChangeBroadcaster* sender)
{
    ignoreUnused(sender);
    updateFrequencyResponses();
    repaint();
}

void EqualizerPlotView::timerCallback()
{
    if (processor.checkForNewAnalyserData()) repaint(plotFrame);
}

void EqualizerPlotView::mouseDown(const MouseEvent& e)
{
    if (e.mods.isPopupMenu() && plotFrame.contains(e.x, e.y))
    {
        for (int i = 0; i < bandControllers.size(); ++i)
        {
            const auto* band = processor.getBand(i);
            if (band == nullptr) return;

            const auto plotFrameX     = static_cast<float>(plotFrame.getX());
            const auto plotFrameWidth = static_cast<float>(plotFrame.getWidth());
            const auto bandPosition   = get_position_for_frequency(float(band->frequency));
            const auto pos            = plotFrameX + bandPosition * plotFrameWidth;

            // If mouse & band match on x-axis
            if (overlap_with_radius(pos, e.position.getX(), clickRadius))
            {
                contextMenu.clear();
                for (int t = 0; t < tobanteAudio::EqualizerProcessor::LastFilterID; ++t)
                    contextMenu.addItem(
                        t + 1,
                        tobanteAudio::EqualizerProcessor::getFilterTypeName(
                            static_cast<tobanteAudio::EqualizerProcessor::FilterType>(t)),
                        true, band->type == t);

                contextMenu.showMenuAsync(
                    PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(
                        {e.getScreenX(), e.getScreenY(), 1, 1}),
                    [this, i](int const selected) {
                        if (selected > 0) bandControllers.getUnchecked(i)->setType(selected - 1);
                    });
            }  // If mouse x overlaps
        }      // For all bands
    }          // If in plotview
}

void EqualizerPlotView::mouseMove(const MouseEvent& e)
{
    if (plotFrame.contains(e.x, e.y))
    {
        for (int i = 0; i < bandControllers.size(); ++i)  //
        {
            const auto* band = processor.getBand(i);
            if (band == nullptr) return;

            const auto plotFrameX     = static_cast<float>(plotFrame.getX());
            const auto plotFrameWidth = static_cast<float>(plotFrame.getWidth());
            const auto bandPosition   = get_position_for_frequency(float(band->frequency));
            const auto pos            = plotFrameX + bandPosition * plotFrameWidth;

            // If mouse & band match on x-axis
            if (overlap_with_radius(pos, e.position.getX(), clickRadius))
            {
                const auto frameY      = static_cast<float>(plotFrame.getY());
                const auto frameBottom = static_cast<float>(plotFrame.getBottom());
                const auto gain        = static_cast<float>(band->gain);
                const auto gainPos     = get_position_for_gain(gain, frameY, frameBottom);

                // If mouse & band match on y-axis
                if (overlap_with_radius(gainPos, e.position.getY(), clickRadius))
                {
                    draggingGain = processor.state.getParameter(processor.getGainParamID(i));
                    setMouseCursor(MouseCursor(MouseCursor::UpDownLeftRightResizeCursor));
                }
                else
                {
                    setMouseCursor(MouseCursor(MouseCursor::LeftRightResizeCursor));
                }
                if (i != draggingBand)
                {
                    draggingBand = i;
                    repaint(plotFrame);
                }
                return;
            }  // If mouse & band match on x-axis
        }      // For all bands
    }          // If in plotframe

    draggingBand = -1;
    draggingGain = false;
    setMouseCursor(MouseCursor(MouseCursor::NormalCursor));
    repaint(plotFrame);
}

void EqualizerPlotView::mouseDrag(const MouseEvent& e)
{
    if (isPositiveAndBelow(draggingBand, bandControllers.size()))
    {
        const auto pos = (e.position.getX() - plotFrame.getX()) / plotFrame.getWidth();
        bandControllers[draggingBand]->setFrequency(get_frequency_for_position(pos));
        if (draggingGain)
            bandControllers[draggingBand]->setGain(
                get_gain_for_position(e.position.getY(), static_cast<float>(plotFrame.getY()),
                                      static_cast<float>(plotFrame.getBottom())));
    }
}

void EqualizerPlotView::mouseDoubleClick(const MouseEvent& e)
{
    if (plotFrame.contains(e.x, e.y))
    {
        for (int i = 0; i < bandControllers.size(); ++i)
        {
            const auto* band = processor.getBand(i);
            if (band == nullptr) return;

            const auto plotFrameX     = plotFrame.getX();
            const auto plotFrameWidth = plotFrame.getWidth();
            const auto bandPosition   = get_position_for_frequency(float(band->frequency));
            const auto pos            = plotFrameX + bandPosition * plotFrameWidth;

            if (overlap_with_radius(pos, e.position.getX(), clickRadius))
            {
                if (auto* param = processor.state.getParameter(processor.getActiveParamID(i)))
                {
                    param->setValueNotifyingHost(param->getValue() < 0.5f ? 1.0f : 0.0f);
                }
            }
        }
    }
}

void EqualizerPlotView::mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel)
{
    // If mouse is in plotview
    if (plotFrame.contains(e.x, e.y))
    {
        // For all bands
        for (int i = 0; i < bandControllers.size(); ++i)
        {
            const auto* band = processor.getBand(i);
            if (band == nullptr) return;

            const auto plotFrameX     = plotFrame.getX();
            const auto plotFrameWidth = plotFrame.getWidth();
            const auto bandPosition   = get_position_for_frequency(float(band->frequency));
            const auto pos            = plotFrameX + bandPosition * plotFrameWidth;

            // If mouse & band match on x-axis
            if (overlap_with_radius(pos, e.position.getX(), clickRadius))
            {
                const auto frameY      = static_cast<float>(plotFrame.getY());
                const auto frameBottom = static_cast<float>(plotFrame.getBottom());
                const auto gain        = static_cast<float>(band->gain);
                const auto gainPos     = get_position_for_gain(gain, frameY, frameBottom);

                // If mouse & band match on y-axis
                if (overlap_with_radius(gainPos, e.position.getY(), clickRadius))
                {
                    const auto paramID = processor.getQualityParamID(i);
                    if (auto* param = processor.state.getParameter(paramID))
                    {
                        const auto wheelMovement = wheel.deltaY * 0.05;
                        const auto newValue      = param->getValue() + wheelMovement;
                        param->setValueNotifyingHost(static_cast<float>(newValue));
                    }
                }  // If mouse & band match on y-axis
            }      // If mouse & band match on x-axis
        }          // For all bands
    }              // If mouse is in plotview
}

void EqualizerPlotView::updateFrequencyResponses()
{
    auto const pixelsPerDouble = 2.0f * plotFrame.getHeight() / Decibels::decibelsToGain(maxDB);

    for (int i = 0; i < bandControllers.size(); ++i)
    {
        auto* bandController = bandControllers.getUnchecked(i);
        bandController->updateSoloState(i);
        if (auto* band = processor.getBand(i))
        {
            bandController->updateControls(band->type);
            bandController->frequencyResponse.clear();
            processor.createFrequencyPlot(bandController->frequencyResponse, band->magnitudes,
                                          plotFrame.withX(plotFrame.getX() + 1), pixelsPerDouble);
        }
        bandController->updateSoloState(processor.getBandSolo(i));
    }
    frequencyResponse.clear();
    processor.createFrequencyPlot(frequencyResponse, processor.getMagnitudes(), plotFrame,
                                  pixelsPerDouble);
}

float EqualizerPlotView::get_position_for_frequency(float const freq)
{
    return (std::log(freq / 20.0f) / std::log(2.0f)) / 10.0f;
}

float EqualizerPlotView::get_frequency_for_position(float const pos)
{
    return 20.0f * std::pow(2.0f, pos * 10.0f);
}

float EqualizerPlotView::get_position_for_gain(float const gain, float const top, float const bottom)
{
    return jmap(Decibels::gainToDecibels(gain, -maxDB), -maxDB, maxDB, bottom, top);
}

float EqualizerPlotView::get_gain_for_position(float const pos, float const top, float const bottom)
{
    return Decibels::decibelsToGain(jmap(pos, bottom, top, -maxDB, maxDB), -maxDB);
}

}  // namespace tobanteAudio