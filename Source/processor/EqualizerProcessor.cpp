/*
  ==============================================================================

    EqualizerProcessor.cpp
    Created: 2 Sep 2018 10:59:13pm
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#include "EqualizerProcessor.h"

//==============================================================================
namespace TA
{

//==============================================================================
String EqualizerProcessor::paramOutput("output");
String EqualizerProcessor::paramType("type");
String EqualizerProcessor::paramFrequency("frequency");
String EqualizerProcessor::paramQuality("quality");
String EqualizerProcessor::paramGain("gain");
String EqualizerProcessor::paramActive("active");

//==============================================================================
EqualizerProcessor::EqualizerProcessor(AudioProcessorValueTreeState& vts)
  : state(vts)
{
  // needs to be in sync with the ProcessorChain filter
  bands.resize(6);

  // setting defaults
  {
    auto& band = bands[0];
    band.name = "Lowest";
    band.colour = Colours::blue;
    band.frequency = 20.0f;
    band.quality = 0.707f;
    band.type = TA::EqualizerProcessor::HighPass;
  }
  {
    auto& band = bands[1];
    band.name = "Low";
    band.colour = Colours::brown;
    band.frequency = 250.0f;
    band.type = TA::EqualizerProcessor::LowShelf;
  }
  {
    auto& band = bands[2];
    band.name = "Low Mids";
    band.colour = Colours::green;
    band.frequency = 500.0f;
    band.type = TA::EqualizerProcessor::Peak;
  }
  {
    auto& band = bands[3];
    band.name = "High Mids";
    band.colour = Colours::coral;
    band.frequency = 1000.0f;
    band.type = TA::EqualizerProcessor::Peak;
  }
  {
    auto& band = bands[4];
    band.name = "High";
    band.colour = Colours::orange;
    band.frequency = 5000.0f;
    band.type = TA::EqualizerProcessor::HighShelf;
  }
  {
    auto& band = bands[5];
    band.name = "Highest";
    band.colour = Colours::red;
    band.frequency = 12000.0f;
    band.quality = 0.707f;
    band.type = TA::EqualizerProcessor::LowPass;
  }

}

//==============================================================================
void EqualizerProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}
void EqualizerProcessor::prepare(const dsp::ProcessSpec& spec) {}

void EqualizerProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer&) {}
void EqualizerProcessor::process(const dsp::ProcessContextReplacing<float>& context) {}

void EqualizerProcessor::reset() {}

//==============================================================================
String EqualizerProcessor::getFilterTypeName(const TA::EqualizerProcessor::FilterType type)
{
  switch (type)
  {
  case NoFilter:
    return TRANS("No Filter");
  case HighPass:
    return TRANS("High Pass");
  case HighPass1st:
    return TRANS("1st High Pass");
  case LowShelf:
    return TRANS("Low Shelf");
  case BandPass:
    return TRANS("Band Pass");
  case AllPass:
    return TRANS("All Pass");
  case AllPass1st:
    return TRANS("1st All Pass");
  case Notch:
    return TRANS("Notch");
  case Peak:
    return TRANS("Peak");
  case HighShelf:
    return TRANS("High Shelf");
  case LowPass1st:
    return TRANS("1st Low Pass");
  case LowPass:
    return TRANS("Low Pass");
  default:
    return TRANS("unknown");
  }
}


int EqualizerProcessor::getNumBands() const { return static_cast<int>(bands.size()); }

String EqualizerProcessor::getBandName(const int index) const
{
  if (isPositiveAndBelow(index, bands.size()))
    return bands[size_t(index)].name;
  return TRANS("unknown");
}
Colour EqualizerProcessor::getBandColour(const int index) const
{
  if (isPositiveAndBelow(index, bands.size()))
    return bands[size_t(index)].colour;
  return Colours::silver;
}

void EqualizerProcessor::setBandSolo(const int index)
{
  soloed = index;
  updateBypassedStates();
}

//==============================================================================
void EqualizerProcessor::updateBypassedStates()
{
  // if (isPositiveAndBelow(soloed, bands.size()))
  //{
  //  //filter.setBypassed<0>(soloed != 0);
  //  //filter.setBypassed<1>(soloed != 1);
  //  //filter.setBypassed<2>(soloed != 2);
  //  //filter.setBypassed<3>(soloed != 3);
  //  //filter.setBypassed<4>(soloed != 4);
  //  //filter.setBypassed<5>(soloed != 5);
  //}
  // else
  //{
  //  //filter.setBypassed<0>(!bands[0].active);
  //  //filter.setBypassed<1>(!bands[1].active);
  //  //filter.setBypassed<2>(!bands[2].active);
  //  //filter.setBypassed<3>(!bands[3].active);
  //  //filter.setBypassed<4>(!bands[4].active);
  //  //filter.setBypassed<5>(!bands[5].active);
  //}
  // updatePlots();
}

//==============================================================================
bool EqualizerProcessor::getBandSolo(const int index) const { return index == soloed; }

EqualizerProcessor::Band* EqualizerProcessor::getBand(const int index)
{
  if (isPositiveAndBelow(index, bands.size()))
    return &bands[size_t(index)];
  return nullptr;
}

String EqualizerProcessor::getTypeParamName(const int index) const { return getBandName(index) + "-" + paramType; }

String EqualizerProcessor::getFrequencyParamName(const int index) const
{
  return getBandName(index) + "-" + paramFrequency;
}

String EqualizerProcessor::getQualityParamName(const int index) const
{
  return getBandName(index) + "-" + paramQuality;
}

String EqualizerProcessor::getGainParamName(const int index) const { return getBandName(index) + "-" + paramGain; }

String EqualizerProcessor::getActiveParamName(const int index) const { return getBandName(index) + "-" + paramActive; }

} // namespace TA