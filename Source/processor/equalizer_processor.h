/* Copyrig / text_value_converter.hnzsch
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

#pragma once
#include "../analyser/spectrum_analyser.h"
#include "../parameters/text_value_converter.h"
#include "base_processor.h"
namespace tobanteAudio
{
/**
 * @brief Main processor class for modEQ. Holds 6 JUCE dsp filters in a
 * ProcessorChain.
 */
class EqualizerProcessor : public BaseProcessor,
                           public ChangeBroadcaster,
                           AudioProcessorValueTreeState::Listener

{
public:
    /**
     * @brief Filter types for an individual band.
     */
    enum FilterType
    {
        NoFilter = 0,
        HighPass,
        LowShelf,
        BandPass,
        Peak,
        HighShelf,
        LowPass,
        LastFilterID
    };

    /**
     * @brief Model of a filter band.
     */
    struct Band
    {
        String name;
        Colour colour;
        FilterType type = BandPass;
        float frequency = 1000.0f;
        float quality   = 1.0f;
        float gain      = 1.0f;
        bool active     = true;
        bool selected   = false;
        std::vector<double> magnitudes;
    };

    /**
     * @brief Constructor.
     */
    EqualizerProcessor(AudioProcessorValueTreeState& vts);

    /**
     * @brief Destructor. Stops the anaylser threads.
     */
    ~EqualizerProcessor() override;

    /**
     * @brief Prepare dsp with samplerate & outputSliderFrame size.
     */
    void prepareToPlay(double /*unused*/, int /*unused*/) override;

    /**
     * @brief Prepare dsp with dsp::ProcessSpec from JUCE.
     */
    void prepare(const dsp::ProcessSpec& spec);

    /**
     * @brief Process block with dsp::ProcessContextReplacing from JUCE.
     */
    void process(const dsp::ProcessContextReplacing<float>& context);

    /**
     * @brief Process audio & midi buffers.
     */
    void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midi) override;

    /**
     * @brief Updates the dsp model if a parameter was changed.
     */
    void parameterChanged(const String& parameter, float newValue) override;

    /**
     * @brief Converts filter type enum class to string value.
     */
    static String getFilterTypeName(tobanteAudio::EqualizerProcessor::FilterType type);

    /**
     * @brief Returns the processor name.
     */
    const String getName() const override { return "Equalizer"; }

    /**
     * @brief Returns a single band by index, with bounds checking.
     */
    Band* getBand(int index);

    /**
     * @brief Updates the filter coefficients for a band by index.
     */
    void updateBand(size_t index);

    /**
     * @brief Updates the bands bypass state.
     */
    void updateBypassedStates();

    /**
     * @brief Updates the bands frequency response plots.
     */
    void updatePlots();

    /**
     * @brief Returns refrence to the magnitudes vector.
     */
    const std::vector<double>& getMagnitudes();

    /**
     * @brief Daws the frequency response plot to a given path & area.
     */
    void createFrequencyPlot(Path& p, const std::vector<double>& mags,
                             Rectangle<int> bounds, float pixelsPerDouble);

    /**
     * @brief Draws the analyser plot to a given path & area.
     */
    void createAnalyserPlot(Path& p, Rectangle<int> bounds, float minFreq, bool input);

    /**
     * @brief Returns true if either the input or output analyser have new data.
     */
    bool checkForNewAnalyserData();

    /**
     * @brief Returns the filter type ValueTree parameter string for a band by
     * index.
     */
    String getTypeParamID(int index) const;

    /**
     * @brief Returns the frequency ValueTree parameter string for a band by
     * index.
     */
    String getFrequencyParamID(int index) const;

    /**
     * @brief Returns the quality ValueTree parameter string for a band by
     * index.
     */
    String getQualityParamID(int index) const;

    /**
     * @brief Returns the gain ValueTree parameter string for a band by index.
     */
    String getGainParamID(int index) const;

    /**
     * @brief Returns the active/bypass ValueTree parameter string for a band by
     * index.
     */
    String getActiveParamID(int index) const;

    /**
     * @brief Returns the number of bands in the processor chain.
     */
    int getNumBands() const;

    /**
     * @brief Returns the bands name by index. Returns "unknown" if out of
     * bounds.
     */
    String getBandName(int index) const;

    /**
     * @brief Returns the bands color by index. Returns Colours::silver if out
     * of bounds.
     */
    Colour getBandColour(int index) const;

    /**
     * @brief Sets global solo band.
     */
    void setBandSolo(int index);

    /**
     * @brief Returns true if the passed in index is the current global solo
     * selected.
     */
    bool getBandSolo(int index) const;

    /**
     * @brief Set the selected band. (clicked or dragged)
     */
    void setSelectedBand(int index);

    /**
     * @brief Returns the current selected band.
     */
    int getSelectedBand();

private:
    int soloed       = -1;
    bool wasBypassed = true;

    using FloatFilter       = dsp::IIR::Filter<float>;
    using FloatCoefficients = dsp::IIR::Coefficients<float>;
    using FBand             = dsp::ProcessorDuplicator<FloatFilter, FloatCoefficients>;

    dsp::ProcessorChain<FBand, FBand, FBand, FBand, FBand, FBand> filter;
    std::vector<Band> bands;

    std::vector<double> frequencies;
    std::vector<double> magnitudes;

    tobanteAudio::SpectrumAnalyser<float> inputAnalyser;
    tobanteAudio::SpectrumAnalyser<float> outputAnalyser;

    tobanteAudio::GainTextConverter gainTextConverter;
    tobanteAudio::ActiveTextConverter activeTextConverter;
    tobanteAudio::QualityTextConverter qualityTextConverter;
    tobanteAudio::FrequencyTextConverter frequencyTextConverter;
    tobanteAudio::FilterTypeTextConverter filterTypeTextConverter;

    void setDefaults();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerProcessor)
};

}  // namespace tobanteAudio
