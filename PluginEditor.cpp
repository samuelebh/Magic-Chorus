/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayLineAudioProcessorEditor::DelayLineAudioProcessorEditor (DelayLineAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    wetSlider.setRange (0.0, 1.0);
    wetSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
    wetSlider.addListener(this);
    wetLabel.setText ("Wet Level", dontSendNotification);
    
    addAndMakeVisible (wetSlider);
    addAndMakeVisible (wetLabel);
    
    drySlider.setRange (0.0, 1.0);
    drySlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
    drySlider.addListener(this);
    dryLabel.setText ("Dry Level", dontSendNotification);
    
    addAndMakeVisible (drySlider);
    addAndMakeVisible (dryLabel);
    
    
    LFOSlider.setRange (0.0, 18.0, 0.5);
    LFOSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
    LFOSlider.addListener(this);
    
    addAndMakeVisible (LFOSlider);
    addAndMakeVisible (LFOLabel);
    
    SweepWidthSlider.setRange (0.0, 2.0);
    SweepWidthSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
    SweepWidthSlider.addListener(this);
    
    addAndMakeVisible (SweepWidthSlider);
    addAndMakeVisible (SweepWidthLabel);
    
    setSize (600, 400);
}

DelayLineAudioProcessorEditor::~DelayLineAudioProcessorEditor()
{
}

//==============================================================================
void DelayLineAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void DelayLineAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    wetLabel.setBounds (10, 10, 90, 20);
    wetSlider.setBounds (100, 10, getWidth() - 110, 20);
    
    dryLabel.setBounds (10, 50, 90, 20);
    drySlider.setBounds (100, 50, getWidth() - 110, 20);
    
    LFOLabel.setBounds (10, 90, 90, 20);
    LFOSlider.setBounds (100, 90, getWidth() - 110, 20);
    
    SweepWidthLabel.setBounds (10, 130, 90, 20);
    SweepWidthSlider.setBounds (100, 130, getWidth() - 110, 20);
    
}

void DelayLineAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &wetSlider)
        processor.set_wet(wetSlider.getValue());
    else if (slider == &drySlider)
        processor.set_dry(drySlider.getValue());
    else if (slider == &LFOSlider)
        processor.set_LFO(LFOSlider.getValue());
    else if (slider == &SweepWidthSlider)
        processor.set_SweepWidth(SweepWidthSlider.getValue());
}

