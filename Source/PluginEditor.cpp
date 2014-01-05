/*
  ==============================================================================

	Author:  Silvere Letellier

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

SequencerAudioProcessorEditor::SequencerAudioProcessorEditor (SequencerAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
	theAudioConfig = getProcessor()->getValueTree();
	for(int i=0;i<NUM_CHANNELS_MAX;i++)
	{
		addAndMakeVisible(theStepSliders.add(new Slider("Pitch" + String(i))));
		theStepSliders[i]->setSliderStyle (Slider::LinearBarVertical);
		theStepSliders[i]->setTextBoxIsEditable(false);
		theStepSliders[i]->addListener (this);
		theStepSliders[i]->setRange (-12, 12, 1);
		theStepSliders[i]->setValue(theAudioConfig.getChild(i).getProperty("Pitch"));
		addAndMakeVisible(theVelocitySliders.add(new Slider("Velocity" + String(i))));
		theVelocitySliders[i]->setSliderStyle (Slider::Rotary);
		theVelocitySliders[i]->setTextBoxIsEditable(false);
		theVelocitySliders[i]->addListener (this);
		theVelocitySliders[i]->setRange (0, 127, 1);
		theVelocitySliders[i]->setValue(theAudioConfig.getChild(i).getProperty("Velocity"));
		addAndMakeVisible(theStateButtons.add(new ToggleButton("State" + String(i))));
		theStateButtons[i]->addListener(this);
		theStateButtons[i]->setToggleState((bool)theAudioConfig.getChild(i).getProperty("State"), dontSendNotification);
	}
	addAndMakeVisible(theSequencerLength = new Slider("Length"));
	theSequencerLength->setSliderStyle(Slider::LinearHorizontal);
	theSequencerLength->setRange(1, 16,1);
	theSequencerLength->setValue(theAudioConfig.getProperty("Length"));
	theSequencerLength->addListener(this);
	addAndMakeVisible(theCreateMidiPortButton = new TextButton("Create Virtual Midi bus"));
	theCreateMidiPortButton->addListener(this);
	theMidiOutputList = new ComboBox("Midi Output list");
	refreshMidiList();
	thePosition = 0;
    setSize (600, 300);
	theAudioConfig.addListener(this);
	startTimer(50);
}

SequencerAudioProcessorEditor::~SequencerAudioProcessorEditor()
{
    stopTimer();
}

void SequencerAudioProcessorEditor::refreshMidiList()
{
    addAndMakeVisible(theMidiOutputList);
    StringArray midiList = getProcessor()->theMidiCore->getMidiDevicesList();
	theMidiOutputList->clear();
    for(int i=0;i<midiList.size();i++)
    {
        theMidiOutputList->addItem(midiList[i], i+1);
    }
    theMidiOutputList->setSelectedItemIndex(theAudioConfig.getProperty("MidiOutput"));
    theMidiOutputList->addListener(this);
}

void SequencerAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
	g.setColour(Colours::red); 
	g.drawEllipse(theStepSliders[thePosition]->getX(), theStateButtons[thePosition]->getBottom(), 20, 20, 2);
}

void SequencerAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	int index = slider->getName().getTrailingIntValue();
	if(slider->getName().contains("Pitch"))
	{
		getProcessor()->setParameterNotifyingHost(index, slider->getValue());
	}
	else if(slider->getName().contains("Velocity"))
	{
		getProcessor()->setParameterNotifyingHost(index + NUM_CHANNELS_MAX, slider->getValue());
	}
	else if(slider == theSequencerLength)
	{
		getProcessor()->setParameterNotifyingHost(49, slider->getValue());
	}
}
void SequencerAudioProcessorEditor::buttonClicked(Button* button)
{
	if(button == theCreateMidiPortButton)
	{
		getProcessor()->theMidiCore->createVirtualMidiBus();
	}
	else
	{
		int index = button->getName().getTrailingIntValue();
		getProcessor()->setParameterNotifyingHost(NUM_CHANNELS_MAX*2 + index, (float)button->getToggleState());
	}
}

void SequencerAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == theMidiOutputList)
    {
        String midiOutString = theMidiOutputList->getItemText(theMidiOutputList->getSelectedItemIndex());
        getProcessor()->setParameterNotifyingHost(50, theMidiOutputList->getSelectedItemIndex());
        theAudioConfig.setProperty("MidiOutput", theMidiOutputList->getSelectedItemIndex(), getProcessor()->theUndoManager);
    }
}

void SequencerAudioProcessorEditor::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
	int index = treeWhosePropertyHasChanged.getParent().indexOf(treeWhosePropertyHasChanged);
	if(String(property) == "Pitch")
	{
		theStepSliders[index]->setValue(treeWhosePropertyHasChanged.getProperty(property));
	}
	else if(String(property) == "Velocity")
	{
		theVelocitySliders[index]->setValue(treeWhosePropertyHasChanged.getProperty(property));
	}
	else if(String(property) == "State")
	{
		theStateButtons[index]->setToggleState((bool)treeWhosePropertyHasChanged.getProperty(property),dontSendNotification);
	}
    else if(String(property) == "MidiOutput")
	{
		theMidiOutputList->setSelectedItemIndex(treeWhosePropertyHasChanged.getProperty(property),dontSendNotification);
	}
	else if(String(property) == "Length")
	{
		theSequencerLength->setValue(treeWhosePropertyHasChanged.getProperty(property),dontSendNotification);
	}
}

void SequencerAudioProcessorEditor::timerCallback()
{
	int numMidi = ((StringArray)getProcessor()->theMidiCore->getMidiDevicesList()).size();
	if(numMidi != theMidiOutputList->getNumItems())
		refreshMidiList();
	thePosition = getProcessor()->theSequencerPosition;
	repaint();
}
void SequencerAudioProcessorEditor::resized()
{
	for(int i=0;i<NUM_CHANNELS_MAX;i++)
	{
		theStepSliders[i]->setBounds((getWidth()/16)*i, getHeight()/3, 20, 100);
		theVelocitySliders[i]->setBounds(theStepSliders[i]->getX(), theStepSliders[i]->getBottom(), 30, 30);
		theStateButtons[i]->setBounds(theStepSliders[i]->getX(), theVelocitySliders[i]->getBottom(), 30, 30);
	}
    theMidiOutputList->setBounds(30, 30, 150, 30);
	theSequencerLength->setBounds(200, 30, 200, 20);
	theCreateMidiPortButton->setBounds(theSequencerLength->getRight(), 30, 60, 15);
}





