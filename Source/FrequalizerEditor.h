/*
  ==============================================================================

 This is the Frequalizer UI editor

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequalizerProcessor.h"
#include "SocialButtons.h"
#include "view/BandView.h"
#include "controller/BandController.h"


//==============================================================================
/**
 */
class FrequalizerAudioProcessorEditor : public AudioProcessorEditor, public ChangeListener, public Timer
{
public:
  FrequalizerAudioProcessorEditor(FrequalizerAudioProcessor&);
  ~FrequalizerAudioProcessorEditor();

  //==============================================================================

  void paint(Graphics&) override;
  void resized() override;

  void changeListenerCallback(ChangeBroadcaster* sender) override;
  void timerCallback() override;

  void mouseDown(const MouseEvent& e) override;
  void mouseMove(const MouseEvent& e) override;
  void mouseDrag(const MouseEvent& e) override;
  void mouseDoubleClick(const MouseEvent& e) override;

private:
  void updateFrequencyResponses();

  static float getPositionForFrequency(float freq);

  static float getFrequencyForPosition(float pos);

  static float getPositionForGain(float gain, float top, float bottom);

  static float getGainForPosition(float pos, float top, float bottom);

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  FrequalizerAudioProcessor& processor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequalizerAudioProcessorEditor)

#ifdef JUCE_OPENGL
  OpenGLContext openGLContext;
#endif

  OwnedArray<TA::BandView> bandViews; 
  OwnedArray<TA::BandController> bandControllers;

  TA::BandView bandView;

  Rectangle<int> plotFrame;
  Rectangle<int> brandingFrame;

  Path frequencyResponse;

  GroupComponent frame;
  Slider output;

  SocialButtons socialButtons;

  int draggingBand = -1;
  bool draggingGain = false;

  OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
  SharedResourcePointer<TooltipWindow> tooltipWindow;

  PopupMenu contextMenu;
};
