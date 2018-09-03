/*
  ==============================================================================

    SocialButtons.h
    Created: 25 Feb 2018 1:59:43pm
    Author:  Dobby

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
 */
class SocialButtons : public Component, public Button::Listener
{
public:
  SocialButtons()
  {
    setOpaque(false);
	   
    auto* b = buttons.add(new ImageButton());
    b->addListener(this);
    auto fbLogo = ImageCache::getFromMemory(TobanteAudioData::FBlogo_png, TobanteAudioData::FBlogo_pngSize);
    b->setImages(false, true, true, fbLogo, 1.0f, Colours::transparentWhite, fbLogo, 0.7f, Colours::transparentWhite,
                 fbLogo, 0.7f, Colours::transparentWhite);
    b->setComponentID("https://www.fb.com/tobanteAudio/");
    b->setTooltip(TRANS("Find us on Facebook"));
    addAndMakeVisible(b);


    b = buttons.add(new ImageButton());
    b->addListener(this);
    auto githubLogo = ImageCache::getFromMemory(TobanteAudioData::GitHublogo_png, TobanteAudioData::GitHublogo_pngSize);
    b->setImages(false, true, true, githubLogo, 1.0f, Colours::transparentWhite, githubLogo, 0.7f,
                 Colours::transparentWhite, githubLogo, 0.7f, Colours::transparentWhite);
    b->setComponentID("https://github.com/tobanteAudio");
    b->setTooltip(TRANS("Find resources on Github"));
    addAndMakeVisible(b);
  }

  ~SocialButtons() {}

  void paint(Graphics& g) override { ignoreUnused(g); }

  void resized() override
  {
    auto bounds = getLocalBounds();
    for (auto* b : buttons)
      b->setBounds(bounds.removeFromLeft(bounds.getHeight()).reduced(3));
  }

  void buttonClicked(Button* b) override
  {
    URL url(b->getComponentID());
    if (url.isWellFormed())
    {
      url.launchInDefaultBrowser();
    }
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SocialButtons)

  OwnedArray<ImageButton> buttons;
};
