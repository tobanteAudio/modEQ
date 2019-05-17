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

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

namespace tobanteAudio
{
/**
 * @brief Text & float converter for active/bypass.
 */
struct ActiveTextConverter
{
    String operator()(float const value) const
    {
        return value > 0.5f ? translate("active") : translate("bypassed");
    }

    float operator()(const String& text) const
    {
        return static_cast<float>(text == translate("active"));
    }
};

/**
 * @brief Text & float converter for frequency.
 */
struct FrequencyTextConverter
{
    String operator()(float const value) const
    {
        return (value < 1000) ? String(value, 0) + " Hz"
                              : String(value / 1000.0, 2) + " kHz";
    }

    float operator()(const String& text) const
    {
        return text.endsWith(" kHz")
                   ? static_cast<float>(text.dropLastCharacters(4).getFloatValue()
                                        * 1000.0)
                   : static_cast<float>(text.dropLastCharacters(3).getFloatValue());
    }
};

/**
 * @brief Text & float converter for filter quality.
 */
struct QualityTextConverter
{
    String operator()(float const value) const { return String(value, 1); }

    float operator()(const String& text) const { return text.getFloatValue(); }
};

/**
 * @brief Text & float converter for gain.
 */
struct GainTextConverter
{
    String operator()(float const value) const
    {
        return String(Decibels::gainToDecibels(value), 1) + " dB";
    }

    float operator()(const String& text) const
    {
        return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue());
    }
};

/**
 * @brief Text & float converter for phase invert.
 */
struct InvertPhaseTextConverter
{
    String operator()(float const value) const
    {
        return value < 0.5 ? "Normal" : "Inverted";
    }

    float operator()(const String& text) const
    {
        if (text == "Normal")
        {
            return 0.0f;
        }
        if (text == "Inverted")
        {
            return 1.0f;
        }
        return 0.0f;
    }
};

/**
 * @brief Text & float converter for filter type.
 */
struct FilterTypeTextConverter
{
    String operator()(float const value) const;

    float operator()(const String& text) const;
};
}  // namespace tobanteAudio
