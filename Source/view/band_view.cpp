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

#include "band_view.h"
#include "../modEQ_processor.h"

namespace tobanteAudio
{
BandView::BandView(int const i, const Colour c)
    : index(i)
    , colour(c)
    , frequency(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
    , quality(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
    , gain(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
    , solo(translate("S"))
    , activate(translate("A"))
{
    // Frame for individual band controls
    frame.setText(String(index));
    frame.setTextLabelPosition(Justification::centred);
    frame.setColour(GroupComponent::textColourId, Colours::silver);
    frame.setColour(GroupComponent::outlineColourId, colour);
    addAndMakeVisible(frame);

    // Add all filter options to combo box
    type.clear();
	for (int j = 0; j < tobanteAudio::EqualizerProcessor::LastFilterID; ++j)
    {
        using EQ         = tobanteAudio::EqualizerProcessor;
        auto type_string = EQ::getFilterTypeName(static_cast<EQ::FilterType>(j));
        DBG(type_string + ": " + String(j));
        type.addItem(type_string, j+1);
    }

    // Make controls visible
    addAndMakeVisible(type);
    addAndMakeVisible(gain);
    addAndMakeVisible(quality);
    addAndMakeVisible(frequency);
    addAndMakeVisible(activate);
    addAndMakeVisible(solo);

    // Tooltip
    frequency.setTooltip(translate("Filter's frequency"));
    quality.setTooltip(translate("Filter's steepness (Quality)"));
    gain.setTooltip(translate("Filter's gain"));

    // Solo
    solo.setClickingTogglesState(true);
    solo.setColour(TextButton::buttonOnColourId, Colours::yellow);
    solo.setTooltip(translate("Listen only through this filter (solo)"));

    // Activate / Bypass
    activate.setClickingTogglesState(true);
    activate.setColour(TextButton::buttonOnColourId, Colours::green);
    activate.setTooltip(translate("Activate or deactivate this filter"));
}

void BandView::resized()
{
    auto bounds = getLocalBounds();
    frame.setBounds(bounds);

    bounds.reduce(10, 20);

    type.setBounds(bounds.removeFromTop(20));

    auto freqBounds = bounds.removeFromBottom(bounds.getHeight() * 2 / 4);
    frequency.setBounds(freqBounds.withTop(freqBounds.getY() + 10));

    auto buttons = freqBounds.reduced(5).withHeight(20);
    solo.setBounds(buttons.removeFromLeft(20));
    activate.setBounds(buttons.removeFromRight(20));

    quality.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
    gain.setBounds(bounds);
}
}  // namespace tobanteAudio