/* Copyright 2018 Tobias Hienzsch
 *
 * modEQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * modEQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with modEQ.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../analyser/modulation_source_analyser.h"
#include "../processor/modulation_source_processor.h"

namespace tobanteAudio
{
//==============================================================================
class ModulationConnectView : public Component, public Slider::Listener
{
public:
    //==============================================================================
    ModulationConnectView(int);
    ~ModulationConnectView() override;

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    //==============================================================================
    void sliderValueChanged(Slider* slider) override;

    //==============================================================================
    Slider amount;
    Label target;

private:
    int index;
    //==============================================================================

#if TOBANTEAUDIO_LIVE_MOCK
public:
    ModulationConnectView() : ModulationConnectView(1) { setSize(100, 100); }
#endif  // TOBANTEAUDIO_LIVE_MOCK
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationConnectView)
};


//==============================================================================
class ModulationSourceView : public Component, public Slider::Listener, public Button::Listener
{
public:
    //==============================================================================
    ModulationSourceView(int);
    ~ModulationSourceView() override;

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    //==============================================================================
    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button* b) override;

    //==============================================================================
    Slider frequency;
    Slider gain;
    TextButton toggleConnectView;  // activates the mapping view.
    Rectangle<int> plotFrame;
    Path modulationPath;
    Label freqLabel, gainLabel;
    ModulationConnectView modConnect;

private:
    int index;
    bool _connectViewActive{false};
    //==============================================================================

#if TOBANTEAUDIO_LIVE_MOCK
public:
    ModulationSourceView() : ModulationSourceView(1) { setSize(100, 100); }
#endif  // TOBANTEAUDIO_LIVE_MOCK
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceView)
};

}  // namespace tobanteAudio
