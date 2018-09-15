/*
  ==============================================================================

    EqualizerPlotView.h
    Created: 4 Sep 2018 8:21:00pm
    Author:  Tobias Hienzsch

  ==============================================================================
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