/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayLineAudioProcessor::DelayLineAudioProcessor()
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
}

DelayLineAudioProcessor::~DelayLineAudioProcessor()
{
}

//==============================================================================
const String DelayLineAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayLineAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayLineAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayLineAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayLineAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayLineAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayLineAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayLineAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayLineAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayLineAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayLineAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    //********************************************************************************************//
    // 2) Initialize the variables that we are going to need in processBlock function: 
    // the buffer, the write and read pointer, the delay value
    
    dbuf.setSize(getTotalNumOutputChannels(), 100000);
    dbuf.clear(); 
    
    dw = 0;
    dr = 1;
    //********************************************************************************************//

}

void DelayLineAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayLineAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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


void DelayLineAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
    
    //********************************************************************************************//
    // 3) Delay line implementation
    int numSamples = buffer.getNumSamples();
    
    float wet_now = wet;
    float dry_now = dry;
    float SweepWidth_now = SweepWidth;
    float ph = 0.0;
    float frequency_now = LFO;
    float interpolatedSample = 0.0;
    float currentDelay = 0.0;
    float fraction = 0.0;
    float delayData[200000];
    float inverseSampleRate = 1.0/getSampleRate();
    
    float* channelOutDataL = buffer.getWritePointer(0);
    float* channelOutDataR = buffer.getWritePointer(1);
    
    const float* channelInData = buffer.getReadPointer(0);
    
    for (int i = 0; i < numSamples; ++i) {
        // setSample(int destChannel, int destSample, Type newValue)
        
        currentDelay = SweepWidth_now * (0.5 + 0.5 * sinf(2.0 * 3.14159 * ph));
        
        float dr = fmodf((float)dw - (float)(currentDelay * getSampleRate()) + (float)numSamples - 3.0, (float)numSamples);
        
        fraction = dr - floorf(dr);
        
        int previousSample = floorf(dr);
        
        int nextSample = (previousSample + 1) % numSamples;
        
        float interpolatedSample = fraction * delayData[nextSample] + (1.0-fraction)*delayData[previousSample];
        
        delayData[dw] = channelInData[i];
        
        if (++dw >= numSamples)
           
           dw = 0;   


        dbuf.setSample(0, dw, interpolatedSample);
        
        channelOutDataL[i] = dry_now * channelInData[i] + wet_now * dbuf.getSample(0, dr); 
        
        dbuf.setSample(1, dw, interpolatedSample);
        
        channelOutDataR[i] = dry_now * channelInData[i] + wet_now * dbuf.getSample(1, dr);
        
        
        ph += frequency_now*inverseSampleRate;
          if(ph >= 1.0)
               ph -= 1.0;
    }
    //********************************************************************************************//

    
}

//==============================================================================
bool DelayLineAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayLineAudioProcessor::createEditor()
{
    return new DelayLineAudioProcessorEditor (*this);
}

//==============================================================================
void DelayLineAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayLineAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
void DelayLineAudioProcessor::set_wet(float val)
{
    wet = val;
}
void DelayLineAudioProcessor::set_dry(float val)
{
    dry = val;
}
void DelayLineAudioProcessor::set_LFO(float val)
{
    LFO = val;
}
void DelayLineAudioProcessor::set_SweepWidth(float val)
{
    SweepWidth = val;
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayLineAudioProcessor();
}
