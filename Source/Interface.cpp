/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "Interface.h"

Interface::Interface(Sequencer* sequencer): theSequencer(sequencer)
{
	theMainScreen = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
	theSequencerTree = theSequencer->getSequencerTree();
	thePosition = theSequencerTree.getProperty("Position");
	for(int i=0;i<16;i++)
	{
		addAndMakeVisible(theStepSliders.add(new Slider("Pitch" + String(i))));
		theStepSliders[i]->setSliderStyle (Slider::LinearBarVertical);
		theStepSliders[i]->setTextBoxIsEditable(false);
		theStepSliders[i]->setRange (-12, 12, 1);
		theStepSliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Pitch"));
		theStepSliders[i]->addListener (this);
		addAndMakeVisible(theVelocitySliders.add(new Slider("Velocity" + String(i))));
		theVelocitySliders[i]->setSliderStyle (Slider::Rotary);
		theVelocitySliders[i]->setTextBoxIsEditable(false);
		theVelocitySliders[i]->setRange (0, 127, 1);
		theVelocitySliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Velocity"));
		theVelocitySliders[i]->addListener (this);
		addAndMakeVisible(theStateButtons.add(new ToggleButton("State" + String(i))));
		theStateButtons[i]->setToggleState((bool)theSequencerTree.getChild(i).getProperty("State"), dontSendNotification);
		theStateButtons[i]->addListener(this);
	}
	addAndMakeVisible(theSequencerLength = new Slider("Length"));
	theSequencerLength->setSliderStyle(Slider::LinearHorizontal);
	theSequencerLength->setRange(1, 16,1);
	theSequencerLength->setValue(16);
	theSequencerLength->addListener(this);
	
	theMidiOutputList = new ComboBox("Midi Output list");
	refreshMidiList();
	theSequencerTree.addListener(this);
	theMidiOutputList->addListener(this);
    setSize (theMainScreen.getWidth()/2, theMainScreen.getHeight()/4);
}

Interface::~Interface()
{
	
}

void Interface::handleAsyncUpdate()
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
		if (midiList[i] != "Sequencer")
        	theMidiOutputList->addItem(midiList[i], i+1);
    }
	theMidiOutputList->setSelectedItemIndex(0);
}

void Interface::paint (Graphics& g)
{
	g.setColour(Colours::red);
	g.drawEllipse(theStepSliders[thePosition]->getX(), theStateButtons[thePosition]->getBottom(), 20, 20, 2);
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
	int index = button->getName().getTrailingIntValue();
	theSequencerTree.getChild(index).setProperty("State", (bool)button->getToggleState(), nullptr);
}

void Interface::sliderValueChanged(Slider* slider)
{
	int index = slider->getName().getTrailingIntValue();
	if(slider->getName().contains("Pitch"))
	{
		theSequencerTree.getChild(index).setProperty("Pitch", (int)slider->getValue(), nullptr);
	}
	else if(slider->getName().contains("Velocity"))
	{
		theSequencerTree.getChild(index).setProperty("Velocity", (int)slider->getValue(), nullptr);
	}
	else if(slider == theSequencerLength)
	{
		theSequencerTree.setProperty("Length", slider->getValue(), nullptr);
	}
}

void Interface::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == theMidiOutputList)
    {
        String midiOutString = theMidiOutputList->getItemText(theMidiOutputList->getSelectedItemIndex());
		theSequencerTree.setProperty("MidiOutput", midiOutString, nullptr);
    }
}

void Interface::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
	if(String(property) == "Position")
	{
		thePosition = tree.getProperty(property);
		triggerAsyncUpdate();
	}
}



