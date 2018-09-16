/*
  ==============================================================================

    modulation_source_controller.h
    Created: 15 Sep 2018 10:26:37am
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../modEQ_processor.h"
#include "../processor/modulation_source_processor.h"
#include "../view/modulation_source_view.h"

//==============================================================================
namespace TA
{

class ModulationSourceController : public ChangeListener, public Timer
{
public:
  //==============================================================================
  ModulationSourceController(const int, ModEQProcessor&, TA::ModulationSourceProcessor&, TA::ModulationSourceView&);

  //==============================================================================
  void changeListenerCallback(ChangeBroadcaster* sender) override;
  void timerCallback() override;

private:
  //==============================================================================
  int index;
  ModEQProcessor& mainProcessor;
  TA::ModulationSourceProcessor& processor;
  TA::ModulationSourceView& view;

  //==============================================================================
  OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceController)
};

} // namespace TA
