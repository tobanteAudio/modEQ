/*
  ==============================================================================

    This is the Frequalizer processor

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Analyser.h"
#include "processor/EqualizerProcessor.h"
#include "processor/modulation_source_processor.h"
#include "utils/TextValueConverters.h"

//==============================================================================
class FrequalizerAudioProcessor : public AudioProcessor,
                                  public AudioProcessorValueTreeState::Listener,
                                  public ChangeBroadcaster
{
public:
  //==============================================================================
  FrequalizerAudioProcessor();
  ~FrequalizerAudioProcessor();

  //==============================================================================
  void prepareToPlay(double newSampleRate, int newSamplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(AudioBuffer<float>&, MidiBuffer&) override;
  void parameterChanged(const String& parameter, float newValue) override;

  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;


  //==============================================================================
  const String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const String getProgramName(int index) override;
  void changeProgramName(int index, const String& newName) override;

  //==============================================================================
  void getStateInformation(MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  //==============================================================================
  TA::EqualizerProcessor& getEQ() { return eqProcessor; }
  AudioProcessorValueTreeState& getPluginState() { return state; }
  TA::ModulationSourceProcessor modSource;

private:
  //==============================================================================
  UndoManager undo;
  AudioProcessorValueTreeState state;

  TA::EqualizerProcessor eqProcessor;
  
  AudioBuffer<float> modBuffer;

  dsp::Gain<float> outputGain;
  double sampleRate = 0;

  GainTextConverter gainTextConverter;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequalizerAudioProcessor)
};
