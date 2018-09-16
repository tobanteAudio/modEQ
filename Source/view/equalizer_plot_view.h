/* Copyright 2018 Tobias Hienzsch
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../controller/band_controller.h"
#include "../processor/equalizer_processor.h"

namespace TA
{

//==============================================================================
/*
 */
class EqualizerPlotView : public Component, public ChangeListener, public Timer
{
public:
  //==============================================================================
  EqualizerPlotView(TA::EqualizerProcessor&, OwnedArray<TA::BandController>&);
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
  TA::EqualizerProcessor& processor;
  OwnedArray<TA::BandController>& bandControllers;

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
  int draggingBand = -1;
  bool draggingGain = false;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerPlotView)
};

} // namespace TA