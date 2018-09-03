/*
  ==============================================================================

    EqualizerProcessor.h
    Created: 2 Sep 2018 10:59:13pm
    Author:  tobante

  ==============================================================================
*/

#pragma once
#include "../Analyser.h"
#include "BaseProcessor.h"

namespace TA
{
//==============================================================================
class EqualizerProcessor : public ProcessorBase, public ChangeBroadcaster, AudioProcessorValueTreeState::Listener

{
public:
  //==============================================================================
  enum FilterType
  {
    NoFilter = 0,
    HighPass,
    HighPass1st,
    LowShelf,
    BandPass,
    AllPass,
    AllPass1st,
    Notch,
    Peak,
    HighShelf,
    LowPass1st,
    LowPass,
    LastFilterID
  };

  //==============================================================================
  struct Band
  {
    String name;
    Colour colour;
    FilterType type = BandPass;
    float frequency = 1000.0f;
    float quality = 1.0f;
    float gain = 1.0f;
    bool active = true;
    std::vector<double> magnitudes;
  };

  //==============================================================================
  EqualizerProcessor(AudioProcessorValueTreeState&);
  ~EqualizerProcessor();

  //==============================================================================
  void prepareToPlay(double, int) override;
  void prepare(const dsp::ProcessSpec&);

  //==============================================================================
  void process(const dsp::ProcessContextReplacing<float>&);
  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

  //==============================================================================
  void reset() override;
   void parameterChanged(const String& parameter, float newValue) override;

  //==============================================================================
  static String getFilterTypeName(const TA::EqualizerProcessor::FilterType type);
  const String getName() const override { return "Equalizer"; }
  Band* getBand(const int index);

  //==============================================================================
  void updateBand(const size_t index);
  void updateBypassedStates();
  void updatePlots();

  //==============================================================================
  const std::vector<double>& getMagnitudes();
  void
  createFrequencyPlot(Path& p, const std::vector<double>& mags, const Rectangle<int> bounds, float pixelsPerDouble);
  void createAnalyserPlot(Path& p, const Rectangle<int> bounds, float minFreq, bool input);
  bool checkForNewAnalyserData();

  //==============================================================================
  static String paramOutput;
  static String paramType;
  static String paramFrequency;
  static String paramQuality;
  static String paramGain;
  static String paramActive;

  String getTypeParamName(const int index) const;
  String getFrequencyParamName(const int index) const;
  String getQualityParamName(const int index) const;
  String getGainParamName(const int index) const;
  String getActiveParamName(const int index) const;

  //==============================================================================
  int getNumBands() const;
  String getBandName(const int index) const;
  Colour getBandColour(const int index) const;
  void setBandSolo(const int index);
  bool getBandSolo(const int index) const;

  //==============================================================================
  using FilterBand = dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>>;

  dsp::ProcessorChain<FilterBand, FilterBand, FilterBand, FilterBand, FilterBand, FilterBand> filter;
  std::vector<Band> bands;

  std::vector<double> frequencies;
  std::vector<double> magnitudes;

private:
  //==============================================================================
  AudioProcessorValueTreeState& state;


  //==============================================================================
  double sampleRate = 0;
  int soloed = -1;
  bool wasBypassed = true;


  Analyser<float> inputAnalyser;
  Analyser<float> outputAnalyser;
};

} // namespace TA