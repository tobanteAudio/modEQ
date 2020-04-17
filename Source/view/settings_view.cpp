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

#include "settings_view.h"

namespace tobanteAudio
{
SettingsView::SettingsView() { rows.emplace_back(String("Settings")); }

void SettingsView::paint(Graphics& g)
{
    // Background
    const auto bgColor = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    g.fillAll(bgColor.brighter().withAlpha(0.5f));

    // Draw rows
    g.setColour(Colours::black);
    g.setFont(32.0f);

    auto bounds       = getLocalBounds();
    const auto height = static_cast<int>(bounds.getHeight() / rows.size());
    for (const auto& row : rows)
    { g.drawText(row, bounds.removeFromTop(height), Justification::centred, true); }
}

void SettingsView::resized() { }

}  // namespace tobanteAudio