/*
  ==============================================================================

    This is the Frequalizer UI editor implementation

  ==============================================================================
*/

//#include "Analyser.h"
#include "FrequalizerEditor.h"
#include "FrequalizerProcessor.h"
#include "SocialButtons.h"

static int clickRadius = 10;
static float maxDB = 24.0f;

//==============================================================================
FrequalizerAudioProcessorEditor::FrequalizerAudioProcessorEditor(FrequalizerAudioProcessor& p)
  : AudioProcessorEditor(&p)
  , processor(p)
  , output(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)
  , plotView(processor.getEQ(), bandControllers)
  , modView(processor.modSource)
{
  tooltipWindow->setMillisecondsBeforeTipAppears(1000);

  addAndMakeVisible(socialButtons);


  for (int i = 0; i < processor.getEQ().getNumBands(); ++i)
  {
    auto* bandView = bandViews.add(new TA::BandView(i));
    bandControllers.add(new TA::BandController(i, processor, processor.getEQ(), *bandView));

    // Add lookAndFeel
    //bandView->setLookAndFeel(&tobanteLookAndFeel);
    addAndMakeVisible(bandView);
  }

  addAndMakeVisible(plotView);
  addAndMakeVisible(modView);

  frame.setText(translate("Output"));
  frame.setTextLabelPosition(Justification::centred);
  addAndMakeVisible(frame);
  addAndMakeVisible(output);
  attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.getPluginState(),
                                                                     TA::EqualizerProcessor::paramOutput, output));
  output.setTooltip(translate("Overall Gain"));

  setResizable(true, true);
  setResizeLimits(800, 450, 2990, 1800);
  setSize(1000, 750);


#ifdef JUCE_OPENGL
  openGLContext.attachTo(*getTopLevelComponent());
#endif


 
}

FrequalizerAudioProcessorEditor::~FrequalizerAudioProcessorEditor()
{
  PopupMenu::dismissAllActiveMenus();

#ifdef JUCE_OPENGL
  openGLContext.detach();
#endif
}

//==============================================================================
void FrequalizerAudioProcessorEditor::paint(Graphics& g)
{
  const Colour inputColour = Colours::greenyellow;
  const Colour outputColour = Colours::red;

  g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

  auto area = getLocalBounds();
  auto versionArea = area.removeFromBottom(static_cast<int>(area.getHeight() / 15 * 1));

  String version = JucePlugin_VersionString;
  g.setColour(Colours::white);
  g.setFont(18.f);
  g.drawText("modEQ v" + version, versionArea.reduced(10), Justification::centredTop);

}

void FrequalizerAudioProcessorEditor::resized()
{
  auto area = getLocalBounds();

  // Facebook & Gitub
  socialButtons.setBounds(area.removeFromBottom(35));
  
  
  auto modArea = area.removeFromBottom(getHeight() / 5);
  modView.setBounds(modArea.removeFromLeft(modArea.getWidth()/4));


  // EQ Bands
  auto bandSpace = area.removeFromBottom(getHeight() / 2);
  auto width = roundToInt(bandSpace.getWidth()) / (bandViews.size() + 1);
  for (auto* bandView : bandViews)
    bandView->setBounds(bandSpace.removeFromLeft(width));

  // Frame around output
  frame.setBounds(bandSpace.removeFromBottom(bandSpace.getHeight() / 2));
  output.setBounds(frame.getBounds().reduced(8));

  // FFT
  plotView.setBounds(area);


}







