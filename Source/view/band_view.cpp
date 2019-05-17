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
#include "../utils/constants.h"

namespace tobanteAudio
{
BandView::BandView(int const i, const Colour c)
    : index(i)
    , colour(c)
    , frequency(Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox)
    , quality(Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox)
    , gain(Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox)
    , solo(translate("S"))
    , activate(translate("A"))
{
    // Add all filter options to combo box
    type.clear();
    for (int j = 0; j < tobanteAudio::EqualizerProcessor::LastFilterID; ++j)
    {
        using EQ               = tobanteAudio::EqualizerProcessor;
        auto const type_string = EQ::getFilterTypeName(static_cast<EQ::FilterType>(j));
        type.addItem(type_string, j + 1);
    }

    // Make controls visible
    addAndMakeVisible(type);
    addAndMakeVisible(gain);
    addAndMakeVisible(quality);
    addAndMakeVisible(frequency);
    addAndMakeVisible(activate);
    addAndMakeVisible(solo);

    // Name
    frequency.setName("Frequency");
    quality.setName("Quality");
    gain.setName("Gain");

    // Tooltip
    frequency.setTooltip(translate("Filter's frequency"));
    quality.setTooltip(translate("Filter's steepness (Quality)"));
    gain.setTooltip(translate("Filter's gain"));

    // Solo
    solo.setClickingTogglesState(true);
    solo.setTooltip(translate("Listen only through this filter (solo)"));

    // Activate / Bypass
    activate.setClickingTogglesState(true);
    activate.setColour(TextButton::buttonOnColourId, Colours::green);
    activate.setTooltip(translate("Activate or deactivate this filter"));
}

void BandView::paint(Graphics& g)
{
    // Background
    const auto color = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    auto area        = getLocalBounds().reduced(5);
    g.setColour(color.brighter().withAlpha(0.5f));
    g.fillRect(area);
}

void BandView::resized()
{
    auto bounds = getLocalBounds();
    bounds.reduce(10, 20);

    const auto height      = bounds.getHeight();
    const auto type_height = static_cast<int>(height / 12);
    // TYPE
    type.setBounds(bounds.removeFromTop(type_height));

    // FREQUENCY
    auto freq_bounds = bounds.removeFromBottom(bounds.getHeight() / 2);
    frequency.setBounds(freq_bounds);

    // BUTTONS
    const auto buttons_y = freq_bounds.getY() - type_height;
    auto buttons         = freq_bounds.withTop(buttons_y).withHeight(type_height);
    solo.setBounds(buttons.removeFromLeft(type_height));
    activate.setBounds(buttons.removeFromRight(type_height));

    // GAIN & QUALITY
    const auto quality_bounds = bounds.removeFromLeft(bounds.getWidth() / 2)
                                    .withTop(bounds.getY() - type_height * 1.5);
    const auto gain_bounds = bounds.withTop(bounds.getY() + type_height);

    quality.setBounds(quality_bounds);
    gain.setBounds(gain_bounds);
}
}  // namespace tobanteAudio