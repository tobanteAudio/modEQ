/* Copyright 2018 Tobias Hienzsch
 *
 * modEQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
namespace TA
{

//==============================================================================
class BaseProcessor : public AudioProcessor
{
public:
  //==============================================================================
  BaseProcessor(AudioProcessorValueTreeState& vts): state(vts) {}
  ~BaseProcessor() {}

  //==============================================================================
  void prepareToPlay(double, int) override {}
  void releaseResources() override {}
  void processBlock(AudioBuffer<float>&, MidiBuffer&) override {}

  //==============================================================================
  AudioProcessorEditor* createEditor() override { return nullptr; }
  bool hasEditor() const override { return false; }

  //==============================================================================
  const String getName() const override { return {}; }
  bool acceptsMidi() const override { return false; }
  bool producesMidi() const override { return false; }
  double getTailLengthSeconds() const override { return 0; }

  //==============================================================================
  int getNumPrograms() override { return 0; }
  int getCurrentProgram() override { return 0; }
  void setCurrentProgram(int) override {}
  const String getProgramName(int) override { return {}; }
  void changeProgramName(int, const String&) override {}

  //==============================================================================
  void getStateInformation(MemoryBlock&) override {}
  void setStateInformation(const void*, int) override {}

  //==============================================================================
  AudioProcessorValueTreeState& getPluginState() { return state; }
  AudioProcessorValueTreeState& state;

  
  double sampleRate;

protected:


private:
  //==============================================================================
  

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseProcessor)
};


} // namespace TA