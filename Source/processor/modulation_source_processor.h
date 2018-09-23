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

#include "../analyser/modulation_source_analyser.h"
#include "../utils/text_value_converter.h"
#include "base_processor.h"

namespace TA {
//==============================================================================
class ModulationSourceProcessor
  : public BaseProcessor
  , AudioProcessorValueTreeState::Listener

{
public:
  //==============================================================================
  ModulationSourceProcessor(AudioProcessorValueTreeState &);
  ~ModulationSourceProcessor() override;

  //==============================================================================
  void prepareToPlay(double /*unused*/, int /*unused*/) override;
  void processBlock(AudioBuffer<float> & /*unused*/, MidiBuffer & /*unused*/) override;

  //==============================================================================
  void parameterChanged(const String &parameter, float newValue) override;

  //==============================================================================
  void createAnalyserPlot(Path &, Rectangle<int> &, float);
  bool checkForNewAnalyserData();

  //==============================================================================
  void reset() override { oscillator.reset(); }

private:
  //==============================================================================
  int index;
  dsp::Oscillator<float> oscillator;
  TA::ModulationSourceAnalyser<float> analyser;
  FrequencyTextConverter frequencyTextConverter;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceProcessor)
};

}// namespace TA