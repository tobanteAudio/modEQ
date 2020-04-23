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
#include "../modEQ_processor.h"
#include "../view/band_view.h"

namespace tobanteAudio
{
/**
 * @brief Controller for the BandView component.
 */
class BandController
{
public:
    /**
     * @brief Constructor.
     */
    BandController(int /*i*/, ModEQProcessor& /*p*/, tobanteAudio::EqualizerProcessor& /*sub*/,
                   tobanteAudio::BandView& /*v*/);

    /**
     * @brief Activates or deactivates UI controls depending on the selected
     * filter type.
     */
    void setUIControls(tobanteAudio::EqualizerProcessor::FilterType type);

    /**
     * @brief Set solo in view.
     */
    void setSolo(bool isSolo);

    /**
     * @brief Set frequency in view.
     */
    void setFrequency(float newFreq);

    /**
     * @brief Set gain in view.
     */
    void setGain(float newGain);

    /**
     * @brief Set filter type in view.
     */
    void setType(int newType);

    /**
     * @brief Filter frequency response.
     */
    Path frequencyResponse;

private:
    int index;
    tobanteAudio::BandView& view;
    ModEQProcessor& mainProcessor;
    tobanteAudio::EqualizerProcessor& processor;

    // ATTACHMENTS
    OwnedArray<AudioProcessorValueTreeState::ComboBoxAttachment> boxAttachments;
    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
    OwnedArray<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandController)
};

}  // namespace tobanteAudio