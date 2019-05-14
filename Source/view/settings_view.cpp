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

#include "settings_view.h"

namespace tobanteAudio
{
SettingsView::SettingsView() {}

void SettingsView::paint(Graphics& g)
{
    // Background
    const auto backgroundColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    g.fillAll(backgroundColour.brighter().withAlpha(0.5f));

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);  // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("SettingsView", getLocalBounds(), Justification::centred,
               true);  // draw some placeholder text
}

void SettingsView::resized() {}

}  // namespace tobanteAudio