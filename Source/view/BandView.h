/*
  ==============================================================================

    BandView.h
    Created: 2 Sep 2018 6:07:18pm
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
 */
namespace TA
{

class BandView : public Component
{
public:
  //==============================================================================
  BandView(int);
  ~BandView();

  //==============================================================================
  void resized() override;

  //==============================================================================
  GroupComponent frame;
  ComboBox filterType;
  Slider frequency;
  Slider quality;
  Slider gain;
  TextButton solo;
  TextButton activate;

private:
  //==============================================================================
  int index;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandView)
};

} // namespace TA