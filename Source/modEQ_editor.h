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

// JUCE
#include "../JuceLibraryCode/JuceHeader.h"

// tobanteAudio
#include "controller/analyser_controller.h"
#include "controller/band_controller.h"
#include "controller/modulation_source_controller.h"
#include "look_and_feel/tobante_look_and_feel.h"
#include "modEQ_processor.h"
#include "view/analyser_view.h"
#include "view/band_view.h"
#include "view/equalizer_plot_view.h"
#include "view/modulation_source_view.h"
#include "view/social_buttons.h"

class ModEQEditor : public AudioProcessorEditor
{
public:
    ModEQEditor(ModEQProcessor&);
    ~ModEQEditor() override;

    void paint(Graphics& /*g*/) override;
    void resized() override;

private:
    ModEQProcessor& processor;

    tobanteAudio::TobanteLookAndFeel tobanteLookAndFeel;
    OwnedArray<tobanteAudio::BandView> bandViews;
    OwnedArray<tobanteAudio::BandController> bandControllers;
    OwnedArray<tobanteAudio::ModulationSourceView> modViews;
    OwnedArray<tobanteAudio::ModulationSourceController> modController;
    tobanteAudio::EqualizerPlotView plotView;
    tobanteAudio::AnalyserView analyserView;
    tobanteAudio::AnalyserController analyserController;

    Rectangle<int> plotArea;

    tobanteAudio::SocialButtons socialButtons;
    Slider output;
    GroupComponent frame;  // for output slider

    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
    SharedResourcePointer<TooltipWindow> tooltipWindow;

#ifdef JUCE_OPENGL
    OpenGLContext openGLContext;
#endif

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModEQEditor)
};
