/*
  ==============================================================================

 This is the Frequalizer UI editor

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequalizerProcessor.h"
#include "SocialButtons.h"
#include "controller/BandController.h"
#include "look_and_feel/TobanteLookAndFeel.h"
#include "view/BandView.h"
#include "view/EqualizerPlotView.h"


//==============================================================================
/**
 */
class FrequalizerAudioProcessorEditor : public AudioProcessorEditor, public ChangeListener
{
public:
  FrequalizerAudioProcessorEditor(FrequalizerAudioProcessor&);
  ~FrequalizerAudioProcessorEditor();

  //==============================================================================

  void paint(Graphics&) override;
  void resized() override;

  void changeListenerCallback(ChangeBroadcaster* sender) override;

private:

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  FrequalizerAudioProcessor& processor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequalizerAudioProcessorEditor)

#ifdef JUCE_OPENGL
  OpenGLContext openGLContext;
#endif

  TA::TobanteLookAndFeel tobanteLookAndFeel;
  OwnedArray<TA::BandView> bandViews;
  OwnedArray<TA::BandController> bandControllers;
  EqualizerPlotView plotView;

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
