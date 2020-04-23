/* Copyright 2018-2020 Tobias Hienzsch
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

// JUCE
#include "modEQ.hpp"

// tobanteAudio
#include "analyser/spectrum_analyser.h"
#include "parameters/text_value_converter.h"
#include "processor/equalizer_processor.h"
#include "processor/modulation_source_processor.h"

/**
 * @brief Entry point for processor thread. Inherites from juce::AudioProcessor
 */
class ModEQProcessor : public juce::AudioProcessor,
                       public juce::AudioProcessorValueTreeState::Listener,
                       public juce::ChangeBroadcaster
{
private:
    juce::UndoManager undo;
    juce::AudioProcessorValueTreeState state;

public:
    ModEQProcessor();
    ~ModEQProcessor() override = default;

    void prepareToPlay(double newSampleRate, int newSamplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>& /*buffer*/, juce::MidiBuffer& /*midiMessages*/) override;
    void parameterChanged(const String& parameter, float newValue) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    tobanteAudio::EqualizerProcessor& getEQ() { return equalizerProcessor; }
    juce::AudioProcessorValueTreeState& getPluginState() { return state; }
    juce::UndoManager& getUndoManager() { return undo; }
    tobanteAudio::ModulationSourceProcessor modSource;
    FFAU::LevelMeterSource* getMeterSource() { return &meterSource; }

private:
    double sampleRate = 0;

    tobanteAudio::EqualizerProcessor equalizerProcessor;
    juce::AudioBuffer<float> modBuffer;

    juce::dsp::Gain<float> outputGain;
    FFAU::LevelMeterSource meterSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModEQProcessor)
};
