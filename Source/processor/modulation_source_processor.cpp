/*
  ==============================================================================

    modulation_source_processor.cpp
    Created: 12 Sep 2018 8:41:46pm
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#include "modulation_source_processor.h"

namespace TA
{

ModulationSourceProcessor::ModulationSourceProcessor(AudioProcessorValueTreeState& vts)
  : ProcessorBase(vts)
{
  oscillator.setFrequency(0.08f);
  oscillator.initialise([](float x) { return std::sin(x); });
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

} // namespace TA