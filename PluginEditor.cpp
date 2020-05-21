

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================


MagicChorusAudioProcessorEditor::MagicChorusAudioProcessorEditor (MagicChorusAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    
    getLookAndFeel().setColour (Slider::thumbColourId, Colours::white);
    
    mixSlider.setRange (0.0, 1.0);                                              //mix slider settings
    mixSlider.setSliderStyle(Slider::Rotary);
    mixSlider.setTextBoxStyle (Slider::NoTextBox, true, 100, 20);
    mixSlider.addListener(this);
    
    addAndMakeVisible (mixSlider);
    
    StereoSlider.setRange (0, 1);                                               //stereo slider settings
    StereoSlider.setSliderStyle(Slider::Rotary);
    StereoSlider.setTextBoxStyle (Slider::NoTextBox, false, 100, 20);
    StereoSlider.addListener(this);
    
    addAndMakeVisible (StereoSlider);
    
    LFO1Slider.setRange (0.1, 3.0);                                             //LFO1 slider settings
    LFO1Slider.setSliderStyle(Slider::Rotary);
    LFO1Slider.setTextBoxStyle (Slider::NoTextBox, false, 100, 20);
    LFO1Slider.addListener(this);
    
    addAndMakeVisible (LFO1Slider);
    
    SweepWidthSlider.setRange (1.0, 10.0);                                      //SweepWidth slider settings
    SweepWidthSlider.setSliderStyle(Slider::Rotary);
    SweepWidthSlider.setTextBoxStyle (Slider::NoTextBox, false, 100, 20);
    SweepWidthSlider.addListener(this);
    
    addAndMakeVisible (SweepWidthSlider);
    
    delaySlider.setRange (0.01, 0.03);                                          //delay slider settings
    delaySlider.setSliderStyle(Slider::Rotary);
    delaySlider.setTextBoxStyle (Slider::NoTextBox, false, 100, 20);
    delaySlider.addListener(this);
    
    addAndMakeVisible (delaySlider);
    
    setSize (800, 550);
}

MagicChorusAudioProcessorEditor::~MagicChorusAudioProcessorEditor()
{
}


void MagicChorusAudioProcessorEditor::paint (Graphics& g)
{
    Image background = ImageCache::getFromMemory (BinaryData::Background_jpg, BinaryData::Background_jpgSize);
    g.drawImageAt (background, 0, 0);
}

void MagicChorusAudioProcessorEditor::resized()
{
    
    delaySlider.setBounds (57, 330, 95, 95);
    
    LFO1Slider.setBounds (157, 260, 115, 115);
    
    mixSlider.setBounds (310, 340, 180, 180); 
        
    SweepWidthSlider.setBounds (525, 260, 115, 115);
    
    StereoSlider.setBounds (650, 330, 95, 95);
    
}

void MagicChorusAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    
   if (slider == &mixSlider){
       
        processor.set_mix(mixSlider.getValue());  
    }
    
    else if (slider == &StereoSlider){
        
        processor.set_Stereo(StereoSlider.getValue());
    }

    else if (slider == &LFO1Slider){

        processor.set_LFO1(LFO1Slider.getValue());
    }
    
    else if (slider == &delaySlider){
    
        processor.set_delay(delaySlider.getValue());     
    }

    else if (slider == &SweepWidthSlider){
     
        processor.set_SweepWidth(SweepWidthSlider.getValue());
    }

    else
        ;//DBG("unknown slider");
}
