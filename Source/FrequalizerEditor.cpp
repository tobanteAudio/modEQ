/*
  ==============================================================================

    This is the Frequalizer UI editor implementation

  ==============================================================================
*/

//#include "Analyser.h"
#include "FrequalizerEditor.h"
#include "FrequalizerProcessor.h"
#include "SocialButtons.h"

static int clickRadius = 4;
static float maxDB = 24.0f;

//==============================================================================
FrequalizerAudioProcessorEditor::FrequalizerAudioProcessorEditor(FrequalizerAudioProcessor& p)
  : AudioProcessorEditor(&p)
  , processor(p)
  , output(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
  , bandView(1)
{
  tooltipWindow->setMillisecondsBeforeTipAppears(1000);

  addAndMakeVisible(socialButtons);
  addAndMakeVisible(bandView);

/*
  for (int i = 0; i < processor.getNumBands(); ++i)
  {
    auto* bandEditor = bandEditors.add(new TA::BandEditor(i, processor));
    addAndMakeVisible(bandEditor);
  }*/

  for (int i = 0; i < processor.getNumBands(); ++i)
  {
    auto* bandView = bandViews.add(new TA::BandView(i));
	auto* bandController = bandControllers.add(new TA::BandController(i,processor, *bandView));
    addAndMakeVisible(bandView);
  }

  frame.setText(translate("Output"));
  frame.setTextLabelPosition(Justification::centred);
  addAndMakeVisible(frame);
  addAndMakeVisible(output);
  attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.getPluginState(),
                                                                     FrequalizerAudioProcessor::paramOutput, output));
  output.setTooltip(translate("Overall Gain"));

  setResizable(true, true);
  setResizeLimits(800, 450, 2990, 1800);
  setSize(900, 500);

  updateFrequencyResponses();

#ifdef JUCE_OPENGL
  openGLContext.attachTo(*getTopLevelComponent());
#endif

  processor.addChangeListener(this);

  startTimerHz(30);
}

FrequalizerAudioProcessorEditor::~FrequalizerAudioProcessorEditor()
{
  PopupMenu::dismissAllActiveMenus();

  processor.removeChangeListener(this);
#ifdef JUCE_OPENGL
  openGLContext.detach();
#endif
}

//==============================================================================
void FrequalizerAudioProcessorEditor::paint(Graphics& g)
{
  const Colour inputColour = Colours::greenyellow;
  const Colour outputColour = Colours::red;

  Graphics::ScopedSaveState state(g);

  g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

  auto logo = ImageCache::getFromMemory(TobanteAudioData::LogoFF_png, TobanteAudioData::LogoFF_pngSize);
  g.drawImage(logo, brandingFrame.toFloat(), RectanglePlacement(RectanglePlacement::fillDestination));

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

  for (int i = 0; i < processor.getNumBands(); ++i)
  {
    auto* bandEditor = bandViews.getUnchecked(i);
    //auto* bandEditor = bandEditors.getUnchecked(i);
    auto* band = processor.getBand(i);

    //
    // Draw individual freq responses
    //
    // g.setColour (band->active ? band->colour : band->colour.withAlpha (0.3f));
    // g.strokePath (bandEditor->frequencyResponse, PathStrokeType (1.0));
    // g.setColour (draggingBand == i ? band->colour : band->colour.withAlpha (0.3f));
    // auto x = roundToInt (plotFrame.getX() + plotFrame.getWidth() * getPositionForFrequency (float
    // (band->frequency))); auto y = roundToInt (getPositionForGain (float (band->gain), (float)plotFrame.getY(),
    // (float)plotFrame.getBottom())); g.drawVerticalLine (x, (float)plotFrame.getY(), (float(y - 5)));
    // g.drawVerticalLine (x, (float(y + 5)), (float)plotFrame.getBottom());
    // g.fillEllipse ((float(x - 3)), (float(y - 3)), (float)6, (float)6);
  }
  g.setColour(Colours::cadetblue);
  g.strokePath(frequencyResponse, PathStrokeType(3.0));
}

void FrequalizerAudioProcessorEditor::resized()
{
  auto area = getLocalBounds();
  plotFrame = area.reduced(3, 3);

  socialButtons.setBounds(plotFrame.removeFromBottom(35));

  auto bandSpace = plotFrame.removeFromBottom(getHeight() / 2);
  auto width = roundToInt(bandSpace.getWidth()) / (bandViews.size() + 1);
  for (auto* bandView : bandViews)
    bandView->setBounds(bandSpace.removeFromLeft(width));

  frame.setBounds(bandSpace.removeFromTop(bandSpace.getHeight() / 2));
  output.setBounds(frame.getBounds().reduced(8));

  plotFrame.reduce(3, 3);
  brandingFrame = bandSpace.reduced(5);

  updateFrequencyResponses();
}

void FrequalizerAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* sender)
{
  ignoreUnused(sender);
  updateFrequencyResponses();
  repaint();
}

void FrequalizerAudioProcessorEditor::timerCallback()
{
  if (processor.checkForNewAnalyserData())
    repaint(plotFrame);
}

void FrequalizerAudioProcessorEditor::mouseDown(const MouseEvent& e)
{
  if (e.mods.isPopupMenu() && plotFrame.contains(e.x, e.y))
    for (int i = 0; i < bandViews.size(); ++i)
      if (auto* band = processor.getBand(i))
      {
        if (std::abs(plotFrame.getX() + getPositionForFrequency(int(band->frequency)) * plotFrame.getWidth()
                     - e.position.getX())
            < clickRadius)
        {
          contextMenu.clear();
          for (int t = 0; t < FrequalizerAudioProcessor::LastFilterID; ++t)
            contextMenu.addItem(
              t + 1,
              FrequalizerAudioProcessor::getFilterTypeName(static_cast<FrequalizerAudioProcessor::FilterType>(t)), true,
              band->type == t);

          contextMenu.showMenuAsync(
            PopupMenu::Options().withTargetComponent(this).withTargetScreenArea({e.getScreenX(), e.getScreenY(), 1, 1}),
            [this, i](int selected) {
              if (selected > 0)
                bandControllers.getUnchecked(i)->setType(selected - 1);
            });
        }
      }
}

void FrequalizerAudioProcessorEditor::mouseMove(const MouseEvent& e)
{
  if (plotFrame.contains(e.x, e.y))
  {
    for (int i = 0; i < bandControllers.size(); ++i)
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

void FrequalizerAudioProcessorEditor::mouseDrag(const MouseEvent& e)
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

void FrequalizerAudioProcessorEditor::mouseDoubleClick(const MouseEvent& e)
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

void FrequalizerAudioProcessorEditor::updateFrequencyResponses()
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

float FrequalizerAudioProcessorEditor::getPositionForFrequency(float freq)
{
  return (std::log(freq / 20.0f) / std::log(2.0f)) / 10.0f;
}

float FrequalizerAudioProcessorEditor::getFrequencyForPosition(float pos)
{
  return 20.0f * std::pow(2.0f, pos * 10.0f);
}

float FrequalizerAudioProcessorEditor::getPositionForGain(float gain, float top, float bottom)
{
  return jmap(Decibels::gainToDecibels(gain, -maxDB), -maxDB, maxDB, bottom, top);
}

float FrequalizerAudioProcessorEditor::getGainForPosition(float pos, float top, float bottom)
{
  return Decibels::decibelsToGain(jmap(pos, bottom, top, -maxDB, maxDB), -maxDB);
}


//==============================================================================
//namespace TA
//{
//BandEditor::BandEditor(const int i, FrequalizerAudioProcessor& p)
//  : index(i)
//  , processor(p)
//  , frequency(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
//  , quality(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
//  , gain(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
//  , solo(translate("S"))
//  , activate(translate("A"))
//{
//  frame.setText(processor.getBandName(index));
//  frame.setTextLabelPosition(Justification::centred);
//  frame.setColour(GroupComponent::textColourId, processor.getBandColour(index));
//  frame.setColour(GroupComponent::outlineColourId, processor.getBandColour(index));
//  addAndMakeVisible(frame);
//
//  for (int j = 0; j < FrequalizerAudioProcessor::LastFilterID; ++j)
//    filterType.addItem(
//      FrequalizerAudioProcessor::getFilterTypeName(static_cast<FrequalizerAudioProcessor::FilterType>(j)), j + 1);
//
//  addAndMakeVisible(filterType);
//  boxAttachments.add(new AudioProcessorValueTreeState::ComboBoxAttachment(
//    processor.getPluginState(), processor.getTypeParamName(index), filterType));
//
//  addAndMakeVisible(frequency);
//  attachments.add(new AudioProcessorValueTreeState::SliderAttachment(
//    processor.getPluginState(), processor.getFrequencyParamName(index), frequency));
//  frequency.setSkewFactorFromMidPoint(1000.0);
//  frequency.setTooltip(translate("Filter's frequency"));
//
//  addAndMakeVisible(quality);
//  attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.getPluginState(),
//                                                                     processor.getQualityParamName(index), quality));
//  quality.setSkewFactorFromMidPoint(1.0);
//  quality.setTooltip(translate("Filter's steepness (Quality)"));
//
//  addAndMakeVisible(gain);
//  attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.getPluginState(),
//                                                                     processor.getGainParamName(index), gain));
//  gain.setSkewFactorFromMidPoint(1.0);
//  gain.setTooltip(translate("Filter's gain"));
//
//  solo.setClickingTogglesState(true);
//  solo.addListener(this);
//  solo.setColour(TextButton::buttonOnColourId, Colours::yellow);
//  addAndMakeVisible(solo);
//  solo.setTooltip(translate("Listen only through this filter (solo)"));
//
//  activate.setClickingTogglesState(true);
//  activate.setColour(TextButton::buttonOnColourId, Colours::green);
//  buttonAttachments.add(new AudioProcessorValueTreeState::ButtonAttachment(
//    processor.getPluginState(), processor.getActiveParamName(index), activate));
//  addAndMakeVisible(activate);
//  activate.setTooltip(translate("Activate or deactivate this filter"));
//}
//
//void BandEditor::resized()
//{
//  auto bounds = getLocalBounds();
//  frame.setBounds(bounds);
//
//  bounds.reduce(10, 20);
//
//  filterType.setBounds(bounds.removeFromTop(20));
//
//  auto freqBounds = bounds.removeFromBottom(bounds.getHeight() * 2 / 4);
//  frequency.setBounds(freqBounds.withTop(freqBounds.getY() + 10));
//
//  auto buttons = freqBounds.reduced(5).withHeight(20);
//  solo.setBounds(buttons.removeFromLeft(20));
//  activate.setBounds(buttons.removeFromRight(20));
//
//  quality.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
//  gain.setBounds(bounds);
//}
//
//void BandEditor::updateControls(FrequalizerAudioProcessor::FilterType type)
//{
//  switch (type)
//  {
//  case FrequalizerAudioProcessor::LowPass:
//    frequency.setEnabled(true);
//    quality.setEnabled(true);
//    gain.setEnabled(false);
//    break;
//  case FrequalizerAudioProcessor::LowPass1st:
//    frequency.setEnabled(true);
//    quality.setEnabled(false);
//    gain.setEnabled(false);
//    break;
//  case FrequalizerAudioProcessor::LowShelf:
//    frequency.setEnabled(true);
//    quality.setEnabled(false);
//    gain.setEnabled(true);
//    break;
//  case FrequalizerAudioProcessor::BandPass:
//    frequency.setEnabled(true);
//    quality.setEnabled(true);
//    gain.setEnabled(false);
//    break;
//  case FrequalizerAudioProcessor::AllPass:
//    frequency.setEnabled(true);
//    quality.setEnabled(false);
//    gain.setEnabled(false);
//    break;
//  case FrequalizerAudioProcessor::AllPass1st:
//    frequency.setEnabled(true);
//    quality.setEnabled(false);
//    gain.setEnabled(false);
//    break;
//  case FrequalizerAudioProcessor::Notch:
//    frequency.setEnabled(true);
//    quality.setEnabled(true);
//    gain.setEnabled(false);
//    break;
//  case FrequalizerAudioProcessor::Peak:
//    frequency.setEnabled(true);
//    quality.setEnabled(true);
//    gain.setEnabled(true);
//    break;
//  case FrequalizerAudioProcessor::HighShelf:
//    frequency.setEnabled(true);
//    quality.setEnabled(true);
//    gain.setEnabled(true);
//    break;
//  case FrequalizerAudioProcessor::HighPass1st:
//    frequency.setEnabled(true);
//    quality.setEnabled(false);
//    gain.setEnabled(false);
//    break;
//  case FrequalizerAudioProcessor::HighPass:
//    frequency.setEnabled(true);
//    quality.setEnabled(true);
//    gain.setEnabled(false);
//    break;
//  default:
//    frequency.setEnabled(true);
//    quality.setEnabled(true);
//    gain.setEnabled(true);
//    break;
//  }
//}
//
//void BandEditor::updateSoloState(bool isSolo) { solo.setToggleState(isSolo, dontSendNotification); }
//
//void BandEditor::setFrequency(float freq) { frequency.setValue(freq, sendNotification); }
//
//void BandEditor::setGain(float gainToUse) { gain.setValue(gainToUse, sendNotification); }
//
//void BandEditor::setType(int type) { filterType.setSelectedId(type + 1, sendNotification); }
//
//void BandEditor::buttonClicked(Button* b)
//{
//  if (b == &solo)
//  {
//    processor.setBandSolo(solo.getToggleState() ? index : -1);
//  }
//}
//
//
//} // namespace TA