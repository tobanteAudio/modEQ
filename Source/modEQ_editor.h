/*
  ==============================================================================

 This is the Frequalizer UI editor

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
#include "modEQ_processor.h"
#include "controller/band_controller.h"
#include "look_and_feel/tobante_look_and_feel.h"
#include "view/band_view.h"
#include "view/equalizer_plot_view.h"
#include "view/modulation_source_view.h"
#include "view/social_buttons.h"


//==============================================================================
/**
 */
class ModEQEditor : public AudioProcessorEditor
{
public:
  //==============================================================================
  ModEQEditor(ModEQProcessor&);
  ~ModEQEditor();

  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;

private:
  //==============================================================================
  ModEQProcessor& processor;

  //==============================================================================
  TA::TobanteLookAndFeel tobanteLookAndFeel;
  OwnedArray<TA::BandView> bandViews;
  OwnedArray<TA::BandController> bandControllers;
  TA::ModulationSourceView modView;
  TA::EqualizerPlotView plotView;

  //==============================================================================
  Rectangle<int> plotArea;

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
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModEQEditor)
};
