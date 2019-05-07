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

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
namespace tobanteAudio
{
//==============================================================================
class BaseProcessor : public AudioProcessor
{
public:
    //==============================================================================
    BaseProcessor(AudioProcessorValueTreeState& vts) : state(vts) {}
    ~BaseProcessor() override = default;

    //==============================================================================
    void prepareToPlay(double /*sampleRate*/,
                       int /*maximumExpectedSamplesPerBlock*/) override
    {
    }
    void releaseResources() override {}
    void processBlock(AudioBuffer<float>& /*buffer*/,
                      MidiBuffer& /*midiMessages*/) override
    {
    }

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
    void setCurrentProgram(int /*index*/) override {}
    const String getProgramName(int /*index*/) override { return {}; }
    void changeProgramName(int /*index*/, const String& /*newName*/) override {}

    //==============================================================================
    void getStateInformation(MemoryBlock& /*destData*/) override {}
    void setStateInformation(const void* /*data*/, int /*sizeInBytes*/) override {}

    //==============================================================================
    AudioProcessorValueTreeState& state;

    double sampleRate{0};

protected:
private:
    //==============================================================================

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseProcessor)
};

}  // namespace tobanteAudio