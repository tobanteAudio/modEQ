/*
  ==============================================================================

    modulation_source_processor.h
    Created: 12 Sep 2018 8:41:46pm
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#pragma once

#include "../analyser/modulation_source_analyser.h"
#include "base_processor.h"
#include "../utils/text_value_converter.h"

namespace TA
{
//==============================================================================
class ModulationSourceProcessor : public BaseProcessor, AudioProcessorValueTreeState::Listener

{
public:
  //==============================================================================
  ModulationSourceProcessor(AudioProcessorValueTreeState&);
  ~ModulationSourceProcessor();

  //==============================================================================
  void prepareToPlay(double, int) override;
  void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

  //==============================================================================
  void parameterChanged(const String& parameter, float newValue) override;

  //==============================================================================
  void createAnalyserPlot(Path&, Rectangle<int>&, float);
  bool checkForNewAnalyserData();

  //==============================================================================
  void reset() override { oscillator.reset(); }

private:
  //==============================================================================
  int index;
  dsp::Oscillator<float> oscillator;
  TA::ModulationSourceAnalyser<float> analyser;
  FrequencyTextConverter frequencyTextConverter;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceProcessor)
};

} // namespace TA