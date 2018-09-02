/*
  ==============================================================================

    This is the Frequalizer processor

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Analyser.h"
#include "processor/EqualizerProcessor.h"

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
  String getBandName(const int index) const;
  Colour getBandColour(const int index) const;
  void setBandSolo(const int index);
  bool getBandSolo(const int index) const;

  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const std::vector<double>& getMagnitudes();
  void
  createFrequencyPlot(Path& p, const std::vector<double>& mags, const Rectangle<int> bounds, float pixelsPerDouble);
  void createAnalyserPlot(Path& p, const Rectangle<int> bounds, float minFreq, bool input);
  bool checkForNewAnalyserData();

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
  TA::EqualizerProcessor::Band* getBand(const int index);
  TA::EqualizerProcessor& getEQ() { return eqProcessor; }
  AudioProcessorValueTreeState& getPluginState() { return state; }


private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequalizerAudioProcessor)

  void updateBand(const size_t index);
  void updateBypassedStates();
  void updatePlots();

  UndoManager undo;
  AudioProcessorValueTreeState state;

  std::vector<TA::EqualizerProcessor::Band> bands;
  TA::EqualizerProcessor eqProcessor;

  std::vector<double> frequencies;
  std::vector<double> magnitudes;

  bool wasBypassed = true;

  using FilterBand = dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>>;
  using Gain = dsp::Gain<float>;
  dsp::ProcessorChain<FilterBand, FilterBand, FilterBand, FilterBand, FilterBand, FilterBand, Gain> filter;

  double sampleRate = 0;

  int soloed = -1;

  Analyser<float> inputAnalyser;
  Analyser<float> outputAnalyser;
};
