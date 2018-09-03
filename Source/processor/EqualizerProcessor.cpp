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
  const float maxGain = Decibels::decibelsToGain(24.0f);

  frequencies.resize(300);
  for (size_t i = 0; i < frequencies.size(); ++i)
  {
    frequencies[i] = 20.0 * std::pow(2.0, i / 30.0);
  }
  magnitudes.resize(frequencies.size());

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

  NormalisableRange<float> filterTypeRange(0, TA::EqualizerProcessor::LastFilterID, 1);
  NormalisableRange<float> frequencyRange(20.0, 20000.0, 1.0);
  NormalisableRange<float> qualityRange(0.1f, 10.0f, 0.1f);
  NormalisableRange<float> gainRange(1.0f / maxGain, maxGain, 0.001f);
  NormalisableRange<float> activeRange(0, 1, 1);

  frequencyRange.setSkewForCentre(2000.0f);
  qualityRange.setSkewForCentre(1.0f);
  gainRange.setSkewForCentre(1.0f);

  for (int i = 0; i < int(bands.size()); ++i)
  {
    auto& band = bands[size_t(i)];

    band.magnitudes.resize(frequencies.size(), 1.0);

    state.createAndAddParameter(
      getTypeParamName(i), band.name + " Type", TRANS("Filter Type"),
      filterTypeRange, (float)band.type,
      [](float value) {
        return TA::EqualizerProcessor::getFilterTypeName(
          static_cast<TA::EqualizerProcessor::FilterType>(static_cast<int>(value)));
      },
      [](String text) {
        for (int i = 0; i < TA::EqualizerProcessor::LastFilterID; ++i)
          if (text == TA::EqualizerProcessor::getFilterTypeName(static_cast<TA::EqualizerProcessor::FilterType>(i)))
            return static_cast<TA::EqualizerProcessor::FilterType>(i);
        return TA::EqualizerProcessor::NoFilter;
      },
      false, true, true);

    state.createAndAddParameter(
      getFrequencyParamName(i), band.name + " freq", "Frequency", frequencyRange,
      band.frequency,
      [](float value) { return (value < 1000) ? String(value, 0) + " Hz" : String(value / 1000.0, 2) + " kHz"; },
      [](String text) {
        return text.endsWith(" kHz") ? text.dropLastCharacters(4).getFloatValue() * 1000.0
                                     : text.dropLastCharacters(3).getFloatValue();
      },
      false, true, false);
    state.createAndAddParameter(getQualityParamName(i), band.name + " Q", TRANS("Quality"),
                                qualityRange, band.quality,
                                [](float value) { return String(value, 1); },
                                [](const String& text) { return text.getFloatValue(); }, false, true, false);
    state.createAndAddParameter(
      getGainParamName(i), band.name + " gain", TRANS("Gain"),
      gainRange, band.gain,
      [](float value) { return String(Decibels::gainToDecibels(value), 1) + " dB"; },
      [](String text) { return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); }, false, true,
      false);
    state.createAndAddParameter(getActiveParamName(i), band.name + " active", TRANS("Active"),
                                activeRange, band.active,
                                [](float value) { return value > 0.5f ? TRANS("active") : TRANS("bypassed"); },
                                [](String text) { return text == TRANS("active"); }, false, true, true);

    state.addParameterListener(getTypeParamName(i), this);
    state.addParameterListener(getFrequencyParamName(i), this);
    state.addParameterListener(getQualityParamName(i), this);
    state.addParameterListener(getGainParamName(i), this);
    state.addParameterListener(getActiveParamName(i), this);
  }
}

EqualizerProcessor::~EqualizerProcessor()
{
  inputAnalyser.stopThread(1000);
  outputAnalyser.stopThread(1000);
}
//==============================================================================
void EqualizerProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
  sampleRate = newSampleRate;

  for (size_t i = 0; i < bands.size(); ++i)
  {
    updateBand(i);
  }

  updatePlots();

  inputAnalyser.setupAnalyser(int(sampleRate), float(sampleRate));
  outputAnalyser.setupAnalyser(int(sampleRate), float(sampleRate));
}
void EqualizerProcessor::prepare(const dsp::ProcessSpec& spec) { filter.prepare(spec); }

void EqualizerProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{
  inputAnalyser.addAudioData(buffer, 0, getTotalNumInputChannels());

  if (wasBypassed)
  {
    filter.reset();
    wasBypassed = false;
  }
  dsp::AudioBlock<float> ioBuffer(buffer);
  dsp::ProcessContextReplacing<float> context(ioBuffer);
  process(context);

  outputAnalyser.addAudioData(buffer, 0, getTotalNumOutputChannels());
}

void EqualizerProcessor::process(const dsp::ProcessContextReplacing<float>& context) { filter.process(context); }

void EqualizerProcessor::reset() {}

void EqualizerProcessor::parameterChanged(const String& parameter, float newValue)
{
  for (size_t i = 0; i < bands.size(); ++i)
  {
    if (parameter.startsWith(getBandName(int(i)) + "-"))
    {
      if (parameter.endsWith(TA::EqualizerProcessor::paramType))
      {
        bands[i].type = static_cast<FilterType>(static_cast<int>(newValue));
      }
      else if (parameter.endsWith(TA::EqualizerProcessor::paramFrequency))
      {
        bands[i].frequency = newValue;
      }
      else if (parameter.endsWith(TA::EqualizerProcessor::paramQuality))
      {
        bands[i].quality = newValue;
      }
      else if (parameter.endsWith(TA::EqualizerProcessor::paramGain))
      {
        bands[i].gain = newValue;
      }
      else if (parameter.endsWith(TA::EqualizerProcessor::paramActive))
      {
        bands[i].active = newValue >= 0.5f;
      }

      updateBand(i);
      return;
    }
  }
}


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
  if (isPositiveAndBelow(soloed, bands.size()))
  {
    filter.setBypassed<0>(soloed != 0);
    filter.setBypassed<1>(soloed != 1);
    filter.setBypassed<2>(soloed != 2);
    filter.setBypassed<3>(soloed != 3);
    filter.setBypassed<4>(soloed != 4);
    filter.setBypassed<5>(soloed != 5);
  }
  else
  {
    filter.setBypassed<0>(!bands[0].active);
    filter.setBypassed<1>(!bands[1].active);
    filter.setBypassed<2>(!bands[2].active);
    filter.setBypassed<3>(!bands[3].active);
    filter.setBypassed<4>(!bands[4].active);
    filter.setBypassed<5>(!bands[5].active);
  }
  updatePlots();
}

void EqualizerProcessor::updatePlots()
{
  auto gain = 1.0f;
  std::fill(magnitudes.begin(), magnitudes.end(), gain);

  if (isPositiveAndBelow(soloed, bands.size()))
  {
    FloatVectorOperations::multiply(magnitudes.data(), bands[size_t(soloed)].magnitudes.data(),
                                    static_cast<int>(magnitudes.size()));
  }
  else
  {
    for (size_t i = 0; i < bands.size(); ++i)
      if (bands[i].active)
        FloatVectorOperations::multiply(magnitudes.data(), bands[i].magnitudes.data(),
                                        static_cast<int>(magnitudes.size()));
  }

  sendChangeMessage();
}

//==============================================================================
bool EqualizerProcessor::getBandSolo(const int index) const { return index == soloed; }

EqualizerProcessor::Band* EqualizerProcessor::getBand(const int index)
{
  if (isPositiveAndBelow(index, bands.size()))
    return &bands[size_t(index)];
  return nullptr;
}

void EqualizerProcessor::updateBand(const size_t index)
{
  if (sampleRate > 0)
  {
    dsp::IIR::Coefficients<float>::Ptr newCoefficients;
    switch (bands[index].type)
    {
    case TA::EqualizerProcessor::NoFilter:
      newCoefficients = new dsp::IIR::Coefficients<float>(1, 0, 1, 0);
      break;
    case TA::EqualizerProcessor::LowPass:
      newCoefficients =
        dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, bands[index].frequency, bands[index].quality);
      break;
    case TA::EqualizerProcessor::LowPass1st:
      newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(sampleRate, bands[index].frequency);
      break;
    case TA::EqualizerProcessor::LowShelf:
      newCoefficients = dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, bands[index].frequency,
                                                                    bands[index].quality, bands[index].gain);
      break;
    case TA::EqualizerProcessor::BandPass:
      newCoefficients =
        dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, bands[index].frequency, bands[index].quality);
      break;
    case TA::EqualizerProcessor::AllPass:
      newCoefficients =
        dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, bands[index].frequency, bands[index].quality);
      break;
    case TA::EqualizerProcessor::AllPass1st:
      newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderAllPass(sampleRate, bands[index].frequency);
      break;
    case TA::EqualizerProcessor::Notch:
      newCoefficients =
        dsp::IIR::Coefficients<float>::makeNotch(sampleRate, bands[index].frequency, bands[index].quality);
      break;
    case TA::EqualizerProcessor::Peak:
      newCoefficients = dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, bands[index].frequency,
                                                                      bands[index].quality, bands[index].gain);
      break;
    case TA::EqualizerProcessor::HighShelf:
      newCoefficients = dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, bands[index].frequency,
                                                                     bands[index].quality, bands[index].gain);
      break;
    case TA::EqualizerProcessor::HighPass1st:
      newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderHighPass(sampleRate, bands[index].frequency);
      break;
    case TA::EqualizerProcessor::HighPass:
      newCoefficients =
        dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, bands[index].frequency, bands[index].quality);
      break;
    default:
      break;
    }

    if (newCoefficients)
    {
      {
        // minimise lock scope, get<0>() needs to be a  compile time constant
        ScopedLock processLock(getCallbackLock());
        if (index == 0)
          *filter.get<0>().state = *newCoefficients;
        else if (index == 1)
          *filter.get<1>().state = *newCoefficients;
        else if (index == 2)
          *filter.get<2>().state = *newCoefficients;
        else if (index == 3)
          *filter.get<3>().state = *newCoefficients;
        else if (index == 4)
          *filter.get<4>().state = *newCoefficients;
        else if (index == 5)
          *filter.get<5>().state = *newCoefficients;
      }
      newCoefficients->getMagnitudeForFrequencyArray(frequencies.data(), bands[index].magnitudes.data(),
                                                     frequencies.size(), sampleRate);
    }
    updateBypassedStates();
    updatePlots();
  }
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


const std::vector<double>& EqualizerProcessor::getMagnitudes() { return magnitudes; }

void EqualizerProcessor::createFrequencyPlot(Path& p,
                                             const std::vector<double>& mags,
                                             const Rectangle<int> bounds,
                                             float pixelsPerDouble)
{
  p.startNewSubPath(bounds.getX(), roundToInt(bounds.getCentreY() - pixelsPerDouble * std::log(mags[0]) / std::log(2)));
  const double xFactor = static_cast<double>(bounds.getWidth()) / frequencies.size();
  for (size_t i = 1; i < frequencies.size(); ++i)
  {
    p.lineTo(roundToInt(bounds.getX() + i * xFactor),
             roundToInt(bounds.getCentreY() - pixelsPerDouble * std::log(mags[i]) / std::log(2)));
  }
}

void EqualizerProcessor::createAnalyserPlot(Path& p, const Rectangle<int> bounds, float minFreq, bool input)
{
  if (input)
    inputAnalyser.createPath(p, bounds.toFloat(), minFreq);
  else
    outputAnalyser.createPath(p, bounds.toFloat(), minFreq);
}

bool EqualizerProcessor::checkForNewAnalyserData()
{
  return inputAnalyser.checkForNewData() || outputAnalyser.checkForNewData();
}
} // namespace TA