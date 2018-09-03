/*
  ==============================================================================

    BandController.cpp
    Created: 2 Sep 2018 6:06:55pm
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#include "BandController.h"
namespace TA
{

BandController::BandController(const int i, FrequalizerAudioProcessor& p, TA::BandView& v)
  : index(i)
  , view(v)
  , processor(p)
{

	
   view.solo.addListener(this);
  // Link GUI components to ValueTree
  using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
  using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
  using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;

  auto& state = processor.getPluginState();

  boxAttachments.add(new ComboBoxAttachment(state, processor.getEQ().getTypeParamName(index), view.filterType));
  buttonAttachments.add(new ButtonAttachment(state, processor.getEQ().getActiveParamName(index), view.activate));

  attachments.add(new SliderAttachment(state, processor.getEQ().getFrequencyParamName(index), view.frequency));
  attachments.add(new SliderAttachment(state, processor.getEQ().getQualityParamName(index), view.quality));
  attachments.add(new SliderAttachment(state, processor.getEQ().getGainParamName(index), view.gain));
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

void BandController::setFrequency(float freq) { view.frequency.setValue(freq, sendNotification); }

void BandController::setGain(float gainToUse) { view.gain.setValue(gainToUse, sendNotification); }

void BandController::setType(int type) { view.filterType.setSelectedId(type + 1, sendNotification); }

void BandController::buttonClicked(Button* b)
{
  if (b == &view.solo)
  {
    processor.getEQ().setBandSolo(view.solo.getToggleState() ? index : -1);
  }
}

} // namespace TA