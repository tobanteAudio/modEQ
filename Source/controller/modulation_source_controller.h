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

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../modEQ_processor.h"
#include "../processor/modulation_source_processor.h"
#include "../view/modulation_source_view.h"

//==============================================================================
namespace TA
{

class ModulationSourceController : public ChangeListener, public Timer
{
public:
  //==============================================================================
  ModulationSourceController(const int, ModEQProcessor&, TA::ModulationSourceProcessor&, TA::ModulationSourceView&);

  //==============================================================================
  void changeListenerCallback(ChangeBroadcaster* sender) override;
  void timerCallback() override;

private:
  //==============================================================================
  int index;
  ModEQProcessor& mainProcessor;
  TA::ModulationSourceProcessor& processor;
  TA::ModulationSourceView& view;

  //==============================================================================
  OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceController)
};

} // namespace TA
