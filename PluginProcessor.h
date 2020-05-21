
#pragma once

#include <JuceHeader.h>

//==============================================================================

class MagicChorusAudioProcessor  : public AudioProcessor
{
public:
   
    //==============================================================================
   
    MagicChorusAudioProcessor();
    ~MagicChorusAudioProcessor();

    //==============================================================================
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void set_mix(float val);
    void set_Stereo(float val);
    void set_LFO1(float val);
    void set_delay(float val);
    void set_SweepWidth(float val);
    
private:

    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagicChorusAudioProcessor)
    
    //********************************************************************************************//
    
    AudioSampleBuffer dbuf;
    int dbuf_writer;
    int dbuf_reader;
    int dbuf_size = 4096;
    
    float mix;
    float LFO1;
    float SweepWidth;
    float delay;
    float ph1;
    float ph2;
    float Stereo;
    
    //extra method
    float updatePhase1(float phase);
    float updatePhase2(float phase);
    float interpolateSample(float delay);
    
};