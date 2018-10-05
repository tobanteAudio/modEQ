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

#include "modEQ_editor.h"
#include "modEQ_processor.h"
#include "view/social_buttons.h"

static int clickRadius = 10;
static float maxDB     = 24.0f;

//==============================================================================
ModEQEditor::ModEQEditor(ModEQProcessor& p)
    : AudioProcessorEditor(&p)
    , processor(p)
    , output(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
    , plotView(processor.getEQ(), bandControllers)
//, modController(1, processor, processor.modSource, modView)
{
    tooltipWindow->setMillisecondsBeforeTipAppears(1000);

    addAndMakeVisible(socialButtons);

    modController.add(new TA::ModulationSourceController(1, processor, processor.modSource, modView));

    for (int i = 0; i < processor.getEQ().getNumBands(); ++i)
    {
        auto* bandView = bandViews.add(new TA::BandView(i));
        bandControllers.add(new TA::BandController(i, processor, processor.getEQ(), *bandView));

        // Add lookAndFeel
        // bandView->setLookAndFeel(&tobanteLookAndFeel);
        addAndMakeVisible(bandView);
    }

    addAndMakeVisible(plotView);
    addAndMakeVisible(modView);

    frame.setText(translate("Output"));
    frame.setTextLabelPosition(Justification::centred);
    addAndMakeVisible(frame);
    addAndMakeVisible(output);
    attachments.add(new AudioProcessorValueTreeState::SliderAttachment(
        processor.getPluginState(), TA::EqualizerProcessor::paramOutput, output));
    output.setTooltip(translate("Overall Gain"));

    setResizable(true, true);
    setResizeLimits(800, 450, 2990, 1800);
    setSize(1000, 750);

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

//==============================================================================
void ModEQEditor::paint(Graphics& g)
{
    const Colour inputColour  = Colours::greenyellow;
    const Colour outputColour = Colours::red;

    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    auto area        = getLocalBounds();
    auto versionArea = area.removeFromBottom(static_cast<int>(area.getHeight() / 15 * 1));

    String version = JucePlugin_VersionString;
    g.setColour(Colours::white);
    g.setFont(18.f);
    g.drawText("modEQ v" + version, versionArea.reduced(10), Justification::centredTop);
}

void ModEQEditor::resized()
{
    auto area = getLocalBounds();

    // Facebook & Gitub
    socialButtons.setBounds(area.removeFromBottom(35));

    // Modulators
    auto modArea        = area.removeFromBottom(getHeight() / 6);
    auto modSourceWidth = modArea.getWidth() / 3;
    modView.setBounds(modArea.removeFromLeft(modSourceWidth));

    // EQ Bands
    auto bandSpace = area.removeFromBottom(getHeight() / 3);
    auto width     = roundToInt(bandSpace.getWidth()) / (bandViews.size() + 1);
    for (auto* bandView : bandViews) bandView->setBounds(bandSpace.removeFromLeft(width));

    // Frame around output
    frame.setBounds(bandSpace.removeFromBottom(bandSpace.getHeight() / 2));
    output.setBounds(frame.getBounds().reduced(8));

    // FFT
    plotView.setBounds(area);
}
