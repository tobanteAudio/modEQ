/*
  ==============================================================================

    modulation_source_controller.cpp
    Created: 15 Sep 2018 10:26:37am
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#include "modulation_source_controller.h"

namespace TA
{

ModulationSourceController::ModulationSourceController(const int i,
                                                       ModulationSourceProcessor& p,
                                                       ModulationSourceView& v)
  : index(i)
  , processor(p)
  , view(v)
{
  startTimerHz(25);
}

void ModulationSourceController::changeListenerCallback(ChangeBroadcaster* sender) {}

void ModulationSourceController::timerCallback()
{
  if (processor.checkForNewAnalyserData())
    processor.createAnalyserPlot(view.modulationPath, view.plotFrame, 20.0f);
  view.repaint(view.plotFrame);
}


} // namespace TA