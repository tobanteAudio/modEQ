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

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../modEQ_processor.h"
#include "../view/band_view.h"

//==============================================================================
namespace TA
{

class BandController : public Button::Listener
{
public:
  //==============================================================================
  BandController(int, ModEQProcessor&, TA::EqualizerProcessor&, TA::BandView&);

  //==============================================================================
  void buttonClicked(Button* b) override;

  //==============================================================================
  void updateControls(TA::EqualizerProcessor::FilterType type);
  void updateSoloState(bool isSolo);

  //==============================================================================
  void setFrequency(float newFreq);
  void setGain(float newGain);
  void setType(int newType);

  //==============================================================================
  Path frequencyResponse;

private:
  //==============================================================================
  int index;
  TA::BandView& view;
  ModEQProcessor& mainProcessor;
  TA::EqualizerProcessor& processor;

  //==============================================================================
  OwnedArray<AudioProcessorValueTreeState::ComboBoxAttachment> boxAttachments;
  OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
  OwnedArray<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandController)
};

} // namespace TA