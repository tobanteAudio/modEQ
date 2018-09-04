/*
  ==============================================================================

 This is the Frequalizer UI editor

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequalizerProcessor.h"

//==============================================================================
#include "SocialButtons.h"
#include "controller/BandController.h"
#include "look_and_feel/TobanteLookAndFeel.h"
#include "view/BandView.h"
#include "view/EqualizerPlotView.h"


//==============================================================================
/**
 */
class FrequalizerAudioProcessorEditor : public AudioProcessorEditor
{
public:
  //==============================================================================
  FrequalizerAudioProcessorEditor(FrequalizerAudioProcessor&);
  ~FrequalizerAudioProcessorEditor();

  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;

private:
  //==============================================================================
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  FrequalizerAudioProcessor& processor;

  //==============================================================================
  TA::TobanteLookAndFeel tobanteLookAndFeel;
  OwnedArray<TA::BandView> bandViews;
  OwnedArray<TA::BandController> bandControllers;
  EqualizerPlotView plotView;

  //==============================================================================
  Rectangle<int> plotFrame;
  Rectangle<int> brandingFrame;

  //==============================================================================
  SocialButtons socialButtons;
  Slider output;
  GroupComponent frame; // for output slider

  //==============================================================================
  OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
  SharedResourcePointer<TooltipWindow> tooltipWindow;

  //==============================================================================
#ifdef JUCE_OPENGL
  OpenGLContext openGLContext;
#endif

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequalizerAudioProcessorEditor)
};
