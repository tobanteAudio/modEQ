/*
  ==============================================================================

    EqualizerProcessor.h
    Created: 2 Sep 2018 10:59:13pm
    Author:  tobante

  ==============================================================================
*/

#pragma once
#include "BaseProcessor.h"

namespace TA
{
//==============================================================================
class EqualizerProcessor : public ProcessorBase
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

  //==============================================================================
  void prepareToPlay(double, int) override;
  void prepare(const dsp::ProcessSpec&);

  //==============================================================================
  void process(const dsp::ProcessContextReplacing<float>&);
  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

  //==============================================================================
  void reset() override;

  //==============================================================================
  static String getFilterTypeName(const TA::EqualizerProcessor::FilterType type);
  const String getName() const override { return "Equalizer"; }
  Band* getBand(const int index);

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
  void updateBypassedStates();
  bool getBandSolo(const int index) const;

private:
  //==============================================================================
  AudioProcessorValueTreeState& state;
  std::vector<Band> bands;

  double sampleRate = 0;
  int soloed = -1;
};

} // namespace TA