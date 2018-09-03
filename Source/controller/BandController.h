/*
  ==============================================================================

    BandController.h
    Created: 2 Sep 2018 6:06:55pm
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../FrequalizerProcessor.h"
#include "../view/BandView.h"

//==============================================================================
namespace TA
{

class BandController : public Button::Listener
{
public:
  //==============================================================================
  BandController(const int, FrequalizerAudioProcessor&, TA::EqualizerProcessor&, TA::BandView&);

  //==============================================================================
  void buttonClicked(Button* b) override;

  //==============================================================================
  void updateControls(TA::EqualizerProcessor::FilterType type);
  void updateSoloState(bool isSolo);

  //==============================================================================
  void setFrequency(float frequency);
  void setGain(float gain);
  void setType(int type);

  //==============================================================================
  Path frequencyResponse;

private:
  //==============================================================================
  int index;
  TA::BandView& view;
  FrequalizerAudioProcessor& processor;
  TA::EqualizerProcessor& subProcessor;

  //==============================================================================
  OwnedArray<AudioProcessorValueTreeState::ComboBoxAttachment> boxAttachments;
  OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
  OwnedArray<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandController)
};

} // namespace TA