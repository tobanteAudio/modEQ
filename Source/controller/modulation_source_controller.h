/*
  ==============================================================================

    modulation_source_controller.h
    Created: 15 Sep 2018 10:26:37am
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../processor/modulation_source_processor.h"
#include "../view/modulation_source_view.h"

//==============================================================================
namespace TA
{

class ModulationSourceController : public ChangeListener, public Timer
{
public:
  //==============================================================================
  ModulationSourceController(const int,TA::ModulationSourceProcessor&, TA::ModulationSourceView&);

  //==============================================================================
  void changeListenerCallback(ChangeBroadcaster* sender) override;
  void timerCallback() override;

private:
  //==============================================================================
  int index;
  TA::ModulationSourceView& view;
  TA::ModulationSourceProcessor& processor;

  //==============================================================================
  OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceController)
};

} // namespace TA
