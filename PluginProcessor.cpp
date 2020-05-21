
#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================

MagicChorusAudioProcessor::MagicChorusAudioProcessor()
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
    //init slider value to avoid garbage
    
    mix = 0.1;
    LFO1 = 0.1;
    SweepWidth = 1.0;
    Stereo=0;
    delay=0.020;
    
    dbuf = AudioSampleBuffer(2, dbuf_size);     //allocate a buffer of 0.33s at 48000Hz
    dbuf_writer = 0;
    dbuf_reader = 0;
    
    ph1 = 0.0;                                  //phase in oposition
    ph2 = 0.5;   
}


MagicChorusAudioProcessor::~MagicChorusAudioProcessor()
{
}

//==============================================================================

const String MagicChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MagicChorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MagicChorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MagicChorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MagicChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MagicChorusAudioProcessor::getNumPrograms()
{
    return 1;   
}

int MagicChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MagicChorusAudioProcessor::setCurrentProgram (int index)
{
}

const String MagicChorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void MagicChorusAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================



void MagicChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //********************************************************************************************//
    
    dbuf.clear(); 
    
    //********************************************************************************************//
}



void MagicChorusAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MagicChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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




float MagicChorusAudioProcessor::interpolateSample(float delay){
    
    float dbuf_reader = fmodf((float)dbuf_writer - (float)(delay * getSampleRate()) + (float)dbuf_size - 3.0, (float)dbuf_size);
    float fraction = dbuf_reader - floorf(dbuf_reader);        
    int previousSample = floorf(dbuf_reader);
        
    int nextSample = (previousSample + 1) % dbuf_size;
        
    float interpolatedSample = fraction * dbuf.getSample(0, nextSample) + (1.0-fraction) * dbuf.getSample(0, previousSample);
    
        return interpolatedSample;
}

float MagicChorusAudioProcessor::updatePhase1(float ph){
    
    float inverseSampleRate = 1.0/getSampleRate();
    float frequency_now_1 = LFO1+(Stereo/20.0);
    ph += frequency_now_1*inverseSampleRate;
    
    if(ph >= 1.0)
       ph -= 1.0;
   
        return ph;
}

float MagicChorusAudioProcessor::updatePhase2(float ph){
    
    float inverseSampleRate = 1.0/getSampleRate();
    float frequency_now_2 = LFO1-(Stereo/20.0);
    ph += frequency_now_2*inverseSampleRate;
    
    if(ph >= 1.0)
       ph -= 1.0;
    
       return ph;
}



void MagicChorusAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //********************************************************************************************//
  
    int numSamples = buffer.getNumSamples();

    //initialization
    float mix_now = mix;
    float SweepWidth_now = SweepWidth;
    float currentDelay1 = 0.0;
    float currentDelay2 = 0.0;
    
    float* channelOutDataL = buffer.getWritePointer(0);
    float* channelOutDataR = buffer.getWritePointer(1);
    
    float frequency_now_1 = LFO1+(Stereo/20.0);     //LFO frequencies are controlled by the parameter "Rate" (LFO1)                                                    
    float frequency_now_2 = LFO1-(Stereo/20.0);     //and their difference is controlled by the parameter "Spatial" (Stereo)
    
    float min_delay1 = delay; 
    float min_delay2 = delay/(1+(Stereo/2.0));     //delay difference between R and L channels depending on stereo effect
    
    const float* channelInData = buffer.getReadPointer(0);
    
    //map sweepwidth from time in s to time in ms
    SweepWidth_now = SweepWidth_now * 0.001;
    
    
    for (int i = 0; i < numSamples; ++i) { 
        
        //current delay in s
        currentDelay1 = min_delay1 + SweepWidth_now * (0.5 + 0.5 * sinf(2.0 * 3.14159 * ph1));
        currentDelay2 = min_delay2 + SweepWidth_now * (0.5 + 0.5 * sinf(2.0 * 3.14159 * ph2));
    
        
        float interpolatedSample1 = (2 - mix_now) * channelInData[i] + interpolateSample(currentDelay1) * mix_now;
        float interpolatedSample2 = (2 - mix_now) * channelInData[i] + interpolateSample(currentDelay2) * mix_now;
        float interpolatedSample = (interpolatedSample1 + interpolatedSample2) / 2.0;
        

        //output
        channelOutDataL[i] = (1 - Stereo) * interpolatedSample/2 + Stereo * interpolatedSample1/2; 
        channelOutDataR[i] = (1 - Stereo) * interpolatedSample/2 + Stereo * interpolatedSample2/2;
        
        if(Stereo<=0.5){    
            ph1 = updatePhase1(ph1);
            ph2 = updatePhase1(ph2);   
        }
        else if (Stereo>=0.5){
            ph1 = updatePhase1(ph1);
            ph2 = updatePhase2(ph2);
        }                  
           
        dbuf.setSample(0,dbuf_writer, channelInData[i]);
        dbuf.setSample(1,dbuf_writer, channelInData[i]);
        dbuf_writer  = (dbuf_writer + 1 )% dbuf_size;
    }
    
}



//==============================================================================

bool MagicChorusAudioProcessor::hasEditor() const
{
    return true; 
}

AudioProcessorEditor* MagicChorusAudioProcessor::createEditor()
{
    return new MagicChorusAudioProcessorEditor (*this);
}

//==============================================================================
void MagicChorusAudioProcessor::getStateInformation (MemoryBlock& destData)
{    
}

void MagicChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{   
}


//==============================================================================



void MagicChorusAudioProcessor::set_mix(float val)
{
    mix = val;
}
void MagicChorusAudioProcessor::set_Stereo(float val)
{
    Stereo = val;
}
void MagicChorusAudioProcessor::set_LFO1(float val)
{
    LFO1 = val;
}

void MagicChorusAudioProcessor::set_delay(float val)
{
    delay = val;
}

void MagicChorusAudioProcessor::set_SweepWidth(float val)
{
    SweepWidth = val;
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MagicChorusAudioProcessor();
}