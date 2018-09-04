/*
  ==============================================================================

    EqualizerPlotView.cpp
    Created: 4 Sep 2018 8:21:00pm
    Author:  Tobias Hienzsch

  ==============================================================================
*/

#include "EqualizerPlotView.h"
#include "../../JuceLibraryCode/JuceHeader.h"

namespace TA
{

static int clickRadius = 10;
static float maxDB = 24.0f;

//==============================================================================
EqualizerPlotView::EqualizerPlotView(TA::EqualizerProcessor& p, OwnedArray<TA::BandController>& bands)
  : processor(p)
  , bandControllers(bands)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  processor.addChangeListener(this);

  //#ifdef JUCE_OPENGL
  //  openGLContext.attachTo(*getTopLevelComponent());
  //#endif

  startTimerHz(30);
}

EqualizerPlotView::~EqualizerPlotView()
{
  processor.removeChangeListener(this);

  //#ifdef JUCE_OPENGL
  //  openGLContext.detach();
  //#endif
}

void EqualizerPlotView::paint(Graphics& g)
{
  const Colour inputColour = Colours::greenyellow;
  const Colour outputColour = Colours::red;

  Graphics::ScopedSaveState state(g);

  g.setFont(12.0f);
  g.setColour(Colours::silver);
  g.drawRoundedRectangle(plotFrame.toFloat(), 5, 2);
  for (int i = 0; i < 10; ++i)
  {
    g.setColour(Colours::silver.withAlpha(0.3f));
    auto x = plotFrame.getX() + plotFrame.getWidth() * i * 0.1f;
    if (i > 0)
      g.drawVerticalLine(roundToInt(x), (float)plotFrame.getY(), (float)plotFrame.getBottom());

    g.setColour(Colours::silver);
    auto freq = getFrequencyForPosition(i * 0.1f);
    g.drawFittedText((freq < 1000) ? String(freq) + " Hz" : String(freq / 1000, 1) + " kHz", roundToInt(x + 3),
                     plotFrame.getBottom() - 18, 50, 15, Justification::left, 1);
  }

  g.setColour(Colours::silver.withAlpha(0.3f));
  g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.25 * plotFrame.getHeight()), (float)plotFrame.getX(),
                       (float)plotFrame.getRight());
  g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.75 * plotFrame.getHeight()), (float)plotFrame.getX(),
                       (float)plotFrame.getRight());

  g.setColour(Colours::silver);
  g.drawFittedText(String(maxDB) + " dB", plotFrame.getX() + 3, plotFrame.getY() + 2, 50, 14, Justification::left, 1);
  g.drawFittedText(String(maxDB / 2) + " dB", plotFrame.getX() + 3,
                   roundToInt(plotFrame.getY() + 2 + 0.25 * plotFrame.getHeight()), 50, 14, Justification::left, 1);
  g.drawFittedText(" 0 dB", plotFrame.getX() + 3, roundToInt(plotFrame.getY() + 2 + 0.5 * plotFrame.getHeight()), 50,
                   14, Justification::left, 1);
  g.drawFittedText(String(-maxDB / 2) + " dB", plotFrame.getX() + 3,
                   roundToInt(plotFrame.getY() + 2 + 0.75 * plotFrame.getHeight()), 50, 14, Justification::left, 1);

  g.reduceClipRegion(plotFrame);

  Path analyser;
  g.setFont(16.0f);
  processor.createAnalyserPlot(analyser, plotFrame, 20.0f, true);
  //
  // Input Analyser
  //
  // g.setColour (inputColour);
  // g.drawFittedText ("Input", plotFrame.reduced (8), Justification::topRight, 1);
  // g.strokePath (analyser, PathStrokeType (1.0));
  processor.createAnalyserPlot(analyser, plotFrame, 20.0f, false);
  g.setColour(outputColour);
  g.drawFittedText("Output", plotFrame.reduced(8, 28), Justification::topRight, 1);
  g.strokePath(analyser, PathStrokeType(2.0));

  // for (int i = 0; i < processor.getEQ().getNumBands(); ++i)
  //{
  //  auto* bandEditor = bandViews.getUnchecked(i);
  //  // auto* bandEditor = bandEditors.getUnchecked(i);
  //  auto* band = processor.getEQ().getBand(i);

  //  //
  //  // Draw individual freq responses
  //  //
  //  // g.setColour (band->active ? band->colour : band->colour.withAlpha (0.3f));
  //  // g.strokePath (bandEditor->frequencyResponse, PathStrokeType (1.0));
  //  // g.setColour (draggingBand == i ? band->colour : band->colour.withAlpha (0.3f));
  //  // auto x = roundToInt (plotFrame.getX() + plotFrame.getWidth() * getPositionForFrequency (float
  //  // (band->frequency))); auto y = roundToInt (getPositionForGain (float (band->gain), (float)plotFrame.getY(),
  //  // (float)plotFrame.getBottom())); g.drawVerticalLine (x, (float)plotFrame.getY(), (float(y - 5)));
  //  // g.drawVerticalLine (x, (float(y + 5)), (float)plotFrame.getBottom());
  //  // g.fillEllipse ((float(x - 3)), (float(y - 3)), (float)6, (float)6);
  //}
  g.setColour(Colours::cadetblue);
  g.strokePath(frequencyResponse, PathStrokeType(3.0));
}

void EqualizerPlotView::resized()
{
  auto area = getLocalBounds();
  plotFrame = area.reduced(3, 3);
  updateFrequencyResponses();
}

void EqualizerPlotView::changeListenerCallback(ChangeBroadcaster* sender)
{
  ignoreUnused(sender);
  updateFrequencyResponses();
  repaint();
}

void EqualizerPlotView::timerCallback()
{
  if (processor.checkForNewAnalyserData())
    repaint(plotFrame);
}

void EqualizerPlotView::mouseDown(const MouseEvent& e)
{
  if (e.mods.isPopupMenu() && plotFrame.contains(e.x, e.y))
    for (int i = 0; i < bandControllers.size(); ++i)
      if (auto* band = processor.getBand(i))
      {
        if (std::abs(plotFrame.getX() + getPositionForFrequency(int(band->frequency)) * plotFrame.getWidth()
                     - e.position.getX())
            < clickRadius)
        {
          contextMenu.clear();
          for (int t = 0; t < TA::EqualizerProcessor::LastFilterID; ++t)
            contextMenu.addItem(
              t + 1, TA::EqualizerProcessor::getFilterTypeName(static_cast<TA::EqualizerProcessor::FilterType>(t)),
              true, band->type == t);

          contextMenu.showMenuAsync(
            PopupMenu::Options().withTargetComponent(this).withTargetScreenArea({e.getScreenX(), e.getScreenY(), 1, 1}),
            [this, i](int selected) {
              if (selected > 0)
                bandControllers.getUnchecked(i)->setType(selected - 1);
            });
        }
      }
}

void EqualizerPlotView::mouseMove(const MouseEvent& e)
{
  if (plotFrame.contains(e.x, e.y))
  {
    for (int i = 0; i < bandControllers.size(); ++i) //
    {
      if (auto* band = processor.getBand(i))
      {
        auto pos = plotFrame.getX() + getPositionForFrequency(float(band->frequency)) * plotFrame.getWidth();
        if (std::abs(pos - e.position.getX()) < clickRadius)
        {
          if (std::abs(getPositionForGain(float(band->gain), plotFrame.getY(), plotFrame.getBottom())
                       - e.position.getY())
              < clickRadius)
          {
            draggingGain = processor.getPluginState().getParameter(processor.getGainParamName(i));
            setMouseCursor(MouseCursor(MouseCursor::UpDownLeftRightResizeCursor));
          }
          else
          {
            setMouseCursor(MouseCursor(MouseCursor::LeftRightResizeCursor));
          }
          if (i != draggingBand)
          {
            draggingBand = i;
            repaint(plotFrame);
          }
          return;
        }
      }
    }
  }
  draggingBand = -1;
  draggingGain = false;
  setMouseCursor(MouseCursor(MouseCursor::NormalCursor));
  repaint(plotFrame);
}

void EqualizerPlotView::mouseDrag(const MouseEvent& e)
{
  if (isPositiveAndBelow(draggingBand, bandControllers.size()))
  {
    auto pos = (e.position.getX() - plotFrame.getX()) / plotFrame.getWidth();
    bandControllers[draggingBand]->setFrequency(getFrequencyForPosition(pos));
    if (draggingGain)
      bandControllers[draggingBand]->setGain(
        getGainForPosition(e.position.getY(), (float)plotFrame.getY(), (float)plotFrame.getBottom()));
  }
}

void EqualizerPlotView::mouseDoubleClick(const MouseEvent& e)
{
  if (plotFrame.contains(e.x, e.y))
  {
    for (int i = 0; i < bandControllers.size(); ++i)
    {
      if (auto* band = processor.getBand(i))
      {
        if (std::abs(plotFrame.getX() + getPositionForFrequency(float(band->frequency)) * plotFrame.getWidth()
                     - e.position.getX())
            < clickRadius)
        {
          if (auto* param = processor.getPluginState().getParameter(processor.getActiveParamName(i)))
            param->setValueNotifyingHost(param->getValue() < 0.5f ? 1.0f : 0.0f);
        }
      }
    }
  }
}

void EqualizerPlotView::updateFrequencyResponses()
{
  auto pixelsPerDouble = 2.0f * plotFrame.getHeight() / Decibels::decibelsToGain(maxDB);

  for (int i = 0; i < bandControllers.size(); ++i)
  {
    auto* bandController = bandControllers.getUnchecked(i);
    bandController->updateSoloState(i);
    if (auto* band = processor.getBand(i))
    {
      bandController->updateControls(band->type);
      bandController->frequencyResponse.clear();
      processor.createFrequencyPlot(bandController->frequencyResponse, band->magnitudes,
                                    plotFrame.withX(plotFrame.getX() + 1), pixelsPerDouble);
    }
    bandController->updateSoloState(processor.getBandSolo(i));
  }
  frequencyResponse.clear();
  processor.createFrequencyPlot(frequencyResponse, processor.getMagnitudes(), plotFrame, pixelsPerDouble);
}

float EqualizerPlotView::getPositionForFrequency(float freq)
{
  return (std::log(freq / 20.0f) / std::log(2.0f)) / 10.0f;
}

float EqualizerPlotView::getFrequencyForPosition(float pos) { return 20.0f * std::pow(2.0f, pos * 10.0f); }

float EqualizerPlotView::getPositionForGain(float gain, float top, float bottom)
{
  return jmap(Decibels::gainToDecibels(gain, -maxDB), -maxDB, maxDB, bottom, top);
}

float EqualizerPlotView::getGainForPosition(float pos, float top, float bottom)
{
  return Decibels::decibelsToGain(jmap(pos, bottom, top, -maxDB, maxDB), -maxDB);
}

} // namespace TA