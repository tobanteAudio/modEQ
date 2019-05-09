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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../controller/band_controller.h"
#include "../processor/equalizer_processor.h"

namespace tobanteAudio
{
/*
 */
class EqualizerPlotView : public Component, public ChangeListener, public Timer
{
public:
    EqualizerPlotView(tobanteAudio::EqualizerProcessor&, OwnedArray<tobanteAudio::BandController>&);
    ~EqualizerPlotView() override = default;

    void paint(Graphics& /*g*/) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* sender) override;
    void timerCallback() override;

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
    void updateFrequencyResponses();

    static float get_position_for_frequency(float freq);
    static float get_frequency_for_position(float pos);
    static float get_position_for_gain(float gain, float top, float bottom);
    static float get_gain_for_position(float pos, float top, float bottom);

    /**
     * @brief Checks if two positions are in a given radius.
     */
    static bool overlap_with_radius(float obj_pos, float mouse_pos, int radius)
    {
        return std::abs(obj_pos - mouse_pos) < radius;
    }

    tobanteAudio::EqualizerProcessor& processor;
    OwnedArray<tobanteAudio::BandController>& bandControllers;

    Rectangle<int> plotFrame;
    Path frequencyResponse;
    PopupMenu contextMenu;

    int draggingBand  = -1;
    bool draggingGain = false;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerPlotView)
};

}  // namespace tobanteAudio