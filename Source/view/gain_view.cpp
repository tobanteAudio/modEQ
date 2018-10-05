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

#include "gain_view.h"
#include "../../JuceLibraryCode/JuceHeader.h"

namespace TA
{
//==============================================================================
GainView::GainView() : gain(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
{
    addAndMakeVisible(gain);
}

GainView::~GainView() {}

void GainView::paint(Graphics& /*g*/) {}

void GainView::resized()
{
    auto area = getLocalBounds();
    gain.setBounds(area);
}
}  // namespace TA