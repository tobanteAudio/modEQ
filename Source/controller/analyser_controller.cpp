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

#include "analyser_controller.h"

namespace tobanteAudio
{
AnalyserController::AnalyserController(ModEQProcessor& p, tobanteAudio::AnalyserView& v)
    : mainProcessor(p), view(v)
{
    view.addMouseListener(this, false);
}

void AnalyserController::changeListenerCallback(ChangeBroadcaster* sender) { ignoreUnused(sender); }
void AnalyserController::timerCallback() {}
void AnalyserController::mouseDown(const MouseEvent& e) { ignoreUnused(e); }
void AnalyserController::mouseMove(const MouseEvent& e) { ignoreUnused(e); }
void AnalyserController::mouseDrag(const MouseEvent& e) { ignoreUnused(e); }
void AnalyserController::mouseDoubleClick(const MouseEvent& e) { ignoreUnused(e); }
void AnalyserController::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
{
    ignoreUnused(event);
    ignoreUnused(wheel);
}
}  // namespace tobanteAudio