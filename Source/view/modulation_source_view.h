/*
  ==============================================================================

    modulation_source_view.h
    Created: 12 Sep 2018 10:35:39pm
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../analyser/modulation_source_analyser.h"
#include "../processor/modulation_source_processor.h"


namespace TA
{
//==============================================================================
class ModulationSourceView : public Component
{
public:
  //==============================================================================
  ModulationSourceView();
  ~ModulationSourceView();

  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;

  //==============================================================================
  Slider frequency;
  Slider gain;
  Rectangle<int> plotFrame;
  Path modulationPath;

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceView)
};

} // namespace TA