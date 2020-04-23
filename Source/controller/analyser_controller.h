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
#include "modEQ.hpp"

// tobanteAudio
#include "../modEQ_processor.h"
#include "../view/analyser_view.h"
#include "band_controller.h"

namespace tobanteAudio
{
/**
 * @brief Controller for the AnalyserView component.
 */
class AnalyserController : public ChangeListener, public Timer, public MouseListener
{
public:
    /**
     * @brief Creates an AnalyserController connected to the EQ processor the
     * band controllers & the anaylser plot view.
     */
    AnalyserController(tobanteAudio::EqualizerProcessor&, OwnedArray<tobanteAudio::BandController>&,
                       tobanteAudio::AnalyserView&);

    /**
     * @brief Destructor.
     */
    ~AnalyserController();

    /**
     * @brief Listen to changes from the processor.
     */
    void changeListenerCallback(ChangeBroadcaster* sender) override;

    /**
     * @brief Refreshes the analyser plots.
     */
    void timerCallback() override;

    /**
     * @brief Selects a band type with right click.
     */
    void mouseDown(const MouseEvent& e) override;

    /**
     * @brief Select the current dragging band.
     */
    void mouseMove(const MouseEvent& e) override;

    /**
     * @brief Drags a band.
     */
    void mouseDrag(const MouseEvent& e) override;

    /**
     * @brief Deactivates the selected band.
     */
    void mouseDoubleClick(const MouseEvent& e) override;

    /**
     * @brief Changes the Q width of the selected band.
     */
    void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;

private:
    void updateFrequencyResponses();
    tobanteAudio::EqualizerProcessor& processor;
    OwnedArray<tobanteAudio::BandController>& bandControllers;
    tobanteAudio::AnalyserView& view;

    int draggingBand  = -1;
    bool draggingGain = false;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyserController)
};
}  // namespace tobanteAudio