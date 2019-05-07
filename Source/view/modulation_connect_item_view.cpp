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

// JUCE
#include "../../JuceLibraryCode/JuceHeader.h"

// tobanteAudio
#include "modulation_connect_item_view.h"

namespace tobanteAudio
{
//==============================================================================
ModulationConnectItemView::ModulationConnectItemView(int i)
    : index(i), active(translate("A")), amount(Slider::LinearHorizontal, Slider::NoTextBox)
{
    // Toogle Button
    addAndMakeVisible(active);

    // Slider
    amount.setRange(-1.0, 1.0, 0.0);
    addAndMakeVisible(amount);

    // Label
    target.setJustificationType(Justification::centred);
    target.setText("Target: " + String(index), NotificationType::dontSendNotification);
    addAndMakeVisible(target);
}

void ModulationConnectItemView::resized()
{
    auto area             = getLocalBounds();
    const auto sliderArea = area.removeFromRight(area.getWidth() / 2);

    // Button
    active.setBounds(area.removeFromRight(area.getWidth() / 6).reduced(0, 5));
    // Labels
    target.setBounds(area);

    // Sliders
    amount.setBounds(sliderArea);
}

}  // namespace tobanteAudio