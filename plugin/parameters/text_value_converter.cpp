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

#include "text_value_converter.h"
#include "../processor/equalizer_processor.h"

namespace tobanteAudio
{
// ACTIVE / BYPASS
String ActiveTextConverter::operator()(float const value) const
{
    return value > 0.5f ? translate("active") : translate("bypassed");
}

float ActiveTextConverter::operator()(const String& text) const
{
    return static_cast<float>(text == translate("active"));
}

// FREQUENCY
String FrequencyTextConverter::operator()(float const value) const
{
    return (value < 1000) ? String(value, 0) + " Hz" : String(value / 1000.0, 2) + " kHz";
}

float FrequencyTextConverter::operator()(const String& text) const
{
    return text.endsWith(" kHz") ? static_cast<float>(text.dropLastCharacters(4).getFloatValue() * 1000.0)
                                 : static_cast<float>(text.dropLastCharacters(3).getFloatValue());
}

// QUALITY
String QualityTextConverter::operator()(float const value) const { return String(value, 1); }

float QualityTextConverter::operator()(const String& text) const { return text.getFloatValue(); }

// GAIN
String GainTextConverter::operator()(float const value) const
{
    return String(Decibels::gainToDecibels(value), 1) + " dB";
}

float GainTextConverter::operator()(const String& text) const
{
    return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue());
}

// PHASE INVERT
String InvertPhaseTextConverter::operator()(float const value) const { return value < 0.5 ? "Normal" : "Inverted"; }

float InvertPhaseTextConverter::operator()(const String& text) const
{
    if (text == "Normal") { return 0.0f; }
    if (text == "Inverted") { return 1.0f; }
    return 0.0f;
}

// FILTER TYPE
String FilterTypeTextConverter::operator()(float const value) const
{
    using EP = tobanteAudio::EqualizerProcessor;
    return EP::getFilterTypeName(static_cast<EP::FilterType>(static_cast<int>(value)));
}

float FilterTypeTextConverter::operator()(const String& text) const
{
    using EP = tobanteAudio::EqualizerProcessor;

    for (int i = 0; i < EP::LastFilterID; ++i)
    {
        if (text == EP::getFilterTypeName(static_cast<EP::FilterType>(i))) { return static_cast<float>(i); }
    }
    return static_cast<float>(EP::NoFilter);
}
}  // namespace tobanteAudio