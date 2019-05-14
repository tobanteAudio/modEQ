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

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../render/svg.h"
#include "menu_bar_view.h"

namespace tobanteAudio
{
MenuBarView::MenuBarView() {}

void MenuBarView::paint(Graphics& g)
{
    // Binary data pointers
    const auto* undo_svg    = TobanteAudioData::outlineundo24px_svg;
    const auto* redo_svg    = TobanteAudioData::outlineredo24px_svg;
    const auto* power_svg   = TobanteAudioData::outlinepower_settings_new24px_svg;
    const auto* setting_svg = TobanteAudioData::outlinesettings24px_svg;
    const auto* info_svg    = TobanteAudioData::outlineinfo24px_svg;

    const auto height  = getHeight();
    const auto width   = getWidth();
    const auto spacing = 8;
    const auto color   = getLookAndFeel().findColour(Slider::thumbColourId);

    // Undo & Redo (left)

    tobanteAudio::drawFromSVG(g, undo_svg, color, Rectangle<float>(spacing, 0, height, height));
    tobanteAudio::drawFromSVG(g, redo_svg, color, Rectangle<float>(1 * height + 2 * spacing, 0,
                              height, height));

    // Power (middle)
    const auto power_x = static_cast<int>(width / 2 - height / 2);
    tobanteAudio::drawFromSVG(g, power_svg, color, Rectangle<float>(power_x, 0, height, height));

    // Settings (right)
    const auto settings_x = width - height - spacing;
    const auto info_x     = width - height * 2 - 2 * spacing;
    tobanteAudio::drawFromSVG(g, info_svg, color, Rectangle<float>(info_x, 0, height, height));
    tobanteAudio::drawFromSVG(g, setting_svg, color, Rectangle<float>(settings_x, 0, height, height));
}

void MenuBarView::resized() {}

}  // namespace tobanteAudio