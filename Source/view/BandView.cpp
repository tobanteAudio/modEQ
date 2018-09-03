/*
  ==============================================================================

    BandView.cpp
    Created: 2 Sep 2018 6:07:18pm
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#include "BandView.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../FrequalizerProcessor.h"

//==============================================================================

namespace TA
{

BandView::BandView(int i)
  : index(i)
  , frequency(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
  , quality(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
  , gain(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
  , solo(translate("S"))
  , activate(translate("A"))
{
  frame.setText(String(index)); // processor.getBandName(index)
  frame.setTextLabelPosition(Justification::centred);
  frame.setColour(GroupComponent::textColourId, Colours::blue); // processor.getBandColour(index)
  frame.setColour(GroupComponent::outlineColourId, Colours::blue);
  addAndMakeVisible(frame);

  for (int j = 0; j < TA::EqualizerProcessor::LastFilterID; ++j)
    filterType.addItem(TA::EqualizerProcessor::getFilterTypeName(static_cast<TA::EqualizerProcessor::FilterType>(j)),
                       j + 1);

  addAndMakeVisible(filterType);


  addAndMakeVisible(frequency);
  frequency.setTooltip(translate("Filter's frequency"));

  addAndMakeVisible(quality);
  quality.setTooltip(translate("Filter's steepness (Quality)"));

  addAndMakeVisible(gain);
  gain.setTooltip(translate("Filter's gain"));

  solo.setClickingTogglesState(true);
  solo.setColour(TextButton::buttonOnColourId, Colours::yellow);
  addAndMakeVisible(solo);
  solo.setTooltip(translate("Listen only through this filter (solo)"));

  activate.setClickingTogglesState(true);
  activate.setColour(TextButton::buttonOnColourId, Colours::green);
  addAndMakeVisible(activate);
  activate.setTooltip(translate("Activate or deactivate this filter"));
}

BandView::~BandView() {}


void BandView::resized()
{
  auto bounds = getLocalBounds();
  frame.setBounds(bounds);

  bounds.reduce(10, 20);

  filterType.setBounds(bounds.removeFromTop(20));

  auto freqBounds = bounds.removeFromBottom(bounds.getHeight() * 2 / 4);
  frequency.setBounds(freqBounds.withTop(freqBounds.getY() + 10));

  auto buttons = freqBounds.reduced(5).withHeight(20);
  solo.setBounds(buttons.removeFromLeft(20));
  activate.setBounds(buttons.removeFromRight(20));

  quality.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
  gain.setBounds(bounds);
}
} // namespace TA