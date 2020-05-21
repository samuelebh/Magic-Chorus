
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================

class OtherLookAndFeel : public LookAndFeel_V4
{
public:
    OtherLookAndFeel()
    {
        setColour (Slider::thumbColourId, Colours::red);
    }
};

class MagicChorusAudioProcessorEditor  : public AudioProcessorEditor,  private Slider::Listener 
{
public:
    MagicChorusAudioProcessorEditor (MagicChorusAudioProcessor&);
    ~MagicChorusAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    
    MagicChorusAudioProcessor& processor;
    
    Slider mixSlider;
    Label mixLabel;
    
    Slider LFO1Slider;
    Label LFO1Label;
    
    Slider SweepWidthSlider;
    Label SweepWidthLabel;
    
    Slider StereoSlider;
    Label StereoLabel;
    
    Slider delaySlider;
    Label delayLabel;
    
    void sliderValueChanged(Slider* slider) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagicChorusAudioProcessorEditor)
};

