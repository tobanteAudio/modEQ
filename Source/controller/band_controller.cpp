/* Copyright 2018 Tobias Hienzsch
 *
 * modEQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "band_controller.h"

namespace TA
{

BandController::BandController(const int i, ModEQProcessor& p, TA::EqualizerProcessor& sub, TA::BandView& v)
  : index(i)
  , view(v)
  , mainProcessor(p)
  , processor(sub)
{
  // Link GUI components to ValueTree
  using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
  using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
  using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;

  auto& state = mainProcessor.getPluginState();

  boxAttachments.add(new ComboBoxAttachment(state, processor.getTypeParamName(index), view.filterType));
  buttonAttachments.add(new ButtonAttachment(state, processor.getActiveParamName(index), view.activate));

  attachments.add(new SliderAttachment(state, processor.getFrequencyParamName(index), view.frequency));
  attachments.add(new SliderAttachment(state, processor.getQualityParamName(index), view.quality));
  attachments.add(new SliderAttachment(state, processor.getGainParamName(index), view.gain));

  // Add listner
  view.solo.addListener(this);
}

void BandController::updateControls(TA::EqualizerProcessor::FilterType type)
{
  switch (type)
  {
  case TA::EqualizerProcessor::LowPass:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(true);
    view.gain.setEnabled(false);
    break;
  case TA::EqualizerProcessor::LowPass1st:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(false);
    view.gain.setEnabled(false);
    break;
  case TA::EqualizerProcessor::LowShelf:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(false);
    view.gain.setEnabled(true);
    break;
  case TA::EqualizerProcessor::BandPass:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(true);
    view.gain.setEnabled(false);
    break;
  case TA::EqualizerProcessor::AllPass:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(false);
    view.gain.setEnabled(false);
    break;
  case TA::EqualizerProcessor::AllPass1st:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(false);
    view.gain.setEnabled(false);
    break;
  case TA::EqualizerProcessor::Notch:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(true);
    view.gain.setEnabled(false);
    break;
  case TA::EqualizerProcessor::Peak:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(true);
    view.gain.setEnabled(true);
    break;
  case TA::EqualizerProcessor::HighShelf:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(true);
    view.gain.setEnabled(true);
    break;
  case TA::EqualizerProcessor::HighPass1st:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(false);
    view.gain.setEnabled(false);
    break;
  case TA::EqualizerProcessor::HighPass:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(true);
    view.gain.setEnabled(false);
    break;
  default:
    view.frequency.setEnabled(true);
    view.quality.setEnabled(true);
    view.gain.setEnabled(true);
    break;
  }
}

void BandController::updateSoloState(bool isSolo) { view.solo.setToggleState(isSolo, dontSendNotification); }

void BandController::setFrequency(float newFreq) { view.frequency.setValue(newFreq, sendNotification); }

void BandController::setGain(float newGain) { view.gain.setValue(newGain, sendNotification); }

void BandController::setType(int newType) { view.filterType.setSelectedId(newType + 1, sendNotification); }

void BandController::buttonClicked(Button* b)
{
  if (b == &view.solo)
  {
    processor.setBandSolo(view.solo.getToggleState() ? index : -1);
  }
}

} // namespace TA