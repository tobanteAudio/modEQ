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

#include "analyser_controller.h"

namespace tobanteAudio
{
AnalyserController::AnalyserController(tobanteAudio::EqualizerProcessor& p,
                                       OwnedArray<tobanteAudio::BandController>& bc,
                                       tobanteAudio::AnalyserView& v)
    : processor(p), bandControllers(bc), view(v)
{
    int i = 0;
    for (const auto& band : bandControllers)
    {
        ignoreUnused(band);
        view.handles.emplace_back(tobanteAudio::AnalyserView::BandHandle {i, 0, 0, 0, 0});
        ++i;
    }
    view.addMouseListener(this, false);
    view.addChangeListener(this);
    processor.addChangeListener(this);

    startTimerHz(GLOBAL_REFRESH_RATE_HZ);
}

AnalyserController::~AnalyserController()
{
    view.removeChangeListener(this);
    processor.removeChangeListener(this);
}

void AnalyserController::changeListenerCallback(ChangeBroadcaster* sender)
{
    ignoreUnused(sender);
    updateFrequencyResponses();
    view.repaint();
}
void AnalyserController::timerCallback()
{
    if (processor.checkForNewAnalyserData())
    {
        processor.createAnalyserPlot(view.in_analyser, view.plotFrame, 20.0f, true);
        processor.createAnalyserPlot(view.out_analyser, view.plotFrame, 20.0f, false);
        view.repaint(view.plotFrame);
    }
}

void AnalyserController::mouseDown(const MouseEvent& e)
{
    auto& plotFrame = view.plotFrame;
    if (e.mods.isPopupMenu() && plotFrame.contains(e.x, e.y))
    {
        for (int i = 0; i < bandControllers.size(); ++i)
        {
            const auto* band = processor.getBand(i);
            if (band == nullptr)
            {
                return;
            }

            const auto plotFrameX     = static_cast<float>(plotFrame.getX());
            const auto plotFrameWidth = static_cast<float>(plotFrame.getWidth());
            const auto bandPosition
                = view.get_position_for_frequency(float(band->frequency));
            const auto pos = plotFrameX + bandPosition * plotFrameWidth;

            // If mouse & band match on x-axis
            if (view.overlap_with_radius(pos, e.position.getX(), HANDLE_CLICK_RADIUS))
            {
                auto& contextMenu = view.contextMenu;
                contextMenu.clear();
                for (int t = 0; t < tobanteAudio::EqualizerProcessor::LastFilterID; ++t)
                {
                    contextMenu.addItem(
                        t + 1,
                        tobanteAudio::EqualizerProcessor::getFilterTypeName(
                            static_cast<tobanteAudio::EqualizerProcessor::FilterType>(t)),
                        true, band->type == t);
                }

                contextMenu.showMenuAsync(
                    PopupMenu::Options().withTargetComponent(&view).withTargetScreenArea(
                        {e.getScreenX(), e.getScreenY(), 1, 1}),
                    [this, i](int const selected) {
                        if (selected > 0)
                        {
                            bandControllers.getUnchecked(i)->setType(selected - 1);
                        }
                    });
            }  // If mouse x overlaps
        }      // For all bands
    }          // If in plotview
}

void AnalyserController::mouseMove(const MouseEvent& e)
{
    auto& plotFrame = view.plotFrame;

    if (plotFrame.contains(e.x, e.y))
    {
        for (int i = 0; i < bandControllers.size(); ++i)  //
        {
            const auto* band = processor.getBand(i);
            if (band == nullptr)
            {
                return;
            }

            const auto plotFrameX     = static_cast<float>(plotFrame.getX());
            const auto plotFrameWidth = static_cast<float>(plotFrame.getWidth());
            const auto bandPosition
                = view.get_position_for_frequency(float(band->frequency));
            const auto pos = plotFrameX + bandPosition * plotFrameWidth;

            // If mouse & band match on x-axis
            if (view.overlap_with_radius(pos, e.position.getX(), HANDLE_CLICK_RADIUS))
            {
                const auto frameY      = static_cast<float>(plotFrame.getY());
                const auto frameBottom = static_cast<float>(plotFrame.getBottom());
                const auto gain        = static_cast<float>(band->gain);
                const auto gainPos
                    = view.get_position_for_gain(gain, frameY, frameBottom);

                // If mouse & band match on y-axis
                if (view.overlap_with_radius(gainPos, e.position.getY(),
                                             HANDLE_CLICK_RADIUS))
                {
                    draggingGain
                        = processor.state.getParameter(processor.getGainParamID(i));
                    view.setMouseCursor(
                        MouseCursor(MouseCursor::UpDownLeftRightResizeCursor));
                }
                else
                {
                    view.setMouseCursor(MouseCursor(MouseCursor::LeftRightResizeCursor));
                }
                if (i != draggingBand)
                {
                    draggingBand = i;
                    view.repaint(plotFrame);
                }
                return;
            }  // If mouse & band match on x-axis
        }      // For all bands
    }          // If in plotframe

    draggingBand = -1;
    draggingGain = false;
    view.setMouseCursor(MouseCursor(MouseCursor::NormalCursor));
    view.repaint(plotFrame);
}
void AnalyserController::mouseDrag(const MouseEvent& e)
{
    auto& plotFrame = view.plotFrame;

    if (isPositiveAndBelow(draggingBand, bandControllers.size()))
    {
        const auto pos = (e.position.getX() - plotFrame.getX()) / plotFrame.getWidth();
        bandControllers[draggingBand]->setFrequency(view.get_frequency_for_position(pos));
        if (draggingGain)
        {
            bandControllers[draggingBand]->setGain(view.get_gain_for_position(
                e.position.getY(), static_cast<float>(plotFrame.getY()),
                static_cast<float>(plotFrame.getBottom())));
        }
    }
}
void AnalyserController::mouseDoubleClick(const MouseEvent& e)
{
    auto& plotFrame = view.plotFrame;

    if (plotFrame.contains(e.x, e.y))
    {
        for (int i = 0; i < bandControllers.size(); ++i)
        {
            const auto* band = processor.getBand(i);
            if (band == nullptr)
            {
                return;
            }

            const auto plotFrameX     = plotFrame.getX();
            const auto plotFrameWidth = plotFrame.getWidth();
            const auto bandPosition
                = view.get_position_for_frequency(float(band->frequency));
            const auto pos = plotFrameX + bandPosition * plotFrameWidth;

            if (view.overlap_with_radius(pos, e.position.getX(), HANDLE_CLICK_RADIUS))
            {
                if (auto* param
                    = processor.state.getParameter(processor.getActiveParamID(i)))
                {
                    param->setValueNotifyingHost(param->getValue() < 0.5f ? 1.0f : 0.0f);
                }
            }
        }
    }
}
void AnalyserController::mouseWheelMove(const MouseEvent& event,
                                        const MouseWheelDetails& wheel)
{
    auto& plotFrame = view.plotFrame;

    // If mouse is in plotview
    if (plotFrame.contains(event.x, event.y))
    {
        // For all bands
        for (int i = 0; i < bandControllers.size(); ++i)
        {
            const auto* band = processor.getBand(i);
            if (band == nullptr)
            {
                return;
            }

            const auto plotFrameX     = plotFrame.getX();
            const auto plotFrameWidth = plotFrame.getWidth();
            const auto bandPosition
                = view.get_position_for_frequency(float(band->frequency));
            const auto pos = plotFrameX + bandPosition * plotFrameWidth;

            // If mouse & band match on x-axis
            if (view.overlap_with_radius(pos, event.position.getX(), HANDLE_CLICK_RADIUS))
            {
                const auto frameY      = static_cast<float>(plotFrame.getY());
                const auto frameBottom = static_cast<float>(plotFrame.getBottom());
                const auto gain        = static_cast<float>(band->gain);
                const auto gainPos
                    = view.get_position_for_gain(gain, frameY, frameBottom);

                // If mouse & band match on y-axis
                if (view.overlap_with_radius(gainPos, event.position.getY(),
                                             HANDLE_CLICK_RADIUS))
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
void AnalyserController::updateFrequencyResponses()
{
    auto& plotFrame = view.plotFrame;
    auto const pixelsPerDouble
        = 2.0f * plotFrame.getHeight() / Decibels::decibelsToGain(MAX_DB);

    for (int i = 0; i < bandControllers.size(); ++i)
    {
        auto* bandController = bandControllers.getUnchecked(i);
        bandController->setSolo(i);
        if (auto* band = processor.getBand(i))
        {
            bandController->setUIControls(band->type);
            bandController->frequencyResponse.clear();
            processor.createFrequencyPlot(
                bandController->frequencyResponse, band->magnitudes,
                plotFrame.withX(plotFrame.getX() + 1), pixelsPerDouble);

            // HANDLE
            auto& handle = view.handles[i];
            // X
            const auto plotFrameX     = static_cast<float>(plotFrame.getX());
            const auto plotFrameWidth = static_cast<float>(plotFrame.getWidth());
            const auto bandPosition
                = view.get_position_for_frequency(float(band->frequency));
            handle.x = plotFrameX + bandPosition * plotFrameWidth;

            // Y
            const auto frameY      = static_cast<float>(plotFrame.getY());
            const auto frameBottom = static_cast<float>(plotFrame.getBottom());
            const auto gain        = static_cast<float>(band->gain);
            handle.y = view.get_position_for_gain(gain, frameY, frameBottom);

            // Color (active || bypass)
            if (auto* param = processor.state.getParameter(processor.getActiveParamID(i)))
            {
                param->getValue() < 0.5f ? handle.color = Colours::grey
                                         : handle.color = band->colour;
            }

            // Label
            const int offset {-20};
            handle.label_x = static_cast<float>(handle.x + offset);
            handle.label_y = static_cast<float>(handle.y + offset);
        }
        bandController->setSolo(processor.getBandSolo(i));
    }
    view.frequencyResponse.clear();
    processor.createFrequencyPlot(view.frequencyResponse, processor.getMagnitudes(),
                                  plotFrame, pixelsPerDouble);
}
}  // namespace tobanteAudio