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

#include "modulation_source_processor.h"

namespace tobanteAudio
{
ModulationSourceProcessor::ModulationSourceProcessor(int i, AudioProcessorValueTreeState& vts)
    : index(i)
    , BaseProcessor(vts)
    , paramIDGain("lfo_" + String(index) + "_gain")
    , paramIDFrequency("lfo_" + String(index) + "_freq")
{
    oscillator.setFrequency(1.f);
    oscillator.initialise([](float x) { return std::sin(x); });
}

ModulationSourceProcessor::~ModulationSourceProcessor() { analyser.stopThread(1000); }

void ModulationSourceProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;

    dsp::ProcessSpec spec{sampleRate, static_cast<uint32>(samplesPerBlock)};
    oscillator.prepare(spec);
    gain.prepare(spec);

    auto freqValue = *state.getRawParameterValue(paramIDFrequency);
    oscillator.setFrequency(freqValue);

    analyser.setupAnalyser(int(sampleRate), float(sampleRate));
}

void ModulationSourceProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
    float freqValue = *state.getRawParameterValue(paramIDFrequency);
    float gainValue = *state.getRawParameterValue(paramIDGain);

    oscillator.setFrequency(freqValue);
    gain.setGainLinear(gainValue);

    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);
    oscillator.process(context);
    gain.process(context);

    analyser.addAudioData(buffer, 0, 1);
}

void ModulationSourceProcessor::parameterChanged(const String& /*parameter*/, float /*newValue*/) {}

void ModulationSourceProcessor::createAnalyserPlot(Path& p, Rectangle<int>& bounds, float minFreq)
{
    analyser.createPath(p, bounds.toFloat(), minFreq);
}

bool ModulationSourceProcessor::checkForNewAnalyserData() { return analyser.checkForNewData(); }

}  // namespace tobanteAudio