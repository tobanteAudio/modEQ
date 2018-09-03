/*
  ==============================================================================

    This is the Frequalizer implementation

  ==============================================================================
*/

#include "FrequalizerProcessor.h"
#include "FrequalizerEditor.h"
#include "SocialButtons.h"


//==============================================================================
FrequalizerAudioProcessor::FrequalizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
  : AudioProcessor(BusesProperties()
                     .withInput("Input", AudioChannelSet::stereo(), true)
                     .withOutput("Output", AudioChannelSet::stereo(), true))
  ,
#else
  :
#endif
  state(*this, &undo)
  , eqProcessor(state)
{
  const float maxGain = Decibels::decibelsToGain(24.0f);

  state.createAndAddParameter(
    TA::EqualizerProcessor::paramOutput, TRANS("Output"), TRANS("Output level"),
    NormalisableRange<float>(0.0f, 2.0f, 0.01f), 1.0f,
    [](float value) { return String(Decibels::gainToDecibels(value), 1) + " dB"; },
    [](String text) { return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); }, false, true,
    false);


  state.addParameterListener(TA::EqualizerProcessor::paramOutput, this);

  state.state = ValueTree(JucePlugin_Name);
}

FrequalizerAudioProcessor::~FrequalizerAudioProcessor() {}

//==============================================================================
const String FrequalizerAudioProcessor::getName() const { return JucePlugin_Name; }

bool FrequalizerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool FrequalizerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

//==============================================================================
bool FrequalizerAudioProcessor::isMidiEffect() const { return false; }
double FrequalizerAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int FrequalizerAudioProcessor::getNumPrograms() { return 1; }
int FrequalizerAudioProcessor::getCurrentProgram() { return 0; }
void FrequalizerAudioProcessor::setCurrentProgram(int) {}
const String FrequalizerAudioProcessor::getProgramName(int) { return {}; }
void FrequalizerAudioProcessor::changeProgramName(int, const String&) {}

//==============================================================================
void FrequalizerAudioProcessor::prepareToPlay(double newSampleRate, int newSamplesPerBlock)
{
  sampleRate = newSampleRate;

  dsp::ProcessSpec spec;
  spec.sampleRate = newSampleRate;
  spec.maximumBlockSize = uint32(newSamplesPerBlock);
  spec.numChannels = uint32(getTotalNumOutputChannels());

  eqProcessor.prepareToPlay(newSampleRate, newSamplesPerBlock);
  eqProcessor.prepare(spec);
  outputGain.prepare(spec);
  outputGain.setGainLinear(*state.getRawParameterValue(TA::EqualizerProcessor::paramOutput));
}

void FrequalizerAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FrequalizerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  // This checks if the input layout matches the output layout
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;

  return true;
}
#endif

void FrequalizerAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
  ScopedNoDenormals noDenormals;
  ignoreUnused(midiMessages);

  // if (getActiveEditor() != nullptr)
  //  inputAnalyser.addAudioData(buffer, 0, getTotalNumInputChannels());

  eqProcessor.processBlock(buffer, midiMessages);

  dsp::AudioBlock<float> ioBuffer(buffer);
  dsp::ProcessContextReplacing<float> context(ioBuffer);
  outputGain.process(context);


  // if (getActiveEditor() != nullptr)
  //  outputAnalyser.addAudioData(buffer, 0, getTotalNumOutputChannels());
}


void FrequalizerAudioProcessor::parameterChanged(const String& parameter, float newValue)
{
  if (parameter == TA::EqualizerProcessor::paramOutput)
  {
    outputGain.setGainLinear(newValue);
    return;
  }
}


//==============================================================================
bool FrequalizerAudioProcessor::hasEditor() const { return true; }

AudioProcessorEditor* FrequalizerAudioProcessor::createEditor() { return new FrequalizerAudioProcessorEditor(*this); }


//==============================================================================
void FrequalizerAudioProcessor::getStateInformation(MemoryBlock& destData)
{
  // MemoryOutputStream stream(destData, false);
  // state.state.writeToStream (stream);
}

void FrequalizerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  // ValueTree tree = ValueTree::readFromData (data, size_t (sizeInBytes));
  // if (tree.isValid()) {
  //    state.state = tree;
  //}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new FrequalizerAudioProcessor(); }
