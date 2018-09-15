/*
  ==============================================================================

    BaseProcessor.h
    Created: 2 Sep 2018 10:59:21pm
    Author:  tobante

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace TA
{

//==============================================================================
class ProcessorBase : public AudioProcessor
{
public:
  //==============================================================================
  ProcessorBase(AudioProcessorValueTreeState& vts): state(vts) {}
  ~ProcessorBase() {}

  //==============================================================================
  void prepareToPlay(double, int) override {}
  void releaseResources() override {}
  void processBlock(AudioBuffer<float>&, MidiBuffer&) override {}

  //==============================================================================
  AudioProcessorEditor* createEditor() override { return nullptr; }
  bool hasEditor() const override { return false; }

  //==============================================================================
  const String getName() const override { return {}; }
  bool acceptsMidi() const override { return false; }
  bool producesMidi() const override { return false; }
  double getTailLengthSeconds() const override { return 0; }

  //==============================================================================
  int getNumPrograms() override { return 0; }
  int getCurrentProgram() override { return 0; }
  void setCurrentProgram(int) override {}
  const String getProgramName(int) override { return {}; }
  void changeProgramName(int, const String&) override {}

  //==============================================================================
  void getStateInformation(MemoryBlock&) override {}
  void setStateInformation(const void*, int) override {}

  //==============================================================================
  AudioProcessorValueTreeState& getPluginState() { return state; }
  AudioProcessorValueTreeState& state;

private:
  //==============================================================================
  

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorBase)
};


} // namespace TA