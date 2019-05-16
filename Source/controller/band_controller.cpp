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

#include "band_controller.h"

namespace tobanteAudio
{
BandController::BandController(const int i, ModEQProcessor& p,
                               tobanteAudio::EqualizerProcessor& sub,
                               tobanteAudio::BandView& v)
    : index(i), view(v), mainProcessor(p), processor(sub)
{
    auto& state              = mainProcessor.getPluginState();
    const auto& type_id      = processor.getTypeParamID(index);
    const auto& active_id    = processor.getActiveParamID(index);
    const auto& frequency_id = processor.getFrequencyParamID(index);
    const auto& quality_id   = processor.getQualityParamID(index);
    const auto& gain_id      = processor.getGainParamID(index);

    // Link GUI components to ValueTree
    using SliderAttachment   = AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment   = AudioProcessorValueTreeState::ButtonAttachment;

    // Type & Bypass
    boxAttachments.add(new ComboBoxAttachment(state, type_id, view.type));
    buttonAttachments.add(new ButtonAttachment(state, active_id, view.activate));

    // Slider
    attachments.add(new SliderAttachment(state, frequency_id, view.frequency));
    attachments.add(new SliderAttachment(state, quality_id, view.quality));
    attachments.add(new SliderAttachment(state, gain_id, view.gain));

    // Add listner
    view.solo.addListener(this);
}

void BandController::setUIControls(tobanteAudio::EqualizerProcessor::FilterType type)
{
    switch (type)
    {
    case tobanteAudio::EqualizerProcessor::LowPass:
        view.frequency.setEnabled(true);
        view.quality.setEnabled(true);
        view.gain.setEnabled(false);
        break;

    case tobanteAudio::EqualizerProcessor::LowShelf:
        view.frequency.setEnabled(true);
        view.quality.setEnabled(false);
        view.gain.setEnabled(true);
        break;
    case tobanteAudio::EqualizerProcessor::BandPass:
        view.frequency.setEnabled(true);
        view.quality.setEnabled(true);
        view.gain.setEnabled(false);
        break;
    case tobanteAudio::EqualizerProcessor::Peak:
        view.frequency.setEnabled(true);
        view.quality.setEnabled(true);
        view.gain.setEnabled(true);
        break;
    case tobanteAudio::EqualizerProcessor::HighShelf:
        view.frequency.setEnabled(true);
        view.quality.setEnabled(true);
        view.gain.setEnabled(true);
        break;
    case tobanteAudio::EqualizerProcessor::HighPass:
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

void BandController::setSolo(bool isSolo)
{
    view.solo.setToggleState(isSolo, dontSendNotification);
}

void BandController::setFrequency(float newFreq)
{
    view.frequency.setValue(newFreq, sendNotification);
}

void BandController::setGain(float newGain)
{
    view.gain.setValue(newGain, sendNotification);
}

void BandController::setType(int newType)
{
    view.type.setSelectedId(newType + 1, sendNotification);
}

void BandController::buttonClicked(Button* b)
{
    if (b == &view.solo)
    {
        processor.setBandSolo(view.solo.getToggleState() ? index : -1);
    }
}

}  // namespace tobanteAudio