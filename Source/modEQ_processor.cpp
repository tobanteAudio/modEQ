/*
  ==============================================================================

    This is the Frequalizer implementation

  ==============================================================================
*/

#include "modEQ_processor.h"
#include "modEQ_editor.h"
#include "view/social_buttons.h"


//==============================================================================
ModEQProcessor::ModEQProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
  : AudioProcessor(BusesProperties()
                     .withInput("Input", AudioChannelSet::stereo(), true)
                     .withOutput("Output", AudioChannelSet::stereo(), true))
  ,
#else
  :
#endif
  state(*this, &undo)
  , equalizerProcessor(state)
  , modSource(state)
{
  const float maxGain = Decibels::decibelsToGain(24.0f);

  state.createAndAddParameter(TA::EqualizerProcessor::paramOutput, translate("Output"), translate("Output level"),
                              NormalisableRange<float>(0.0f, 2.0f, 0.01f), 1.0f, gainTextConverter, gainTextConverter,
                              false, true, false);


  state.addParameterListener(TA::EqualizerProcessor::paramOutput, this);

  state.state = ValueTree(JucePlugin_Name);
}

ModEQProcessor::~ModEQProcessor() {}

//==============================================================================
const String ModEQProcessor::getName() const { return JucePlugin_Name; }

bool ModEQProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool ModEQProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

//==============================================================================
bool ModEQProcessor::isMidiEffect() const { return false; }
double ModEQProcessor::getTailLengthSeconds() const { return 0.0; }
int ModEQProcessor::getNumPrograms() { return 1; }
int ModEQProcessor::getCurrentProgram() { return 0; }
void ModEQProcessor::setCurrentProgram(int) {}
const String ModEQProcessor::getProgramName(int) { return {}; }
void ModEQProcessor::changeProgramName(int, const String&) {}

//==============================================================================
void ModEQProcessor::prepareToPlay(double newSampleRate, int newSamplesPerBlock)
{
  sampleRate = newSampleRate;
  modBuffer.setSize(1, newSamplesPerBlock, false, false, true);
  
  dsp::ProcessSpec spec;
  spec.sampleRate = newSampleRate;
  spec.maximumBlockSize = uint32(newSamplesPerBlock);
  spec.numChannels = uint32(getTotalNumOutputChannels());
  
  modSource.prepareToPlay(sampleRate, newSamplesPerBlock);
  equalizerProcessor.prepareToPlay(newSampleRate, newSamplesPerBlock);
  equalizerProcessor.prepare(spec);
  outputGain.prepare(spec);

  outputGain.setGainLinear(*state.getRawParameterValue(TA::EqualizerProcessor::paramOutput));
}

void ModEQProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ModEQProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  // This checks if the input layout matches the output layout
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;

  return true;
}
#endif

void ModEQProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
  ignoreUnused(midiMessages);
  ScopedNoDenormals noDenormals;

  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  // if (getActiveEditor() != nullptr) {}

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  modBuffer.clear();
  modSource.processBlock(modBuffer, midiMessages);

  equalizerProcessor.processBlock(buffer, midiMessages);
  
  dsp::AudioBlock<float> ioBuffer(buffer);
  dsp::ProcessContextReplacing<float> context(ioBuffer);
  outputGain.process(context);
}


void ModEQProcessor::parameterChanged(const String& parameter, float newValue)
{
  if (parameter == TA::EqualizerProcessor::paramOutput)
  {
    outputGain.setGainLinear(newValue);
    return;
  }
}


//==============================================================================
bool ModEQProcessor::hasEditor() const { return true; }

AudioProcessorEditor* ModEQProcessor::createEditor() { return new ModEQEditor(*this); }


//==============================================================================
void ModEQProcessor::getStateInformation(MemoryBlock& destData)
{
   MemoryOutputStream stream(destData, false);
   state.state.writeToStream (stream);
}

void ModEQProcessor::setStateInformation(const void* data, int sizeInBytes)
{
   ValueTree tree = ValueTree::readFromData (data, size_t (sizeInBytes));
   if (tree.isValid()) {
      state.state = tree;
  }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ModEQProcessor(); }