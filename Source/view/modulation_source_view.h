/*
  ==============================================================================

    modulation_source_view.h
    Created: 12 Sep 2018 10:35:39pm
    Author:  tobante

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../analyser/modulation_source_analyser.h"
#include "../processor/modulation_source_processor.h"


namespace TA
{
//==============================================================================
/*
 */
class ModulationSourceView : public Component, public ChangeListener, public Timer
{
public:
  //==============================================================================
  ModulationSourceView(TA::ModulationSourceProcessor&);
  ~ModulationSourceView();

  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;

  //==============================================================================
  void changeListenerCallback(ChangeBroadcaster* sender) override;
  void timerCallback() override;


private:
  //==============================================================================
  TA::ModulationSourceProcessor& processor;

  //==============================================================================
  Rectangle<int> plotFrame;
  Path modulationPath;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceView)
};

} // namespace TA