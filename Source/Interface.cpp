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
	theSequencerLength->setValue(theSequencerTree.getProperty("Length"));
	theSequencerLength->addListener(this);
	
	addAndMakeVisible(theRandomAllButton = new TextButton("Random all"));
	theRandomAllButton->addListener(this);
	
	addAndMakeVisible(theShuffleSlider = new Slider("Shuffle"));
	theShuffleSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
	theShuffleSlider->setRange(0, 6, 1);
	theShuffleSlider->setSliderStyle(Slider::SliderStyle::Rotary);
	theShuffleSlider->addListener(this);
	
	addAndMakeVisible(theMidiOutputList = new ComboBox("Midi Output list"));
	refreshMidiList();
	String str = theSequencerTree.getProperty("MidiOutput");
	updateSelectedMidiOut(str);
	addAndMakeVisible(theRootNoteList = new ComboBox("RootNoteList"));
	addAndMakeVisible(theRootOctaveList = new ComboBox("RootOctaveList"));
	updateNotesAndOctaves();
	theRootNoteList->setSelectedItemIndex(theSequencerTree.getProperty("RootNote"));
	theRootOctaveList->setSelectedItemIndex(theSequencerTree.getProperty("RootOctave"));
	theRootNoteList->addListener(this);
	theRootOctaveList->addListener(this);
	theSequencerTree.addListener(this);
	theMidiOutputList->addListener(this);
    setSize(theMainScreen.getWidth()/2, theMainScreen.getHeight()/3);
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
	theSequencerLength->setBounds(200, 20, 200, 30);
	theMidiOutputList->setBounds(30, 20, 150, 30);
	theRootNoteList->setBounds(30, 50, 70, 20);
	theRootOctaveList->setBounds(theRootNoteList->getRight(), theRootNoteList->getY(), theRootNoteList->getWidth(), theRootNoteList->getHeight());
	theRandomAllButton->setBounds(theSequencerLength->getRight(), 30, 90, 20);
	theShuffleSlider->setBounds(200, 50, 30, 30);
}

void Interface::buttonClicked(Button* button)
{
	if (button == theRandomAllButton)
	{
		for (int i=0;i<16;i++)
		{
			ValueTree child = theSequencerTree.getChild(i);
			child.setProperty("Pitch", ((int)rand() % 24) - 12, nullptr);
			child.setProperty("State", rand() % 2, nullptr);
			child.setProperty("Velocity", ((int)rand() % 127), nullptr);
		}
	}
	else
	{
		int index = button->getName().getTrailingIntValue();
		theSequencerTree.getChild(index).setProperty("State", (bool)button->getToggleState(), nullptr);
	}

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
	else if(slider == theShuffleSlider)
	{
		theSequencerTree.setProperty("Shuffle", slider->getValue(), nullptr);
	}
}

void Interface::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == theMidiOutputList)
    {
        String midiOutString = theMidiOutputList->getItemText(theMidiOutputList->getSelectedItemIndex());
		theSequencerTree.setProperty("MidiOutput", midiOutString, nullptr);
    }
	else if(comboBoxThatHasChanged == theRootOctaveList)
    {
        int id = comboBoxThatHasChanged->getSelectedItemIndex();
		theSequencerTree.setProperty("RootOctave", id, nullptr);
    }
	else if(comboBoxThatHasChanged == theRootNoteList)
    {
        int id = comboBoxThatHasChanged->getSelectedItemIndex();
		theSequencerTree.setProperty("RootNote", id, nullptr);
    }
}

void Interface::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
	if(String(property) == "Position")
	{
		thePosition = tree.getProperty(property);
		triggerAsyncUpdate();
	}
	else if(String(property) == "MidiOutput")
	{
		String midiOutput = tree.getProperty("MidiOutput");
		updateSelectedMidiOut(midiOutput);
	}
	else
	{
		for (int i=0; i<16;i++)
		{
			if (tree == (theSequencerTree.getChild(i)))
			{
				if (String(property) == "Pitch")
					theStepSliders[i]->setValue((int)tree.getProperty(property));
				else if (String(property) == "State")
					theStateButtons[i]->setToggleState((bool)tree.getProperty(property), dontSendNotification);
				else if (String(property) == "Velocity")
					theVelocitySliders[i]->setValue((int)tree.getProperty(property));
			}
		}
	}
}

void Interface::updateSelectedMidiOut(String& midiOut)
{
	refreshMidiList();
	for (int i=0;i<theMidiOutputList->getNumItems();i++)
	{
		if (theMidiOutputList->getItemText(i) == midiOut)
		{
			theMidiOutputList->setSelectedItemIndex(i);
			break;
		}
	}
}

void Interface::updateNotesAndOctaves()
{
	theRootNoteList->addItem("C",1);
	theRootNoteList->addItem("C#",2);
	theRootNoteList->addItem("D",3);
	theRootNoteList->addItem("D#",4);
	theRootNoteList->addItem("E",5);
	theRootNoteList->addItem("F",6);
	theRootNoteList->addItem("F#",7);
	theRootNoteList->addItem("G",8);
	theRootNoteList->addItem("G#",9);
	theRootNoteList->addItem("A",10);
	theRootNoteList->addItem("A#",11);
	theRootNoteList->addItem("B",12);
	
	for (int i=0;i<8;i++)
	{
		theRootOctaveList->addItem(String(i), i+1);
	}
}


