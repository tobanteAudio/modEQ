/*
  ==============================================================================

    TextValueConverters.h
    Created: 28 Aug 2018 4:31:13pm
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"


class BypassTextConverter
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

class FilterQualityTextConverter
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
