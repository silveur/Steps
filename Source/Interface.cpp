/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "Interface.h"

Interface::Interface(Sequencer* sequencer): theSequencer(sequencer)
{
	theMainScreen = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
	thePosition = theSequencer->getPosition();
	for(int i=0;i<16;i++)
	{
		addAndMakeVisible(theStepSliders.add(new Slider("Pitch" + String(i))));
		theStepSliders[i]->setSliderStyle (Slider::LinearBarVertical);
		theStepSliders[i]->setTextBoxIsEditable(false);
		theStepSliders[i]->addListener (this);
		theStepSliders[i]->setRange (-12, 12, 1);
		theStepSliders[i]->setValue(0);
		addAndMakeVisible(theVelocitySliders.add(new Slider("Velocity" + String(i))));
		theVelocitySliders[i]->setSliderStyle (Slider::Rotary);
		theVelocitySliders[i]->setTextBoxIsEditable(false);
		theVelocitySliders[i]->addListener (this);
		theVelocitySliders[i]->setRange (0, 127, 1);
		theVelocitySliders[i]->setValue(127);
		addAndMakeVisible(theStateButtons.add(new ToggleButton("State" + String(i))));
		theStateButtons[i]->addListener(this);
		theStateButtons[i]->setToggleState(true, dontSendNotification);
	}
	theMidiOutputList = new ComboBox("Midi Output list");
	refreshMidiList();
    setSize (theMainScreen.getWidth()/2, theMainScreen.getHeight()/4);
	startTimer(50);
}

Interface::~Interface()
{
	
}

void Interface::timerCallback()
{
	repaint();
}

void Interface::refreshMidiList()
{
    addAndMakeVisible(theMidiOutputList);
    StringArray midiList = theSequencer->getMidiCore()->getMidiDevicesList();
	theMidiOutputList->clear();
    for(int i=0;i<midiList.size();i++)
    {
        theMidiOutputList->addItem(midiList[i], i+1);
    }
	theMidiOutputList->setSelectedItemIndex(0);
}

void Interface::paint (Graphics& g)
{
	g.setColour(Colours::red);
	g.drawEllipse(theStepSliders[*thePosition]->getX(), theStateButtons[*thePosition]->getBottom(), 20, 20, 2);
}

void Interface::resized()
{
	for(int i=0;i<16;i++)
	{
		theStepSliders[i]->setBounds((getWidth()/16)*i, getHeight()/3, 20, 100);
		theVelocitySliders[i]->setBounds(theStepSliders[i]->getX(), theStepSliders[i]->getBottom(), 30, 30);
		theStateButtons[i]->setBounds(theStepSliders[i]->getX(), theVelocitySliders[i]->getBottom(), 30, 30);
	}
	theMidiOutputList->setBounds(30, 30, 150, 30);
}

void Interface::buttonClicked(Button* button)
{
	
}

void Interface::sliderValueChanged(Slider* slider)
{
	
}

void Interface::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == theMidiOutputList)
    {
        String midiOutString = theMidiOutputList->getItemText(theMidiOutputList->getSelectedItemIndex());
    }
}

