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

// tobanteAudio
#include "modulation_source_controller.h"
#include "../utils/constants.h"

namespace tobanteAudio
{
ModulationSourceController::ModulationSourceController(
    const int i, ModEQProcessor& mp, ModulationSourceProcessor& p,
    ModulationSourceView& v)
    : index(i)
    , connectViewActive(false)
    , mainProcessor(mp)
    , processor(p)
    , view(v)
{
    // Link GUI components to ValueTree
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    auto& state            = mainProcessor.getPluginState();

    attachments.add(new SliderAttachment(
        state, "lfo_" + String(index) + "_freq", view.frequency));
    attachments.add(new SliderAttachment(
        state, "lfo_" + String(index) + "_gain", view.gain));

    // Button Connect
    view.toggleConnectView.addListener(this);
    view.modConnect1.setVisible(connectViewActive);
    view.modConnect2.setVisible(connectViewActive);
    // Slider connect
    view.frequency.addListener(this);
    view.gain.addListener(this);

    // Start Timer
    startTimerHz(GLOBAL_REFRESH_RATE_HZ);
}

void ModulationSourceController::buttonClicked(Button* b)
{
    if (b == &view.toggleConnectView)
    {
        connectViewActive = !connectViewActive;
        view.modConnect1.setVisible(connectViewActive);
        view.modConnect2.setVisible(connectViewActive);
    }
}

void ModulationSourceController::sliderValueChanged(Slider* slider)
{
    auto& frequency = view.frequency;
    auto& gain      = view.gain;
    auto& gainLabel = view.gainLabel;
    auto& freqLabel = view.freqLabel;

    if (slider == &frequency)
    {
        freqLabel.setText(frequency.getTextFromValue(frequency.getValue()),
                          NotificationType::dontSendNotification);
    }

    if (slider == &gain)
    {
        gainLabel.setText(gain.getTextFromValue(gain.getValue()),
                          NotificationType::dontSendNotification);
    }
}
void ModulationSourceController::timerCallback()
{
    if (processor.checkForNewAnalyserData())
    {
        processor.createAnalyserPlot(view.modulationPath, view.plotFrame,
                                     20.0f);
    }
    view.repaint(view.plotFrame);
}

}  // namespace tobanteAudio