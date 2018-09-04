/*
  ==============================================================================

    EqualizerPlotView.h
    Created: 4 Sep 2018 8:21:00pm
    Author:  tobante

  ==============================================================================
*/

#pragma once

#include "../processor/EqualizerProcessor.h"
#include "../controller/BandController.h"
#include "JuceHeader.h"

//==============================================================================
/*
 */
class EqualizerPlotView : public Component, public ChangeListener, public Timer
{
public:
  EqualizerPlotView(TA::EqualizerProcessor&, OwnedArray<TA::BandController>&);
  ~EqualizerPlotView();

  void paint(Graphics&) override;
  void resized() override;

  void changeListenerCallback(ChangeBroadcaster* sender) override;
  void timerCallback() override;

  void mouseDown(const MouseEvent& e) override;
  void mouseMove(const MouseEvent& e) override;
  void mouseDrag(const MouseEvent& e) override;
  void mouseDoubleClick(const MouseEvent& e) override;

private:
  TA::EqualizerProcessor& processor;
  OwnedArray<TA::BandController>& bandControllers;

  void updateFrequencyResponses();
  static float getPositionForFrequency(float freq);
  static float getFrequencyForPosition(float pos);
  static float getPositionForGain(float gain, float top, float bottom);
  static float getGainForPosition(float pos, float top, float bottom);

  Rectangle<int> plotFrame;
  //Rectangle<int> brandingFrame;

  Path frequencyResponse;
  GroupComponent frame;

  int draggingBand = -1;
  bool draggingGain = false;

  PopupMenu contextMenu;

  
//#ifdef JUCE_OPENGL
//  OpenGLContext openGLContext;
//#endif

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerPlotView)
};
