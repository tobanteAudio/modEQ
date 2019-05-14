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

#include "menu_bar_view.h"

namespace tobanteAudio
{
MenuBarView::MenuBarView()
    : undoButton("undo", DrawableButton::ImageStretched)
    , redoButton("redo", DrawableButton::ImageStretched)
    , powerButton("power", DrawableButton::ImageStretched)
    , settingButton("setting", DrawableButton::ImageStretched)
    , infoButton("info", DrawableButton::ImageStretched)
{
    const auto color = Colour(255, 87, 34).withAlpha(0.8f);
    ScopedPointer<XmlElement> svg;
    ScopedPointer<Drawable> drawable;

    // UNDO
    svg = XmlDocument::parse(TobanteAudioData::outlineundo24px_svg);
    jassert(svg != nullptr);
    drawable = Drawable::createFromSVG(*svg);
    drawable->replaceColour(Colours::black, color);
    undoButton.setImages(drawable);
    addAndMakeVisible(undoButton);

    // REDO
    svg = XmlDocument::parse(TobanteAudioData::outlineredo24px_svg);
    jassert(svg != nullptr);
    drawable = Drawable::createFromSVG(*svg);
    drawable->replaceColour(Colours::black, color);
    redoButton.setImages(drawable);
    addAndMakeVisible(redoButton);

    // POWER
    svg = XmlDocument::parse(TobanteAudioData::outlinepower_settings_new24px_svg);
    jassert(svg != nullptr);
    drawable = Drawable::createFromSVG(*svg);
    drawable->replaceColour(Colours::black, color);
    powerButton.setImages(drawable);
    addAndMakeVisible(powerButton);

    // SETTING
    svg = XmlDocument::parse(TobanteAudioData::outlinesettings24px_svg);
    jassert(svg != nullptr);
    drawable = Drawable::createFromSVG(*svg);
    drawable->replaceColour(Colours::black, color);
    settingButton.setImages(drawable);
    addAndMakeVisible(settingButton);

    // INFO
    svg = XmlDocument::parse(TobanteAudioData::outlineinfo24px_svg);
    jassert(svg != nullptr);
    drawable = Drawable::createFromSVG(*svg);
    drawable->replaceColour(Colours::black, color);
    infoButton.setImages(drawable);
    addAndMakeVisible(infoButton);

}

void MenuBarView::paint(Graphics& g) { ignoreUnused(g); }

void MenuBarView::resized()
{
    const auto height  = getHeight();
    const auto width   = getWidth();
    const auto spacing = 8;

    // Undo & Redo (left)
    undoButton.setBounds(Rectangle<int>(spacing, 0, height, height));
    redoButton.setBounds(Rectangle<int>(1 * height + 2 * spacing, 0, height, height));

    // Power (middle)
    const auto power_x = static_cast<int>(width / 2 - height / 2);
    powerButton.setBounds(Rectangle<int>(power_x, 0, height, height));

    // Settings (right)
    const auto settings_x = width - height - spacing;
    const auto info_x     = width - height * 2 - 2 * spacing;
    settingButton.setBounds(Rectangle<int>(info_x, 0, height, height));
    infoButton.setBounds(Rectangle<int>(settings_x, 0, height, height));
}

}  // namespace tobanteAudio