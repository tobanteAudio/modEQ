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

#include "modEQ_processor.h"
#include "modEQ_editor.h"
#include "utils/constants.h"
#include "utils/parameters.h"
#include "view/social_buttons.h"

#include "../Tests/test_main.h"

ModEQProcessor::ModEQProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
                         .withInput("Input", AudioChannelSet::stereo(), true)
                         .withOutput("Output", AudioChannelSet::stereo(), true))
    ,
#else
    :
#endif
    state(*this, &undo)
    , modSource(1, state)
    , equalizerProcessor(state)

{
    using tobanteAudio::GAIN_DEFAULT;
    using tobanteAudio::GAIN_MAX;
    using tobanteAudio::GAIN_MIN;
    using tobanteAudio::GAIN_STEP_SIZE;

    using tobanteAudio::LFO_FREQ_DEFAULT;
    using tobanteAudio::LFO_FREQ_MAX;
    using tobanteAudio::LFO_FREQ_MIN;
    using tobanteAudio::LFO_FREQ_SKEW;
    using tobanteAudio::LFO_FREQ_STEP_SIZE;
    using tobanteAudio::LFO_GAIN_MAX;

    using Parameter = AudioProcessorValueTreeState::Parameter;

    auto const gainRange = NormalisableRange<float>(GAIN_MIN, GAIN_MAX, GAIN_STEP_SIZE);
    auto const lfoGainRange
        = NormalisableRange<float>(GAIN_MIN, LFO_GAIN_MAX, GAIN_STEP_SIZE);
    auto const lfoFreqRange = []() -> NormalisableRange<float> {
        auto range
            = NormalisableRange<float>(LFO_FREQ_MIN, LFO_FREQ_MAX, LFO_FREQ_STEP_SIZE);
        range.setSkewForCentre(LFO_FREQ_SKEW);
        return range;
    }();

    state.createAndAddParameter(std::make_unique<Parameter>(
        tobanteAudio::Parameters::Output, translate("Output"), translate("Output level"),
        gainRange, GAIN_DEFAULT, gainTextConverter, gainTextConverter, false, true,
        false));

    state.createAndAddParameter(std::make_unique<Parameter>(
        "lfo_1_freq", translate("lfo freq"), translate("lfo freq"), lfoFreqRange,
        LFO_FREQ_DEFAULT, freqTextConverter, freqTextConverter, false, true, false));

    state.createAndAddParameter(std::make_unique<Parameter>(
        "lfo_1_gain", translate("lfo gain"), translate("lfo gain"), lfoGainRange,
        GAIN_DEFAULT, gainTextConverter, gainTextConverter, false, true, false));

    state.addParameterListener(tobanteAudio::Parameters::Output, this);

    state.state = ValueTree(JucePlugin_Name);

#ifdef JUCE_DEBUG
    tobanteAudio::tests::run();
#else
    // in release mode, default to not running tests.

#endif
}

const String ModEQProcessor::getName() const { return JucePlugin_Name; }

bool ModEQProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ModEQProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ModEQProcessor::isMidiEffect() const { return false; }
double ModEQProcessor::getTailLengthSeconds() const { return 0.0; }
int ModEQProcessor::getNumPrograms() { return 1; }
int ModEQProcessor::getCurrentProgram() { return 0; }
void ModEQProcessor::setCurrentProgram(int index) { ignoreUnused(index); }
const String ModEQProcessor::getProgramName(int index)
{
    ignoreUnused(index);
    return {};
}
void ModEQProcessor::changeProgramName(int index, const String& newName)
{
    ignoreUnused(index);
    ignoreUnused(newName);
}

void ModEQProcessor::prepareToPlay(double newSampleRate, int newSamplesPerBlock)
{
    sampleRate = newSampleRate;
    modBuffer.setSize(1, newSamplesPerBlock, false, false, true);

    dsp::ProcessSpec spec;
    spec.sampleRate       = newSampleRate;
    spec.maximumBlockSize = uint32(newSamplesPerBlock);
    spec.numChannels      = uint32(getTotalNumOutputChannels());

    modSource.prepareToPlay(sampleRate, newSamplesPerBlock);
    equalizerProcessor.prepareToPlay(newSampleRate, newSamplesPerBlock);
    equalizerProcessor.prepare(spec);
    outputGain.prepare(spec);

    outputGain.setGainLinear(
        *state.getRawParameterValue(tobanteAudio::Parameters::Output));
}

void ModEQProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ModEQProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // This checks if the input layout matches the output layout
    return layouts.getMainOutputChannelSet() == layouts.getMainInputChannelSet();
}
#endif

void ModEQProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ignoreUnused(midiMessages);
    ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    // modBuffer.clear();
    // modSource.processBlock(modBuffer, midiMessages);

    // auto modRange  = .8f;
    // auto modValue  = modBuffer.getSample(0,
    // static_cast<int>(modBuffer.getNumSamples() / 2)); auto gainValue =
    // *state.getRawParameterValue(tobanteAudio::Parameters::Output); auto
    // gainMod   = gainValue + (modRange * modValue);

    // if (gainMod < -0.0f)
    //{
    //    gainMod = 0.0;
    //}
    // if (gainMod > 2.0f)
    //{
    //    gainMod = 2.0;
    //}

    // outputGain.setGainLinear(gainMod);

    equalizerProcessor.processBlock(buffer, midiMessages);

    dsp::AudioBlock<float> ioBuffer(buffer);
    dsp::ProcessContextReplacing<float> context(ioBuffer);

    outputGain.process(context);

    meterSource.measureBlock(buffer);
}

void ModEQProcessor::parameterChanged(const String& parameter, float newValue)
{
    if (parameter == tobanteAudio::Parameters::Output)
    {
        outputGain.setGainLinear(newValue);
        return;
    }
}

bool ModEQProcessor::hasEditor() const { return true; }

AudioProcessorEditor* ModEQProcessor::createEditor() { return new ModEQEditor(*this); }

void ModEQProcessor::getStateInformation(MemoryBlock& destData)
{
    MemoryOutputStream stream(destData, false);
    state.state.writeToStream(stream);
}

void ModEQProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    ValueTree tree = ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));
    if (tree.isValid())
    {
        state.state = tree;
    }
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ModEQProcessor(); }
