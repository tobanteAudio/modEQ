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

#pragma once

// JUCE
#include "modEQ.hpp"
// tobanteAudio
#include "modEQ_processor.h"

#include "controller/analyser_controller.h"
#include "controller/band_controller.h"
#include "controller/menu_bar_controller.h"
#include "controller/modulation_source_controller.h"
#include "look_and_feel/tobante_look_and_feel.h"
#include "view/analyser_view.h"
#include "view/band_view.h"
#include "view/info_view.h"
#include "view/menu_bar_view.h"
#include "view/modulation_source_view.h"
#include "view/settings_view.h"
#include "view/social_buttons.h"

/**
 * @brief Entry point for GUI thread. Inherites from juce::AudioProcessorEditor
 */
class ModEQEditor : public AudioProcessorEditor
{
public:
    ModEQEditor(ModEQProcessor& p);
    ~ModEQEditor() override;

    void paint(Graphics& g) override;
    void resized() override;

private:
    ModEQProcessor& mainProcessor;

    // Components
    tobanteAudio::TobanteLookAndFeel tobanteLookAndFeel;
    tobanteAudio::SocialButtons socialButtons;
    tobanteAudio::InfoView infoView;
    tobanteAudio::SettingsView settingsView;
    tobanteAudio::MenuBarView menuButtons;
    tobanteAudio::MenuBarController menuController;

    // Bands
    OwnedArray<tobanteAudio::BandView> bandViews;
    OwnedArray<tobanteAudio::BandController> bandControllers;

    // Modulation
    OwnedArray<tobanteAudio::ModulationSourceView> modViews;
    OwnedArray<tobanteAudio::ModulationSourceController> modController;

    // Analyser Plot
    std::unique_ptr<tobanteAudio::AnalyserView> analyserView;
    std::unique_ptr<tobanteAudio::AnalyserController> analyserController;

    // Meter
    std::unique_ptr<tobanteAudio::TobanteMetersLookAndFeel> lnf;
    std::unique_ptr<FFAU::LevelMeter> meter;

    // Master - Out
    Slider output;
    Rectangle<int> outputSliderFrame;
    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;

    SharedResourcePointer<TooltipWindow> tooltipWindow;

#ifdef JUCE_OPENGL
    OpenGLContext openGLContext;
#endif

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModEQEditor)
};
