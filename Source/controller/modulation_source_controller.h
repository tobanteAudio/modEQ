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
#include "../processor/modulation_source_processor.h"
#include "../view/modulation_source_view.h"

namespace tobanteAudio
{
/**
 * @brief Controller for the ModulationSourceView component.
 */
class ModulationSourceController : public Button::Listener, public Slider::Listener, public Timer
{
public:
    /**
     * @brief Constructor.
     */
    ModulationSourceController(int, ModEQProcessor&, tobanteAudio::ModulationSourceProcessor&,
                               tobanteAudio::ModulationSourceView&);

    /**
     * @brief Listens to button clicks from view.
     */
    void buttonClicked(Button* b) override;

    /**
     * @brief Listens to slider changes from view.
     */
    void sliderValueChanged(Slider* slider) override;

    /**
     * @brief Refreshes the modulation source plot.
     */
    void timerCallback() override;

private:
    int index;
    bool connectViewActive;

    // Processor & View connections
    ModEQProcessor& mainProcessor;
    tobanteAudio::ModulationSourceProcessor& processor;
    tobanteAudio::ModulationSourceView& view;

    // Attachments to ValueTree
    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceController)
};

}  // namespace tobanteAudio
