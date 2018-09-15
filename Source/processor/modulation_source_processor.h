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


namespace TA
{
//==============================================================================
class ModulationSourceProcessor : public ProcessorBase, public AudioProcessorValueTreeState::Listener

{
public:
  //==============================================================================
  ModulationSourceProcessor(AudioProcessorValueTreeState&);
  ~ModulationSourceProcessor();

  //==============================================================================
  void prepareToPlay(double, int) override;
  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

  //==============================================================================
  void parameterChanged(const String& parameter, float newValue) override;

  //==============================================================================
  void createAnalyserPlot(Path&, Rectangle<int>&, float);
  bool checkForNewAnalyserData();

  //==============================================================================
  AudioProcessorValueTreeState& getPluginState() { return state; }
  void reset() override { oscillator.reset(); }

private:
  //==============================================================================
  float sampleRate;
  AudioProcessorValueTreeState& state;
  dsp::Oscillator<float> oscillator;
  TA::ModulationSourceAnalyser<float> analyser;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceProcessor)
};

} // namespace TA