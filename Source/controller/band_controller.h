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
// JUCE
#include "../../JuceLibraryCode/JuceHeader.h"

// tobanteAudio
#include "../modEQ_processor.h"
#include "../view/band_view.h"

//==============================================================================
namespace tobanteAudio
{
class BandController : public Button::Listener
{
public:
    //==============================================================================
    BandController(int /*i*/, ModEQProcessor& /*p*/,
                   tobanteAudio::EqualizerProcessor& /*sub*/,
                   tobanteAudio::BandView& /*v*/);

    //==============================================================================
    void buttonClicked(Button* b) override;

    //==============================================================================
    void updateControls(tobanteAudio::EqualizerProcessor::FilterType type);
    void updateSoloState(bool isSolo);

    //==============================================================================
    void setFrequency(float newFreq);
    void setGain(float newGain);
    void setType(int newType);

    //==============================================================================
    Path frequencyResponse;

private:
    //==============================================================================
    int index;
    tobanteAudio::BandView& view;
    ModEQProcessor& mainProcessor;
    tobanteAudio::EqualizerProcessor& processor;

    //==============================================================================
    OwnedArray<AudioProcessorValueTreeState::ComboBoxAttachment> boxAttachments;
    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
    OwnedArray<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandController)
};

}  // namespace tobanteAudio