/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SndelayAudioProcessor::SndelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    randomStore = new RandomStore();
    dman = new DelayManager(randomStore);
     
}

SndelayAudioProcessor::~SndelayAudioProcessor()
{
    delete randomStore;
    delete dman;
}

//==============================================================================
const String SndelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SndelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SndelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SndelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SndelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SndelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SndelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SndelayAudioProcessor::setCurrentProgram (int index)
{
}

const String SndelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void SndelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SndelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    dman->sampleRate = getSampleRate();
}

void SndelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SndelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

bool SndelayAudioProcessor::followEnvelopes(float sample) {
    bool flag = true;
    
    for (int i = 0; flag && i < (numberOfEnvelopes - 2); i++) {
        //samples[i].add(sample);
        float delta = sample - lastSample[i];
        int sign = delta / abs(delta);
        if (lastSign[i] == 1 && sign == -1) {
            flag = true;
        }
        else {
            flag = false;
        }
        lastSample[i] = sample;
        lastSign[i] = sign;
    }
    
    
    // We wish to find the minimum of the top envelope, as a splitting point.
    if (flag) {
        int i = numberOfEnvelopes - 2;
        //samples[i].add(sample);
        float delta = sample - lastSample[i];
        int sign = delta / abs(delta);
        if (lastSign[i] == -1 && sign == 1) {
            flag = true;
        }
        else {
            flag = false;
        }
        lastSample[i] = sample;
        lastSign[i] = sign;
    }
    // Hit a minimum in the top envelope
    if (flag) {
        //samples[numberOfEnvelopes - 1].add(sample);
        //std::cout << "Buffer Cleared" << std::endl;
        return true;
        
    }
    else {
        return false;
    }
}

float SndelayAudioProcessor::getDryGain() {
    jassert(-1 <= mix <= 1);
    if (mix < 0) {
        return 1;
    }
    else {
        return 1 - mix;
    }
}

float SndelayAudioProcessor::getWetGain() {
    jassert(-1 <= mix <= 1);
    if (mix > 0) {
        return 1;
    }
    else {
        return 1 + mix;
    }
}



void SndelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    playHead = this->getPlayHead();
    if(playHead != NULL) {
        playHead->getCurrentPosition(currentPositionInfo);
        dman->tempo = currentPositionInfo.bpm;
    }
    
    float* leftChannelData = buffer.getWritePointer(0); // Left channel only
    float* rightChannelData;
    int channels = buffer.getNumChannels();
    if (channels >= 2) {
        rightChannelData = buffer.getWritePointer(1);
    }
    
    int currentSampleIndex = 0;
    
    
    
    for (; currentSampleIndex < buffer.getNumSamples(); ++currentSampleIndex) {
        float sample = 0;
        for (int i = 0; i < buffer.getNumChannels(); i++) {
            sample += buffer.getSample(i, currentSampleIndex);
        }
        
        if (followEnvelopes(sample)) {
            dman->newLine();
        }
        StereoPair sndelay = dman->readWriteSample(sample);
        leftChannelData[currentSampleIndex] =
            std::get<0>(sndelay)*getWetGain() +
            buffer.getSample(0, currentSampleIndex)*getDryGain();
        if (channels >= 2) {
           rightChannelData[currentSampleIndex] =
                std::get<1>(sndelay)*getWetGain() +
                buffer.getSample(1, currentSampleIndex)*getDryGain();
        }
    }
}

//==============================================================================
bool SndelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SndelayAudioProcessor::createEditor()
{
    return new SndelayAudioProcessorEditor (*this);
}

//==============================================================================
void SndelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SndelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SndelayAudioProcessor();
}
