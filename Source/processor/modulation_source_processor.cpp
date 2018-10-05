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

namespace TA
{
ModulationSourceProcessor::ModulationSourceProcessor(AudioProcessorValueTreeState& vts)
    : index(1), BaseProcessor(vts)
{
    oscillator.setFrequency(1.f);
    oscillator.initialise([](float x) { return std::sin(x); });

    auto lfoRange = NormalisableRange<float>(0.0, 15.0, 0.01);
}

ModulationSourceProcessor::~ModulationSourceProcessor() { analyser.stopThread(1000); }

void ModulationSourceProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;

    dsp::ProcessSpec spec{sampleRate, static_cast<uint32>(samplesPerBlock)};
    oscillator.prepare(spec);

    analyser.setupAnalyser(int(sampleRate), float(sampleRate));
}

void ModulationSourceProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
    auto freqValue = *state.getRawParameterValue("lfo_freq");

	oscillator.setFrequency(freqValue);

    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);
    oscillator.process(context);

    analyser.addAudioData(buffer, 0, 1);
}

void ModulationSourceProcessor::parameterChanged(const String& /*parameter*/, float /*newValue*/) {}

void ModulationSourceProcessor::createAnalyserPlot(Path& p, Rectangle<int>& bounds, float minFreq)
{
    analyser.createPath(p, bounds.toFloat(), minFreq);
}

bool ModulationSourceProcessor::checkForNewAnalyserData() { return analyser.checkForNewData(); }

}  // namespace TA