/* Copyright 2018-2019 Tobias Hienzsch
 *
 * modEQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * modEQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with modEQ. If not, see <http://www.gnu.org/licenses/>.
 */

#include "equalizer_processor.h"
#include "../utils/parameters.h"

//==============================================================================
namespace tobanteAudio
{
//==============================================================================
EqualizerProcessor::EqualizerProcessor(AudioProcessorValueTreeState& vts)
    : BaseProcessor(vts)
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

    setDefaults();

    // Create Ranges for parameters
    NormalisableRange<float> filterTypeRange(
        0, tobanteAudio::EqualizerProcessor::LastFilterID, 1);
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

        state.createAndAddParameter(getFrequencyParamID(i), band.name + " freq",
                                    "Frequency", frequencyRange, band.frequency,
                                    frequencyTextConverter, frequencyTextConverter, false,
                                    true, false);
        state.createAndAddParameter(
            getQualityParamID(i), band.name + " Q", translate("Quality"), qualityRange,
            band.quality, qualityTextConverter, qualityTextConverter, false, true, false);
        state.createAndAddParameter(
            getGainParamID(i), band.name + " gain", translate("Gain"), gainRange,
            band.gain, gainTextConverter, gainTextConverter, false, true, false);
        state.createAndAddParameter(
            getActiveParamID(i), band.name + " active", translate("Active"), activeRange,
            band.active, activeTextConverter, activeTextConverter, false, true, true);

        state.createAndAddParameter(
            getTypeParamID(i), band.name + " Type", translate("Filter Type"),
            filterTypeRange, (float)band.type,
            [](float value) {
                return tobanteAudio::EqualizerProcessor::getFilterTypeName(
                    static_cast<tobanteAudio::EqualizerProcessor::FilterType>(
                        static_cast<int>(value)));
            },
            [](String text) {
                for (int i = 0; i < tobanteAudio::EqualizerProcessor::LastFilterID; ++i)
                    if (text
                        == tobanteAudio::EqualizerProcessor::getFilterTypeName(
                            static_cast<tobanteAudio::EqualizerProcessor::FilterType>(i)))
                        return static_cast<tobanteAudio::EqualizerProcessor::FilterType>(
                            i);
                return tobanteAudio::EqualizerProcessor::NoFilter;
            },
            false, true, true);

        state.addParameterListener(getTypeParamID(i), this);
        state.addParameterListener(getFrequencyParamID(i), this);
        state.addParameterListener(getQualityParamID(i), this);
        state.addParameterListener(getGainParamID(i), this);
        state.addParameterListener(getActiveParamID(i), this);
    }
}

EqualizerProcessor::~EqualizerProcessor()
{
    inputAnalyser.stopThread(1000);
    outputAnalyser.stopThread(1000);
}
//==============================================================================
void EqualizerProcessor::prepareToPlay(double newSampleRate, int /*samplesPerBlock*/)
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

void EqualizerProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
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

void EqualizerProcessor::process(const dsp::ProcessContextReplacing<float>& context)
{
    filter.process(context);
}

void EqualizerProcessor::reset() {}

void EqualizerProcessor::parameterChanged(const String& parameter, float newValue)
{
    for (size_t i = 0; i < bands.size(); ++i)
    {
        if (parameter.startsWith(getBandName(int(i)) + "-"))
        {
            if (parameter.endsWith(tobanteAudio::Parameters::Type))
            {
                bands[i].type = static_cast<FilterType>(static_cast<int>(newValue));
            }
            else if (parameter.endsWith(tobanteAudio::Parameters::Frequency))
            {
                bands[i].frequency = newValue;
            }
            else if (parameter.endsWith(tobanteAudio::Parameters::Quality))
            {
                bands[i].quality = newValue;
            }
            else if (parameter.endsWith(tobanteAudio::Parameters::Gain))
            {
                bands[i].gain = newValue;
            }
            else if (parameter.endsWith(tobanteAudio::Parameters::Active))
            {
                bands[i].active = newValue >= 0.5f;
            }

            updateBand(i);
            return;
        }
    }
}

//==============================================================================
String EqualizerProcessor::getFilterTypeName(
    const tobanteAudio::EqualizerProcessor::FilterType type)
{
    switch (type)
    {
    case NoFilter:
        return translate("No Filter");
    case HighPass:
        return translate("High Pass");
    case HighPass1st:
        return translate("1st High Pass");
    case LowShelf:
        return translate("Low Shelf");
    case BandPass:
        return translate("Band Pass");
    case AllPass:
        return translate("All Pass");
    case AllPass1st:
        return translate("1st All Pass");
    case Notch:
        return translate("Notch");
    case Peak:
        return translate("Peak");
    case HighShelf:
        return translate("High Shelf");
    case LowPass1st:
        return translate("1st Low Pass");
    case LowPass:
        return translate("Low Pass");
    default:
        return translate("unknown");
    }
}

int EqualizerProcessor::getNumBands() const { return static_cast<int>(bands.size()); }

String EqualizerProcessor::getBandName(const int index) const
{
    if (isPositiveAndBelow(index, bands.size())) return bands[size_t(index)].name;
    return translate("unknown");
}
Colour EqualizerProcessor::getBandColour(const int index) const
{
    if (isPositiveAndBelow(index, bands.size())) return bands[size_t(index)].colour;
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
        FloatVectorOperations::multiply(magnitudes.data(),
                                        bands[size_t(soloed)].magnitudes.data(),
                                        static_cast<int>(magnitudes.size()));
    }
    else
    {
        for (size_t i = 0; i < bands.size(); ++i)
            if (bands[i].active)
                FloatVectorOperations::multiply(magnitudes.data(),
                                                bands[i].magnitudes.data(),
                                                static_cast<int>(magnitudes.size()));
    }

    sendChangeMessage();
}

//==============================================================================
bool EqualizerProcessor::getBandSolo(const int index) const { return index == soloed; }

void EqualizerProcessor::setDefaults()
{
    // setting defaults
    {
        auto& band     = bands[0];
        band.name      = "Lowest";
        band.colour    = Colours::blue;
        band.frequency = 20.0f;
        band.quality   = 0.707f;
        band.type      = tobanteAudio::EqualizerProcessor::HighPass;
    }
    {
        auto& band     = bands[1];
        band.name      = "Low";
        band.colour    = Colours::brown;
        band.frequency = 250.0f;
        band.type      = tobanteAudio::EqualizerProcessor::LowShelf;
    }
    {
        auto& band     = bands[2];
        band.name      = "Low Mids";
        band.colour    = Colours::green;
        band.frequency = 500.0f;
        band.type      = tobanteAudio::EqualizerProcessor::Peak;
    }
    {
        auto& band     = bands[3];
        band.name      = "High Mids";
        band.colour    = Colours::coral;
        band.frequency = 1000.0f;
        band.type      = tobanteAudio::EqualizerProcessor::Peak;
    }
    {
        auto& band     = bands[4];
        band.name      = "High";
        band.colour    = Colours::orange;
        band.frequency = 5000.0f;
        band.type      = tobanteAudio::EqualizerProcessor::HighShelf;
    }
    {
        auto& band     = bands[5];
        band.name      = "Highest";
        band.colour    = Colours::red;
        band.frequency = 12000.0f;
        band.quality   = 0.707f;
        band.type      = tobanteAudio::EqualizerProcessor::LowPass;
    }
}

EqualizerProcessor::Band* EqualizerProcessor::getBand(const int index)
{
    if (isPositiveAndBelow(index, bands.size())) return &bands[size_t(index)];
    return nullptr;
}

void EqualizerProcessor::updateBand(const size_t index)
{
    if (sampleRate > 0)
    {
        dsp::IIR::Coefficients<float>::Ptr newCoefficients;
        switch (bands[index].type)
        {
        case tobanteAudio::EqualizerProcessor::NoFilter:
            newCoefficients = new dsp::IIR::Coefficients<float>(1, 0, 1, 0);
            break;
        case tobanteAudio::EqualizerProcessor::LowPass:
            newCoefficients = dsp::IIR::Coefficients<float>::makeLowPass(
                sampleRate, bands[index].frequency, bands[index].quality);
            break;
        case tobanteAudio::EqualizerProcessor::LowPass1st:
            newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(
                sampleRate, bands[index].frequency);
            break;
        case tobanteAudio::EqualizerProcessor::LowShelf:
            newCoefficients = dsp::IIR::Coefficients<float>::makeLowShelf(
                sampleRate, bands[index].frequency, bands[index].quality,
                bands[index].gain);
            break;
        case tobanteAudio::EqualizerProcessor::BandPass:
            newCoefficients = dsp::IIR::Coefficients<float>::makeBandPass(
                sampleRate, bands[index].frequency, bands[index].quality);
            break;
        case tobanteAudio::EqualizerProcessor::AllPass:
            newCoefficients = dsp::IIR::Coefficients<float>::makeAllPass(
                sampleRate, bands[index].frequency, bands[index].quality);
            break;
        case tobanteAudio::EqualizerProcessor::AllPass1st:
            newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderAllPass(
                sampleRate, bands[index].frequency);
            break;
        case tobanteAudio::EqualizerProcessor::Notch:
            newCoefficients = dsp::IIR::Coefficients<float>::makeNotch(
                sampleRate, bands[index].frequency, bands[index].quality);
            break;
        case tobanteAudio::EqualizerProcessor::Peak:
            newCoefficients = dsp::IIR::Coefficients<float>::makePeakFilter(
                sampleRate, bands[index].frequency, bands[index].quality,
                bands[index].gain);
            break;
        case tobanteAudio::EqualizerProcessor::HighShelf:
            newCoefficients = dsp::IIR::Coefficients<float>::makeHighShelf(
                sampleRate, bands[index].frequency, bands[index].quality,
                bands[index].gain);
            break;
        case tobanteAudio::EqualizerProcessor::HighPass1st:
            newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderHighPass(
                sampleRate, bands[index].frequency);
            break;
        case tobanteAudio::EqualizerProcessor::HighPass:
            newCoefficients = dsp::IIR::Coefficients<float>::makeHighPass(
                sampleRate, bands[index].frequency, bands[index].quality);
            break;
        default:
            break;
        }

        if (newCoefficients)
        {
            {
                // minimise lock scope, get<0>() needs to be a  compile time
                // constant
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
            newCoefficients->getMagnitudeForFrequencyArray(
                frequencies.data(), bands[index].magnitudes.data(), frequencies.size(),
                sampleRate);
        }
        updateBypassedStates();
        updatePlots();
    }
}

String EqualizerProcessor::getTypeParamID(const int index) const
{
    return getBandName(index) + "-" + tobanteAudio::Parameters::Type;
}

String EqualizerProcessor::getFrequencyParamID(const int index) const
{
    return getBandName(index) + "-" + tobanteAudio::Parameters::Frequency;
}

String EqualizerProcessor::getQualityParamID(const int index) const
{
    return getBandName(index) + "-" + tobanteAudio::Parameters::Quality;
}

String EqualizerProcessor::getGainParamID(const int index) const
{
    return getBandName(index) + "-" + tobanteAudio::Parameters::Gain;
}

String EqualizerProcessor::getActiveParamID(const int index) const
{
    return getBandName(index) + "-" + tobanteAudio::Parameters::Active;
}

const std::vector<double>& EqualizerProcessor::getMagnitudes() { return magnitudes; }

void EqualizerProcessor::createFrequencyPlot(Path& p, const std::vector<double>& mags,
                                             const Rectangle<int> bounds,
                                             float pixelsPerDouble)
{
    p.startNewSubPath(
        static_cast<float>(bounds.getX()),
        static_cast<float>(roundToInt(
            bounds.getCentreY() - pixelsPerDouble * std::log(mags[0]) / std::log(2))));
    const double xFactor = static_cast<double>(bounds.getWidth()) / frequencies.size();
    for (size_t i = 1; i < frequencies.size(); ++i)
    {
        p.lineTo(roundToInt(bounds.getX() + i * xFactor),
                 roundToInt(bounds.getCentreY()
                            - pixelsPerDouble * std::log(mags[i]) / std::log(2)));
    }
}

void EqualizerProcessor::createAnalyserPlot(Path& p, const Rectangle<int> bounds,
                                            float minFreq, bool input)
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
}  // namespace tobanteAudio