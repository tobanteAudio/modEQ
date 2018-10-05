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

#include "modulation_source_controller.h"

namespace TA
{
ModulationSourceController::ModulationSourceController(
    const int i, ModEQProcessor& mp, ModulationSourceProcessor& p,
    ModulationSourceView& v)
    : index(i), mainProcessor(mp), processor(p), view(v)
{
    // Link GUI components to ValueTree
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    auto& state            = mainProcessor.getPluginState();

    attachments.add(new SliderAttachment(state, "lfo_freq", view.frequency));

    // Start Timer
    startTimerHz(25);
}

void ModulationSourceController::changeListenerCallback(
    ChangeBroadcaster* sender)
{
}

void ModulationSourceController::timerCallback()
{
    if (processor.checkForNewAnalyserData())
        processor.createAnalyserPlot(view.modulationPath, view.plotFrame,
                                     20.0f);
    view.repaint(view.plotFrame);
}

}  // namespace TA