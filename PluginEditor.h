/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayLineAudioProcessorEditor  : public AudioProcessorEditor,  private Slider::Listener
{
public:
    DelayLineAudioProcessorEditor (DelayLineAudioProcessor&);
    ~DelayLineAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayLineAudioProcessor& processor;
    
    Slider wetSlider;
    Label wetLabel;
    Slider drySlider;
    Label dryLabel;
    Slider LFOSlider;
    Label LFOLabel;
    Slider SweepWidthSlider;
    Label SweepWidthLabel;
    
    void sliderValueChanged(Slider* slider) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayLineAudioProcessorEditor)
};
