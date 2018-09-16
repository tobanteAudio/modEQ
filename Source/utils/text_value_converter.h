/* Copyright 2018 Tobias Hienzsch
 *
 * modEQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

class ActiveTextConverter
{
public:
  String operator()(float value) { return value > 0.5f ? translate("active") : translate("bypassed"); }

  float operator()(const String& text) { return text == translate("active"); }
};

class FrequencyTextConverter
{
public:
  String operator()(float value)
  {
    return (value < 1000) ? String(value, 0) + " Hz" : String(value / 1000.0, 2) + " kHz";
  }

  float operator()(const String& text)
  {
    return text.endsWith(" kHz") ? static_cast<float>(text.dropLastCharacters(4).getFloatValue() * 1000.0)
                                 : static_cast<float>(text.dropLastCharacters(3).getFloatValue());
  }
};

class QualityTextConverter
{
public:
  String operator()(float value) { return String(value, 1); }

  float operator()(const String& text) { return text.getFloatValue(); }
};

class GainTextConverter
{
public:
  String operator()(float value) { return String(Decibels::gainToDecibels(value), 1) + " dB"; }

  float operator()(const String& text) { return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); }
};


class InvertPhaseTextConverter
{
public:
  String operator()(float value) { return value < 0.5 ? "Normal" : "Inverted"; }

  float operator()(const String& text)
  {
    if (text == "Normal")
      return 0.0f;
    if (text == "Inverted")
      return 1.0f;
    return 0.0f;
  }
};
