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

// tobanteAudio
#include "../modEQ_processor.h"
#include "../view/analyser_view.h"

namespace tobanteAudio
{
class AnalyserController : public ChangeListener, public Timer, public MouseListener
{
public:
    AnalyserController(ModEQProcessor& /*p*/, tobanteAudio::AnalyserView& /*v*/);

    void changeListenerCallback(ChangeBroadcaster* sender) override;
    void timerCallback() override;

    /**
     * @brief Selects a band.
     */
    void mouseDown(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;
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
    tobanteAudio::AnalyserView& view;
    ModEQProcessor& mainProcessor;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyserController)
};
}  // namespace tobanteAudio