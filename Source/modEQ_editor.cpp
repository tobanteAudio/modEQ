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
    , plotView(processor.getEQ(), bandControllers)
    , analyserController(processor.getEQ(), bandControllers, analyserView)

    , output(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
{
    // Social buttons
    addAndMakeVisible(socialButtons);

    // Plot
    // addAndMakeVisible(plotView);
    addAndMakeVisible(analyserView);

    // Modulation
    for (int i = 1; i < 2; ++i)
    {
        using MSC = tobanteAudio::ModulationSourceController;
        using MSV = tobanteAudio::ModulationSourceView;

        auto* const modView = modViews.add(new MSV(i));
        modController.add(new MSC(i, processor, processor.modSource, *modView));

        // modView->setLookAndFeel(&tobanteLookAndFeel);
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

        // bandView->setLookAndFeel(&tobanteLookAndFeel);
        addAndMakeVisible(bandView);
    }

    //  Master Section
    frame.setText(translate("Master - Out"));
    frame.setTextLabelPosition(Justification::centred);
    addAndMakeVisible(frame);
    addAndMakeVisible(output);
    output.setTooltip(translate("Overall Gain"));
    output.setColour(Slider::thumbColourId, Colours::red);

    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    auto& state            = processor.getPluginState();

    attachments.add(new SliderAttachment(state, tobanteAudio::Parameters::Output, output));

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
    PopupMenu::dismissAllActiveMenus();

#ifdef JUCE_OPENGL
    openGLContext.detach();
#endif
}

void ModEQEditor::paint(Graphics& g)
{
    const auto backgroundColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);

    // Background
    g.fillAll(backgroundColour);

    // Version footer
    const String version   = JucePlugin_VersionString;
    const auto versionArea = [&]() -> juce::Rectangle<int> {
        auto area = getLocalBounds();
        return area.removeFromBottom(static_cast<int>(area.getHeight() / 20));
    }();
    g.setColour(Colours::white);
    g.setFont(16.f);
    g.drawText("modEQ v" + version, versionArea.reduced(5), Justification::centred);
}

void ModEQEditor::resized()
{
    auto area = getLocalBounds();

    // Facebook & Gitub
    socialButtons.setBounds(area.removeFromBottom(static_cast<int>(area.getHeight() / 20)));

    // Modulators
    auto modArea              = area.removeFromBottom(getHeight() / 5);
    auto const modSourceWidth = modArea.getWidth() / 3;
    for (auto* modView : modViews) modView->setBounds(modArea.removeFromLeft(modSourceWidth));

    // EQ Bands
    auto bandSpace   = area.removeFromBottom(getHeight() / 3);
    auto const width = roundToInt(bandSpace.getWidth()) / (bandViews.size() + 1);
    for (auto* bandView : bandViews) bandView->setBounds(bandSpace.removeFromLeft(width));

    // Frame around output
    frame.setBounds(bandSpace.removeFromBottom(bandSpace.getHeight() / 2));
    output.setBounds(frame.getBounds().reduced(8));

    // FFT
    // plotView.setBounds(area);
    analyserView.setBounds(area);
}
