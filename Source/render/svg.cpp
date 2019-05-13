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

#include "svg.h"

namespace tobanteAudio
{
void SvgchangeColor(XmlElement* xml, String color_hex)
{
    forEachXmlChildElement(*xml, xmlnode)
    {
        forEachXmlChildElement(*xmlnode, xmlouterdefs)
        {
            String style = xmlouterdefs->getStringAttribute("style");
            if (style.isNotEmpty())
            {
                style = style.replace("#030104", color_hex, true);
                xmlouterdefs->setAttribute("style", style);
            }

            forEachXmlChildElement(*xmlouterdefs, xmldefs)
            {
                String style = xmldefs->getStringAttribute("style");
                if (style.isNotEmpty())
                {
                    style = style.replace("#030104", color_hex, true);
                    xmldefs->setAttribute("style", style);
                }
            }
        }
    }
}

void drawFromSVG(Graphics& g, const char* svgbinary, String col_hex, int x, int y, int newWidth,
                 int newHeight)
{
    ScopedPointer<XmlElement> svg(XmlDocument::parse(svgbinary));
    jassert(svg != nullptr);
    SvgchangeColor(svg, col_hex);

    ScopedPointer<Drawable> drawable;

    if (svg != nullptr)
    {
        drawable = Drawable::createFromSVG(*svg);
        drawable->setTransformToFit(Rectangle<float>(x, y, newWidth, newHeight),
                                    RectanglePlacement::stretchToFit);
        drawable->draw(g, 1.0f);
    }
}
}  // namespace tobanteAudio