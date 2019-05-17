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

#include "social_buttons.h"
#include "../utils/constants.h"

namespace tobanteAudio
{
SocialButtons::SocialButtons()
{
    setOpaque(false);

    const auto button_type = DrawableButton::ImageFitted;
    ScopedPointer<XmlElement> svg;
    ScopedPointer<Drawable> drawable;

    // GITHUB
    {
        svg      = XmlDocument::parse(TobanteAudioData::github_svg);
        drawable = Drawable::createFromSVG(*svg);

        auto* b = buttons.add(new DrawableButton("Github", button_type));
        b->addListener(this);
        b->setComponentID("https://github.com/tobanteAudio/modEQ");
        b->setImages(drawable);
        b->setTooltip(translate("Github repository"));

        addAndMakeVisible(b);
    }

    // GITHUB PAGES
    {
        svg      = XmlDocument::parse(TobanteAudioData::outlinepublic24px_svg);
        drawable = Drawable::createFromSVG(*svg);

        auto* b = buttons.add(new DrawableButton("Website", button_type));
        b->addListener(this);
        b->setComponentID("https://tobanteAudio.github.io");
        b->setImages(drawable);
        b->setTooltip(translate("Find us online"));

        addAndMakeVisible(b);
    }
}

void SocialButtons::paint(Graphics& g) { ignoreUnused(g); }

void SocialButtons::resized()
{
    auto bounds        = getLocalBounds();
    const auto spacing = 3;
    for (auto* b : buttons)
    {
        bounds.removeFromLeft(spacing);
        b->setBounds(bounds.removeFromLeft(bounds.getHeight()).reduced(3));
    }
}

void SocialButtons::buttonClicked(Button* b)
{
    URL url(b->getComponentID());
    if (url.isWellFormed())
    {
        url.launchInDefaultBrowser();
    }
}

}  // namespace tobanteAudio