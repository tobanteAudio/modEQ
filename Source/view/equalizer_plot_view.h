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
//==============================================================================
/*
 */
class EqualizerPlotView : public Component, public ChangeListener, public Timer
{
public:
    //==============================================================================
    EqualizerPlotView(tobanteAudio::EqualizerProcessor&,
                      OwnedArray<tobanteAudio::BandController>&);
    ~EqualizerPlotView();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    //==============================================================================
    void changeListenerCallback(ChangeBroadcaster* sender) override;
    void timerCallback() override;

    //==============================================================================
    void mouseDown(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseDoubleClick(const MouseEvent& e) override;

private:
    //==============================================================================
    tobanteAudio::EqualizerProcessor& processor;
    OwnedArray<tobanteAudio::BandController>& bandControllers;

    //==============================================================================
    void updateFrequencyResponses();

    //==============================================================================
    static float getPositionForFrequency(float freq);
    static float getFrequencyForPosition(float pos);
    static float getPositionForGain(float gain, float top, float bottom);
    static float getGainForPosition(float pos, float top, float bottom);

    //==============================================================================
    Rectangle<int> plotFrame;
    Path frequencyResponse;
    PopupMenu contextMenu;

    //==============================================================================
    int draggingBand  = -1;
    bool draggingGain = false;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerPlotView)
};

}  // namespace tobanteAudio