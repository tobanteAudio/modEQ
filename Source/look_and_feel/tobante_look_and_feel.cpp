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

// tobanteAudio
#include "tobante_look_and_feel.h"
#include "../settings/constants.h"

namespace tobanteAudio
{
juce::Rectangle<float>
TobanteMetersLookAndFeel::drawBackground(juce::Graphics& g,
                                         const FFAU::LevelMeter::MeterFlags meterType,
                                         const juce::Rectangle<float> bounds)
{
    ignoreUnused(g);
    ignoreUnused(meterType);
    return bounds;
}

void TobanteMetersLookAndFeel::drawMeterBarsBackground(
    juce::Graphics& g, const FFAU::LevelMeter::MeterFlags meterType,
    const juce::Rectangle<float> bounds, const int numChannels,
    const int fixedNumChannels)
{
    ignoreUnused(g);
    ignoreUnused(meterType);
    ignoreUnused(bounds);
    ignoreUnused(numChannels);
    ignoreUnused(fixedNumChannels);
}

void TobanteMetersLookAndFeel::drawClipIndicator(
    juce::Graphics& g, const FFAU::LevelMeter::MeterFlags meterType,
    const juce::Rectangle<float> bounds, const bool hasClipped)
{
    ignoreUnused(g);
    ignoreUnused(meterType);
    ignoreUnused(bounds);
    ignoreUnused(hasClipped);
}

void TobanteMetersLookAndFeel::drawMaxNumber(juce::Graphics& g,
                                             const FFAU::LevelMeter::MeterFlags meterType,
                                             const juce::Rectangle<float> bounds,
                                             const float maxGain)
{
    ignoreUnused(g);
    ignoreUnused(meterType);
    ignoreUnused(bounds);
    ignoreUnused(maxGain);
}

juce::Rectangle<float> TobanteMetersLookAndFeel::getMeterTickmarksBounds(
    const juce::Rectangle<float> bounds,
    const FFAU::LevelMeter::MeterFlags meterType) const
{
    ignoreUnused(meterType);
    ignoreUnused(bounds);
    return juce::Rectangle<float>();
}

juce::Rectangle<float> TobanteMetersLookAndFeel::getMeterBarBounds(
    const juce::Rectangle<float> bounds,
    const FFAU::LevelMeter::MeterFlags meterType) const
{
    ignoreUnused(meterType);
    return bounds;
}

TobanteLookAndFeel::TobanteLookAndFeel()
{
    // General
    setColour(ResizableWindow::backgroundColourId, Colour(55, 71, 79));
    // Slider
    setColour(Slider::thumbColourId, tobanteAudio::ORANGE);
}

Label* TobanteLookAndFeel::createSliderTextBox(Slider& slider)
{
    Label* l = LookAndFeel_V4::createSliderTextBox(slider);
    l->setBorderSize(BorderSize<int>(2, 2, 40, 2));
    l->setColour(Label::outlineColourId, Colours::red);
    l->setColour(Label::outlineWhenEditingColourId, Colours::pink);
    return l;
}

void TobanteLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width,
                                          int height, float sliderPos,
                                          const float rotaryStartAngle,
                                          const float rotaryEndAngle, Slider& slider)
{
    auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
    auto fill    = slider.findColour(Slider::rotarySliderFillColourId);

    auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(5);

    auto radius    = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle   = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW     = jmin(8.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;

    Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius,
                                arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);

    const auto label = [&]() -> String {
        // Show frequency value
        if (slider.getName() == String("Frequency"))
        {
            return String(slider.getValue());
        }
        // Show "Master" on master gain slider
        if (slider.getName() == String("Master"))
        {
            return slider.getName();
        }

        // Default show first char of name
        return String::charToString(slider.getName()[0]);
    }();

    const auto stroke
        = PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded);
    g.setColour(outline);
    g.strokePath(backgroundArc, stroke);
    g.setFont(16.0f);
    g.drawText(label, bounds, Justification::centred, true);

    if (slider.isEnabled())
    {
        Path valueArc;
        valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius,
                               arcRadius, 0.0f, rotaryStartAngle, toAngle, true);

        g.setColour(fill);
        g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved,
                                              PathStrokeType::rounded));
    }

    auto thumbWidth = lineW * 1.5f;
    Point<float> thumbPoint(
        bounds.getCentreX()
            + arcRadius * std::cos(toAngle - MathConstants<float>::halfPi),
        bounds.getCentreY()
            + arcRadius * std::sin(toAngle - MathConstants<float>::halfPi));

    g.setColour(slider.findColour(Slider::thumbColourId));
    g.fillEllipse(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));
}

void TobanteLookAndFeel::drawComboBox(Graphics& g, int width, int height,
                                      bool isButtonDown, int buttonX, int buttonY,
                                      int buttonW, int buttonH, ComboBox& box)
{
    ignoreUnused(buttonH);
    ignoreUnused(buttonW);
    ignoreUnused(buttonX);
    ignoreUnused(buttonY);
    ignoreUnused(isButtonDown);

    const auto* parent    = box.findParentComponentOfClass<ChoicePropertyComponent>();
    const auto cornerSize = parent != nullptr ? 0.0f : 1.0f;
    const auto boxBounds  = Rectangle<int>(0, 0, width, height);

    // Background
    g.setColour(box.findColour(ComboBox::backgroundColourId));
    g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

    // Arrow
    Path path;
    const auto arrow = Rectangle<int>(width - 30, 0, 20, height);

    path.startNewSubPath(arrow.getX() + 3.0f, arrow.getCentreY() - 2.0f);
    path.lineTo(static_cast<float>(arrow.getCentreX()), arrow.getCentreY() + 3.0f);
    path.lineTo(arrow.getRight() - 3.0f, arrow.getCentreY() - 2.0f);

    const auto alpha = box.isEnabled() ? 0.9f : 0.2f;
    g.setColour(box.findColour(ComboBox::arrowColourId).withAlpha(alpha));
    g.strokePath(path, PathStrokeType(2.0f));
}

void TobanteLookAndFeel::getIdealPopupMenuItemSize(const String& text, bool isSeparator,
                                                   int standardItemHeight,
                                                   int& idealWidth, int& idealHeight)
{
    // This was added compared to the JUCE impl. The rest is the same
    standardItemHeight = static_cast<int>(standardItemHeight * 1.25);

    if (isSeparator)
    {
        idealWidth  = 50;
        idealHeight = standardItemHeight > 0 ? standardItemHeight / 10 : 10;
    }
    else
    {
        auto font = getPopupMenuFont();

        if (standardItemHeight > 0 && font.getHeight() > standardItemHeight / 1.3f)
        {
            font.setHeight(standardItemHeight / 1.3f);
        }

        idealHeight = standardItemHeight > 0 ? standardItemHeight
                                             : roundToInt(font.getHeight() * 1.3f);
        idealWidth = font.getStringWidth(text) + idealHeight * 2;
    }
}

void TobanteLookAndFeel::drawPopupMenuBackground(Graphics& g, int width, int height)
{
    ignoreUnused(width, height);
    g.fillAll(findColour(PopupMenu::backgroundColourId));
}
}  // namespace tobanteAudio