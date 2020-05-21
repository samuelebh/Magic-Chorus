

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================


MagicChorusAudioProcessorEditor::MagicChorusAudioProcessorEditor (MagicChorusAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    
    getLookAndFeel().setColour (Slider::thumbColourId, Colours::white);
    //getLookAndFeel().drawRotarySlider (
    
    //drawLinearSlider(Graphics&, 10, 10, 15, 15, 
    
    mixSlider.setRange (0.0, 1.0);                                              //mix slider settings
    mixSlider.setSliderStyle(Slider::Rotary);
    mixSlider.setTextBoxStyle (Slider::NoTextBox, true, 100, 20);
    mixSlider.addListener(this);
    mixLabel.setText ("Mix Level", dontSendNotification);
    //mixSlider.setColour( Slider::thumbColourId, Colours::white);
    //mixSlider.setLookAndFeel (&otherLookAndFeel);
    //mixSlider.fillEllipse;
    //mixSlider.setDragMode(Slider::absoluteDrag);
    
    addAndMakeVisible (mixSlider);
    //addAndMakeVisible (mixLabel);
    
    StereoSlider.setRange (0, 1);                                               //stereo slider settings
    StereoSlider.setSliderStyle(Slider::Rotary);
    StereoSlider.setTextBoxStyle (Slider::NoTextBox, false, 100, 20);
    StereoSlider.addListener(this);
    StereoLabel.setText ("Spatial", dontSendNotification);
    
    addAndMakeVisible (StereoSlider);
    addAndMakeVisible (StereoLabel);
    
    LFO1Slider.setRange (0.1, 3.0);                                             //LFO1 slider settings
    LFO1Slider.setSliderStyle(Slider::Rotary);
    LFO1Slider.setTextBoxStyle (Slider::NoTextBox, false, 100, 20);
    LFO1Slider.addListener(this);
    LFO1Label.setText ("Rate", dontSendNotification);
    
    addAndMakeVisible (LFO1Slider);
    addAndMakeVisible (LFO1Label);
    
    SweepWidthSlider.setRange (1.0, 10.0);                                      //SweepWidth slider settings
    SweepWidthSlider.setSliderStyle(Slider::Rotary);
    SweepWidthSlider.setTextBoxStyle (Slider::NoTextBox, false, 100, 20);
    SweepWidthSlider.addListener(this);
    SweepWidthLabel.setText("Width", dontSendNotification);
    
    addAndMakeVisible (SweepWidthSlider);
    addAndMakeVisible (SweepWidthLabel);
    
    delaySlider.setRange (0.01, 0.03);                                          //delay slider settings
    delaySlider.setSliderStyle(Slider::Rotary);
    delaySlider.setTextBoxStyle (Slider::NoTextBox, false, 100, 20);
    delaySlider.addListener(this);
    delayLabel.setText ("Delay", dontSendNotification);
    
    addAndMakeVisible (delaySlider);
    addAndMakeVisible (delayLabel);
    
    setSize (800, 550);
}

MagicChorusAudioProcessorEditor::~MagicChorusAudioProcessorEditor()
{
}


void MagicChorusAudioProcessorEditor::paint (Graphics& g)
{
    Image background = ImageCache::getFromMemory (BinaryData::Background_jpg, BinaryData::Background_jpgSize);
    g.drawImageAt (background, 0, 0);
    
    /*g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);*/
}

void MagicChorusAudioProcessorEditor::resized()
{
    
    //delayLabel.setBounds (10, 190, 100, 20);
    delaySlider.setBounds (57, 330, 95, 95);
    
    //LFO1Label.setBounds (10, 230, 90, 20);
    LFO1Slider.setBounds (157, 260, 115, 115);
    
    //mixLabel.setBounds (10, 150, 90, 4);
        mixSlider.setBounds (310, 340, 180, 180); 
        
        //mixSlider.draw
    
    //SweepWidthLabel.setBounds (10, 270, 90, 20);
    SweepWidthSlider.setBounds (525, 260, 115, 115);
    
    //StereoLabel.setBounds (10, 310, 90, 20);
    StereoSlider.setBounds (650, 330, 95, 95);
    
}

void MagicChorusAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    
   if (slider == &mixSlider){
        //DBG("wet slider change");
        processor.set_mix(mixSlider.getValue());  
    }
    
    else if (slider == &StereoSlider){
        //DBG("dry slider change");
        processor.set_Stereo(StereoSlider.getValue());
    }

    else if (slider == &LFO1Slider){
        //DBG("dry slider change");
        processor.set_LFO1(LFO1Slider.getValue());
    }

   /* else if (slider == &LFO2Slider){
        //DBG("LFO slider change");
        processor.set_LFO2(LFO2Slider.getValue());     
    }*/
    
    else if (slider == &delaySlider){
        //DBG("delay slider change");
        processor.set_delay(delaySlider.getValue());     
    }

    else if (slider == &SweepWidthSlider){
        //DBG("sweep width slider change");
        processor.set_SweepWidth(SweepWidthSlider.getValue());
    }

    else
        ;//DBG("unknown slider");
}
