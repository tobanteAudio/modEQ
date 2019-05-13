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
MenuBarView::MenuBarView()
{
    // buttons.add(XmlDocument::parse(TobanteAudioData::sharpcheck_box24px_svg));
    // tobanteAudio::SvgchangeColor(buttons[0], "#61f0c4");  // RECOLOUR}
}

void MenuBarView::paint(Graphics& g)
{
    const auto height = getHeight();
    // const auto area   = Rectangle<float>(0, 0, height, height);
    // drawable->setTransformToFit(area, RectanglePlacement::stretchToFit);
    // drawable->draw(g, 1.0f);

    tobanteAudio::drawFromSVG(g, TobanteAudioData::sharpcheck_box24px_svg, "#61f0c4", 0, 0, height,
                              height);
}

void MenuBarView::resized() {}

}  // namespace tobanteAudio