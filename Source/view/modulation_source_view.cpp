/*
  ==============================================================================

    modulation_source_view.cpp
    Created: 12 Sep 2018 10:35:39pm
    Author:  tobante

  ==============================================================================
*/

#include "modulation_source_view.h"
#include "../../JuceLibraryCode/JuceHeader.h"


namespace TA
{

//==============================================================================
ModulationSourceView::ModulationSourceView(TA::ModulationSourceProcessor& p)
  : processor(p)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  startTimerHz(10);
}

ModulationSourceView::~ModulationSourceView() {}

void ModulationSourceView::paint(Graphics& g)
{
  const Colour inputColour = Colours::greenyellow;
  const Colour outputColour = Colours::red;

  Graphics::ScopedSaveState state(g);

  g.setFont(12.0f);
  g.setColour(Colours::silver);
  g.drawRoundedRectangle(plotFrame.toFloat(), 6, 5);
  //for (int i = 0; i < 10; ++i)
  //{
  //  g.setColour(Colours::silver.withAlpha(0.4f));
  //  auto x = plotFrame.getX() + plotFrame.getWidth() * i * 0.1f;
  //  if (i > 0)
  //    g.drawVerticalLine(roundToInt(x), (float)plotFrame.getY(), (float)plotFrame.getBottom());

  //  // g.setColour(Colour(0xffb9f6ca));
  //  // auto freq = getFrequencyForPosition(i * 0.1f);
  //  // g.drawFittedText((freq < 1000) ? String(freq) + " Hz" : String(freq / 1000, 1) + " kHz", roundToInt(x + 3),
  //  //                 plotFrame.getBottom() - 18, 50, 15, Justification::left, 1);
  //}

  //g.setColour(Colours::silver.withAlpha(0.4f));
  //g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.25 * plotFrame.getHeight()), (float)plotFrame.getX(),
  //                     (float)plotFrame.getRight());
  //g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.75 * plotFrame.getHeight()), (float)plotFrame.getX(),
  //                     (float)plotFrame.getRight());

  //g.reduceClipRegion(plotFrame);

  

  //
  // Input Analyser
  //
  g.setFont(16.0f);
  processor.createAnalyserPlot(modulationPath, plotFrame, 20.0f);
  g.drawFittedText("LFO", plotFrame.reduced(8), Justification::topRight, 1);
  g.setColour(Colour(0xff00ff08));
  g.strokePath(modulationPath, PathStrokeType(3.0));
}

void ModulationSourceView::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
  auto area = getLocalBounds();
  plotFrame = area.reduced(3, 3);
}

void ModulationSourceView::changeListenerCallback(ChangeBroadcaster* sender) { ignoreUnused(sender); }

void ModulationSourceView::timerCallback()
{
  if (processor.checkForNewAnalyserData())
    repaint(plotFrame);
}
} // namespace TA