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

#include "modEQ_editor.h"
#include "modEQ_processor.h"
#include "utils/parameters.h"

ModEQEditor::ModEQEditor(ModEQProcessor& p)
    : AudioProcessorEditor(&p)
    , processor(p)
    , menuController(processor, menuButtons)
    , output(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
{
    // Global look & feel
    setLookAndFeel(&tobanteLookAndFeel);

    // Social buttons
    addAndMakeVisible(socialButtons);

    // Menu
    addAndMakeVisible(menuButtons);
    menuController.toggleBypass   = [this]() { DBG("BYPASS"); };
    menuController.toggleSettings = [this]() {
        infoView.setVisible(false);
        settingsView.setVisible(!settingsView.isVisible());
        analyserView->setVisible(!settingsView.isVisible());
    };
    menuController.toggleInfo = [this]() {
        settingsView.setVisible(false);
        infoView.setVisible(!infoView.isVisible());
        analyserView->setVisible(!infoView.isVisible());
    };

    // Settings & Info
    addAndMakeVisible(infoView);
    addAndMakeVisible(settingsView);
    infoView.setVisible(false);
    settingsView.setVisible(false);

    // Modulation
    for (int i = 1; i < 2; ++i)
    {
        using MSC = tobanteAudio::ModulationSourceController;
        using MSV = tobanteAudio::ModulationSourceView;

        auto* const modView = modViews.add(new MSV(i));
        modController.add(new MSC(i, processor, processor.modSource, *modView));

        addAndMakeVisible(modView);
    }

    // EQ bands
    for (int i = 0; i < processor.getEQ().getNumBands(); ++i)
    {
        using BC = tobanteAudio::BandController;
        using BV = tobanteAudio::BandView;

        const auto color     = processor.getEQ().getBand(i)->colour;
        auto* const bandView = bandViews.add(new BV(i, color));
        bandControllers.add(new BC(i, processor, processor.getEQ(), *bandView));

        addAndMakeVisible(bandView);
    }

    // Plot
    using tobanteAudio::AnalyserController;
    using tobanteAudio::AnalyserView;

    auto& eq           = processor.getEQ();
    analyserView       = std::make_unique<AnalyserView>();
    analyserController = std::make_unique<AnalyserController>(
        eq, bandControllers, *analyserView.get());
    addAndMakeVisible(analyserView.get());

    //  Master Section
    frame.setText(translate("Master - Out"));
    frame.setTextLabelPosition(Justification::centred);
    addAndMakeVisible(frame);
    addAndMakeVisible(output);
    output.setTooltip(translate("Overall Gain"));

    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    auto& state            = processor.getPluginState();

    attachments.add(
        new SliderAttachment(state, tobanteAudio::Parameters::Output, output));

    setResizable(true, true);
    setResizeLimits(800, 450, 2990, 1800);
    setSize(1000, 750);
    tooltipWindow->setMillisecondsBeforeTipAppears(1000);

#ifdef JUCE_OPENGL
    openGLContext.attachTo(*getTopLevelComponent());
#endif
}

ModEQEditor::~ModEQEditor()
{
    setLookAndFeel(nullptr);
    PopupMenu::dismissAllActiveMenus();

#ifdef JUCE_OPENGL
    openGLContext.detach();
#endif
}

void ModEQEditor::paint(Graphics& g)
{
    const auto backgroundColour
        = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    g.fillAll(backgroundColour);
}

void ModEQEditor::resized()
{
    auto area = getLocalBounds();

    // Facebook & Gitub
    const auto button_height = static_cast<int>(area.getHeight() / 20);
    socialButtons.setBounds(area.removeFromBottom(button_height));
    menuButtons.setBounds(area.removeFromTop(button_height));

    // Modulators
    // auto modArea              = area.removeFromBottom(getHeight() / 5);
    // auto const modSourceWidth = modArea.getWidth() / 3;
    // for (auto* modView : modViews)
    // modView->setBounds(modArea.removeFromLeft(modSourceWidth));

    // EQ Bands
    auto bandSpace = area.removeFromBottom((getHeight() / 10) * 4);
    auto const width
        = roundToInt(bandSpace.getWidth()) / (bandViews.size() + 1);
    for (auto* bandView : bandViews)
        bandView->setBounds(bandSpace.removeFromLeft(width));

    // Frame around output
    frame.setBounds(bandSpace.removeFromBottom(bandSpace.getHeight() / 2));
    output.setBounds(frame.getBounds().reduced(8));

    // FFT
    analyserView->setBounds(area);
    infoView.setBounds(area);
    settingsView.setBounds(area);
}
