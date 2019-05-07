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

#include "equalizer_plot_view.h"
#include "../utils/constants.h"

namespace tobanteAudio
{
static int clickRadius = 10;
static float maxDB     = 24.0f;

//==============================================================================
EqualizerPlotView::EqualizerPlotView(tobanteAudio::EqualizerProcessor& p, OwnedArray<tobanteAudio::BandController>& bands)
    : processor(p), bandControllers(bands)
{
    processor.addChangeListener(this);
    startTimerHz(GLOBAL_REFRESH_RATE_HZ);
}

EqualizerPlotView::~EqualizerPlotView() { processor.removeChangeListener(this); }

void EqualizerPlotView::paint(Graphics& g)
{
    // Setup
    const Colour inputColour  = Colours::greenyellow;
    const Colour outputColour = Colours::red;

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
        if (i > 0) g.drawVerticalLine(roundToInt(x), (float)plotFrame.getY(), (float)plotFrame.getBottom());

        g.setColour(Colour(0xffb9f6ca));
        auto freq = getFrequencyForPosition(i * 0.1f);
        g.drawFittedText((freq < 1000) ? String(freq) + " Hz" : String(freq / 1000, 1) + " kHz", roundToInt(x + 3),
                         plotFrame.getBottom() - 18, 50, 15, Justification::left, 1);
    }

    // Horizontal lines
    g.setColour(Colours::silver.withAlpha(0.4f));
    g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.25 * plotFrame.getHeight()),
                         static_cast<float>(plotFrame.getX()), static_cast<float>(plotFrame.getRight()));
    g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.75 * plotFrame.getHeight()),
                         static_cast<float>(plotFrame.getX()), static_cast<float>(plotFrame.getRight()));

    // dB labels
    g.setColour(Colours::silver);
    g.drawFittedText(String(maxDB) + " dB", plotFrame.getX() + 3, plotFrame.getY() + 2, 50, 14, Justification::left, 1);
    g.drawFittedText(String(maxDB / 2) + " dB", plotFrame.getX() + 3,
                     roundToInt(plotFrame.getY() + 2 + 0.25 * plotFrame.getHeight()), 50, 14, Justification::left, 1);
    g.drawFittedText(" 0 dB", plotFrame.getX() + 3, roundToInt(plotFrame.getY() + 2 + 0.5 * plotFrame.getHeight()), 50,
                     14, Justification::left, 1);
    g.drawFittedText(String(-maxDB / 2) + " dB", plotFrame.getX() + 3,
                     roundToInt(plotFrame.getY() + 2 + 0.75 * plotFrame.getHeight()), 50, 14, Justification::left, 1);

    g.reduceClipRegion(plotFrame);

    // Analysers
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
        for (int i = 0; i < bandControllers.size(); ++i)
            if (auto* band = processor.getBand(i))
            {
                if (std::abs(static_cast<float>(plotFrame.getX())
                             + getPositionForFrequency(int(band->frequency)) * static_cast<float>(plotFrame.getWidth())
                             - e.position.getX())
                    < clickRadius)
                {
                    contextMenu.clear();
                    for (int t = 0; t < tobanteAudio::EqualizerProcessor::LastFilterID; ++t)
                        contextMenu.addItem(t + 1,
                                            tobanteAudio::EqualizerProcessor::getFilterTypeName(
                                                static_cast<tobanteAudio::EqualizerProcessor::FilterType>(t)),
                                            true, band->type == t);

                    contextMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(
                                                  {e.getScreenX(), e.getScreenY(), 1, 1}),
                                              [this, i](int const selected) {
                                                  if (selected > 0)
                                                      bandControllers.getUnchecked(i)->setType(selected - 1);
                                              });
                }
            }
}

void EqualizerPlotView::mouseMove(const MouseEvent& e)
{
    if (plotFrame.contains(e.x, e.y))
    {
        for (int i = 0; i < bandControllers.size(); ++i)  //
        {
            if (auto* band = processor.getBand(i))
            {
                auto const pos
                    = plotFrame.getX() + getPositionForFrequency(float(band->frequency)) * plotFrame.getWidth();
                if (std::abs(pos - e.position.getX()) < clickRadius)
                {
                    if (std::abs(getPositionForGain(float(band->gain), plotFrame.getY(), plotFrame.getBottom())
                                 - e.position.getY())
                        < clickRadius)
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
                }
            }
        }
    }
    draggingBand = -1;
    draggingGain = false;
    setMouseCursor(MouseCursor(MouseCursor::NormalCursor));
    repaint(plotFrame);
}

void EqualizerPlotView::mouseDrag(const MouseEvent& e)
{
    if (isPositiveAndBelow(draggingBand, bandControllers.size()))
    {
        auto const pos = (e.position.getX() - plotFrame.getX()) / plotFrame.getWidth();
        bandControllers[draggingBand]->setFrequency(getFrequencyForPosition(pos));
        if (draggingGain)
            bandControllers[draggingBand]->setGain(getGainForPosition(
                e.position.getY(), static_cast<float>(plotFrame.getY()), static_cast<float>(plotFrame.getBottom())));
    }
}

void EqualizerPlotView::mouseDoubleClick(const MouseEvent& e)
{
    if (plotFrame.contains(e.x, e.y))
    {
        for (int i = 0; i < bandControllers.size(); ++i)
        {
            if (auto* band = processor.getBand(i))
            {
                if (std::abs(plotFrame.getX() + getPositionForFrequency(float(band->frequency)) * plotFrame.getWidth()
                             - e.position.getX())
                    < clickRadius)
                {
                    if (auto* param = processor.state.getParameter(processor.getActiveParamID(i)))
                        param->setValueNotifyingHost(param->getValue() < 0.5f ? 1.0f : 0.0f);
                }
            }
        }
    }
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
    processor.createFrequencyPlot(frequencyResponse, processor.getMagnitudes(), plotFrame, pixelsPerDouble);
}

float EqualizerPlotView::getPositionForFrequency(float const freq)
{
    return (std::log(freq / 20.0f) / std::log(2.0f)) / 10.0f;
}

float EqualizerPlotView::getFrequencyForPosition(float const pos) { return 20.0f * std::pow(2.0f, pos * 10.0f); }

float EqualizerPlotView::getPositionForGain(float const gain, float const top, float const bottom)
{
    return jmap(Decibels::gainToDecibels(gain, -maxDB), -maxDB, maxDB, bottom, top);
}

float EqualizerPlotView::getGainForPosition(float const pos, float const top, float const bottom)
{
    return Decibels::decibelsToGain(jmap(pos, bottom, top, -maxDB, maxDB), -maxDB);
}

}  // namespace tobanteAudio