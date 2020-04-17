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

#include "svg.h"

namespace tobanteAudio
{
void drawFromSVG(Graphics& g, const char* svgbinary, Colour color, Rectangle<float> pos)
{
    ScopedPointer<XmlElement> svg(XmlDocument::parse(svgbinary));
    jassert(svg != nullptr);

    ScopedPointer<Drawable> drawable;
    if (svg != nullptr)
    {
        drawable = Drawable::createFromSVG(*svg);
        drawable->setTransformToFit(pos, RectanglePlacement::stretchToFit);
        drawable->replaceColour(Colours::black, color);
        drawable->draw(g, 0.8f);
    }
}
}  // namespace tobanteAudio