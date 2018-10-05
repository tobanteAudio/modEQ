/* Copyright 2018 Tobias Hienzsch
 *
 * modEQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * modEQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with modEQ.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "../analyser/spectrum_analyser.h"
#include "../utils/text_value_converter.h"
#include "base_processor.h"
namespace TA
{
//==============================================================================
class EqualizerProcessor : public BaseProcessor, public ChangeBroadcaster, AudioProcessorValueTreeState::Listener

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
        float quality   = 1.0f;
        float gain      = 1.0f;
        bool active     = true;
        std::vector<double> magnitudes;
    };

    //==============================================================================
    EqualizerProcessor(AudioProcessorValueTreeState&);
    ~EqualizerProcessor() override;

    //==============================================================================
    void prepareToPlay(double /*unused*/, int /*unused*/) override;
    void prepare(const dsp::ProcessSpec&);

    //==============================================================================
    void process(const dsp::ProcessContextReplacing<float>&);
    void processBlock(AudioBuffer<float>& /*unused*/, MidiBuffer& /*unused*/) override;

    //==============================================================================
    void reset() override;
    void parameterChanged(const String& parameter, float newValue) override;

    //==============================================================================
    static String getFilterTypeName(TA::EqualizerProcessor::FilterType);
    const String getName() const override { return "Equalizer"; }
    Band* getBand(int);

    //==============================================================================
    void updateBand(size_t);
    void updateBypassedStates();
    void updatePlots();

    //==============================================================================
    const std::vector<double>& getMagnitudes();
    void createFrequencyPlot(Path&, const std::vector<double>&, Rectangle<int>, float);
    void createAnalyserPlot(Path&, Rectangle<int>, float, bool);
    bool checkForNewAnalyserData();

    //==============================================================================
    String getTypeParamID(int index) const;
    String getFrequencyParamID(int index) const;
    String getQualityParamID(int index) const;
    String getGainParamID(int index) const;
    String getActiveParamID(int index) const;

    //==============================================================================
    int getNumBands() const;
    String getBandName(int index) const;
    Colour getBandColour(int index) const;
    void setBandSolo(int index);
    bool getBandSolo(int index) const;

private:
    //==============================================================================
    int soloed       = -1;
    bool wasBypassed = true;

    //==============================================================================
    using FilterBand = dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>>;

    dsp::ProcessorChain<FilterBand, FilterBand, FilterBand, FilterBand, FilterBand, FilterBand> filter;
    std::vector<Band> bands;

    std::vector<double> frequencies;
    std::vector<double> magnitudes;

    TA::SpectrumAnalyser<float> inputAnalyser;
    TA::SpectrumAnalyser<float> outputAnalyser;

    GainTextConverter gainTextConverter;
    ActiveTextConverter activeTextConverter;
    QualityTextConverter qualityTextConverter;
    FrequencyTextConverter frequencyTextConverter;

    void setDefaults();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerProcessor)
};

}  // namespace TA
