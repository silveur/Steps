/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "SequencerView.h"
#include "ControllerView.h"

extern UndoManager* theUndoManager;
extern File thePresetFolder;

SequencerView::SequencerView(ValueTree& sequencerTree, ControllerView* controllerView): theControllerView(controllerView), showPopUp(true)
{
	theSequencerTree = sequencerTree;
	thePosition = theSequencerTree.getProperty("Position");
	for(int i=0;i<16;i++)
	{
		addAndMakeVisible(theStepSliders.add(new Slider("Pitch" + String(i))));
		theStepSliders[i]->setSliderStyle(Slider::RotaryVerticalDrag);
		theStepSliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
		theStepSliders[i]->setTextBoxIsEditable(false);
		theStepSliders[i]->setDoubleClickReturnValue(true, 0);
		theStepSliders[i]->setRange(0- 12 * (int)theSequencerTree.getProperty("Range"), 12 * (int)theSequencerTree.getProperty("Range"), 1);
		theStepSliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Pitch"));
		theStepSliders[i]->addListener (this);
		addAndMakeVisible(theVelocitySliders.add(new Slider("Velocity" + String(i))));
		theVelocitySliders[i]->setSliderStyle(Slider::RotaryVerticalDrag);
		theVelocitySliders[i]->setTextBoxIsEditable(false);
		theVelocitySliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
		theVelocitySliders[i]->setRange(0, 127, 1);
		theVelocitySliders[i]->setTextValueSuffix(" Velocity");
		theVelocitySliders[i]->setPopupDisplayEnabled(true, theControllerView);
		theVelocitySliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Velocity"));
		theVelocitySliders[i]->addListener(this);
		addAndMakeVisible(theStateButtons.add(new TextButton("State" + String(i))));
		int state = (int)theSequencerTree.getChild(i).getProperty("State", dontSendNotification);
		theStateButtons[i]->setButtonText(getTextForEnum(state));
		
		theStateButtons[i]->addListener(this);
		addAndMakeVisible(theDecaySliders.add(new Slider("Decay" + String(i))));
		theDecaySliders[i]->setSliderStyle(Slider::RotaryVerticalDrag);
		theDecaySliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
		theDecaySliders[i]->setTextBoxIsEditable(false);
		theDecaySliders[i]->setDoubleClickReturnValue(true, 0);
		theDecaySliders[i]->setTextValueSuffix(" ms Decay");
		theDecaySliders[i]->setPopupDisplayEnabled(true, theControllerView);
		theDecaySliders[i]->setRange(1, 200, 1);
		theDecaySliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Decay"));
		theDecaySliders[i]->addListener (this);
	}
	addAndMakeVisible(theSequencerLength = new Slider("Length"));
	theSequencerLength->setSliderStyle(Slider::LinearHorizontal);
	theSequencerLength->setRange(1, 16,1);
	theSequencerLength->setTextValueSuffix(" steps");
	theSequencerLength->setValue(theSequencerTree.getProperty("Length"));
	theSequencerLength->addListener(this);
	
	addAndMakeVisible(theRandomAllButton = new TextButton("Random all"));
	theRandomAllButton->addListener(this);
	
	addAndMakeVisible(theResetAllButton = new TextButton("Reset all"));
	theResetAllButton->addListener(this);
	
	addAndMakeVisible(theCopyButton = new TextButton("Copy settings"));
	theCopyButton->addListener(this);
	
	addAndMakeVisible(thePasteButton = new TextButton("Paste settings"));
	thePasteButton->addListener(this);
	
	addAndMakeVisible(theExportButton = new TextButton("Export preset"));
	theExportButton->addListener(this);
	
	addAndMakeVisible(theImportButton = new TextButton("Import preset"));
	theImportButton->addListener(this);
	
	theCurrentBubbleMessage = new BubbleMessageComponent();

	addAndMakeVisible(theShuffleSlider = new Slider("Shuffle"));
	theShuffleSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
	theShuffleSlider->setRange(0, 5, 1);
	theShuffleSlider->setTextValueSuffix(" Shuffle");
	theShuffleSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	theShuffleSlider->setValue(theSequencerTree.getProperty("Shuffle"));
	theShuffleSlider->setPopupDisplayEnabled(true, theControllerView);
	theShuffleSlider->addListener(this);
	
	addAndMakeVisible(theOffsetSlider = new Slider("Offset"));
	theOffsetSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
	theOffsetSlider->setRange(0, 15, 1);
	theOffsetSlider->setTextValueSuffix(" Offset");
	theOffsetSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	theOffsetSlider->setValue(theSequencerTree.getProperty("Offset"));
	theOffsetSlider->setPopupDisplayEnabled(true, theControllerView);
	theOffsetSlider->addListener(this);
	
	addAndMakeVisible(theRangeSlider = new Slider("Range"));
	theRangeSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
	theRangeSlider->setRange(1, 3, 1);
	theRangeSlider->setTextValueSuffix(" Range");
	theRangeSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	theRangeSlider->setPopupDisplayEnabled(true, theControllerView);
	theRangeSlider->setValue(theSequencerTree.getProperty("Range"));
	theRangeSlider->addListener(this);
	
	addAndMakeVisible(&theStepView);
	
	addAndMakeVisible(theOnOffButton = new ToggleButton("On/Off"));
	theOnOffButton->setToggleState(theSequencerTree.getProperty("Status"), dontSendNotification);
	theOnOffButton->addListener(this);

	addAndMakeVisible(theMidiOutputList = new ComboBox("Midi Output list"));
	theMidiOutputList->setTextWhenNothingSelected("Select an midi output");
	theMidiOutputList->setTextWhenNoChoicesAvailable("No midi output available");
	
	addAndMakeVisible(theChannelList = new ComboBox("Channel"));
	for (int i=1;i<=16;i++)
		theChannelList->addItem(String(i), i);
	theChannelList->setSelectedId(theSequencerTree.getProperty("Channel"));
	theChannelList->addListener(this);
	
	refreshMidiList();
	String str = theSequencerTree.getProperty("MidiOutput");
	updateSelectedMidiOut(str);
	addAndMakeVisible(theRootNoteList = new ComboBox("RootNoteList"));
	addAndMakeVisible(theRootOctaveList = new ComboBox("RootOctaveList"));
	addAndMakeVisible(theScaleList = new ComboBox("Scale"));
	loadScales();
	updateNotesAndOctaves();
	theCurrentScale = nullptr;
	theRootNoteList->setSelectedItemIndex(theSequencerTree.getProperty("RootNote"));
	theRootOctaveList->setSelectedItemIndex(theSequencerTree.getProperty("RootOctave"));
	theRootNoteList->addListener(this);
	theRootOctaveList->addListener(this);
	theSequencerTree.addListener(this);
	theMidiOutputList->addListener(this);
	theScaleList->addListener(this);
	setSize(getWidth(), getHeight());
}

SequencerView::~SequencerView()
{
	theCurrentScale = nullptr;
}

void SequencerView::handleAsyncUpdate()
{
	theStepView.update(theStepSliders[thePosition]->getX());
}

void SequencerView::refreshMidiList()
{
	addAndMakeVisible(theMidiOutputList);
	StringArray midiList = MidiOutput::getDevices();
	theMidiOutputList->clear();
	for(int i=0;i<midiList.size();i++)
	{
		if (midiList[i] != "Sequencer")
			theMidiOutputList->addItem(midiList[i], i+1);
	}
}

void SequencerView::resized()
{
	int heigthDiv = getHeight() / 8;

	theMidiOutputList->setBounds(10, 10, 150, heigthDiv);
	theChannelList->setBounds(theMidiOutputList->getRight(), theMidiOutputList->getY(), 50, heigthDiv);
	theSequencerLength->setBounds(theChannelList->getRight(), theMidiOutputList->getY(), 150, heigthDiv);
	theRootNoteList->setBounds(10, theMidiOutputList->getBottom(), 40, heigthDiv);
	theRootOctaveList->setBounds(theRootNoteList->getRight(), theRootNoteList->getY(), theRootNoteList->getWidth(), theRootNoteList->getHeight());
	theScaleList->setBounds(theRootOctaveList->getRight(), theRootOctaveList->getY(), theRootOctaveList->getWidth() * 2, theRootOctaveList->getHeight());
	theRandomAllButton->setBounds(theSequencerLength->getRight(), theMidiOutputList->getY(), 90, heigthDiv);
	theResetAllButton->setBounds(theRandomAllButton->getX(), theRandomAllButton->getBottom(), theRandomAllButton->getWidth(), heigthDiv);
	theShuffleSlider->setBounds(200, theMidiOutputList->getBottom(), 30, heigthDiv);
	theRangeSlider->setBounds(theShuffleSlider->getRight(), theMidiOutputList->getBottom(), 30, heigthDiv);
	theOffsetSlider->setBounds(theRangeSlider->getRight(), theRangeSlider->getY(), 30, heigthDiv);
	theCopyButton->setBounds(theRandomAllButton->getRight(), theMidiOutputList->getY(), 60, heigthDiv);
	thePasteButton->setBounds(theCopyButton->getRight(), theMidiOutputList->getY(), 60, heigthDiv);
	theStepView.setBounds(0, getHeight()-20, getWidth(), heigthDiv);
	theOnOffButton->setBounds(thePasteButton->getRight(), thePasteButton->getY(), 60, heigthDiv);
	theImportButton->setBounds(theOnOffButton->getRight(), theOnOffButton->getY(), 60, heigthDiv);
	theExportButton->setBounds(theImportButton->getRight(), theImportButton->getY(), 60, heigthDiv);
	for(int i=0;i<16;i++)
	{
		theStepSliders[i]->setBounds(theMidiOutputList->getX() + (getWidth()/16)*i, theRootNoteList->getBottom() + 5, heigthDiv * 2, heigthDiv * 2);
		theVelocitySliders[i]->setBounds(theStepSliders[i]->getX(), theStepSliders[i]->getBottom(), heigthDiv, heigthDiv);
		theDecaySliders[i]->setBounds(theVelocitySliders[i]->getRight(), theStepSliders[i]->getBottom(), heigthDiv, heigthDiv);
		theStateButtons[i]->setBounds(theStepSliders[i]->getX(), theVelocitySliders[i]->getBottom(), 50, heigthDiv);
	}
}

int randomise(int min, int max)
{
	return ((int)rand() % (max + abs(min))) - abs(min);
}

void SequencerView::buttonClicked(Button* button)
{
	showPopUp = false;
	if (button == theRandomAllButton)
	{
		for (int i=0;i<16;i++)
		{
			ValueTree child = theSequencerTree.getChild(i);
			int min = 0 - (int)theSequencerTree.getProperty("Range") * 12;
			int max = (int)theSequencerTree.getProperty("Range") * 12;
			int pitch = randomise(min , max);
			while (isOnScale(pitch) == String() && theCurrentScale != nullptr)
		   {
			   pitch = randomise(min, max);
		   }
			child.setProperty("Pitch", pitch, theUndoManager);
			child.setProperty("State", rand() % 2, theUndoManager);
			child.setProperty("Velocity", ((int)rand() % 127), theUndoManager);
			child.setProperty("Decay", ((int)rand() % 200), theUndoManager);
		}
	}
	else if (button == theCopyButton)
	{
		getCopyTree() = theSequencerTree.createCopy();
	}
	else if (button == thePasteButton)
	{
		getCopyTree().removeProperty("MidiOutput", nullptr);
		if (!getCopyTree().isValid()) return;
		theSequencerTree.copyPropertiesFrom(getCopyTree(), theUndoManager);
		for (int i=0; i<16; i++)
		{
			ValueTree sourceChild = getCopyTree().getChild(i);
			ValueTree destinationChild = theSequencerTree.getChild(i);
			destinationChild.copyPropertiesFrom(sourceChild, theUndoManager);
		}
	}
	else if (button == theResetAllButton)
	{
		File presetToLoad = File(thePresetFolder.getFullPathName() + "/default.seq");
		FileInputStream inputStream(presetToLoad);
		ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
		treeToLoad.removeProperty("MidiOutput", nullptr);
		theSequencerTree.copyPropertiesFrom(treeToLoad, theUndoManager);
		for (int i=0; i<16; i++)
		{
			ValueTree sourceChild = treeToLoad.getChild(i);
			ValueTree destinationChild = theSequencerTree.getChild(i);
			destinationChild.copyPropertiesFrom(sourceChild, theUndoManager);
		}
	}
	else if (button == theOnOffButton)
	{
		theSequencerTree.setProperty("Status", theOnOffButton->getToggleState(), nullptr);
	}
	else if (button == theImportButton)
	{
		FileChooser fileChooser ("Load preset file...", thePresetFolder, "*.seq");
		if (fileChooser.browseForFileToOpen())
		{
			showPopUp = false;
			File presetToLoad = fileChooser.getResult();
			FileInputStream inputStream(presetToLoad);
			ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
			treeToLoad.removeProperty("MidiOutput", nullptr);
			theSequencerTree.copyPropertiesFrom(treeToLoad, theUndoManager);
			for (int i=0; i<16; i++)
			{
				ValueTree sourceChild = treeToLoad.getChild(i);
				ValueTree destinationChild = theSequencerTree.getChild(i);
				destinationChild.copyPropertiesFrom(sourceChild, theUndoManager);
			}
		}
	}
	else if (button == theExportButton)
	{
		FileChooser fileChooser ("Save as...", thePresetFolder, "*.seq");
		if (fileChooser.browseForFileToSave(false))
		{
			File preset = File(fileChooser.getResult().getFullPathName());
			FileOutputStream outputStream(preset);
			theSequencerTree.writeToStream(outputStream);
		}
	}
	else
	{
		int index = button->getName().getTrailingIntValue();
		ModifierKeys key =  ModifierKeys::getCurrentModifiersRealtime();
		int currentState = theSequencerTree.getChild(index).getProperty("State");
		if (key.isAltDown())
		{
			currentState = JUMP;
		}
		else
		{
			currentState = !theSequencerTree.getChild(index).getProperty("State");
		}
		
		theSequencerTree.getChild(index).setProperty("State", currentState, theUndoManager);
	}
	startTimer(100);
}

void SequencerView::showBubbleMessage(Component *targetComponent, const String &textToShow)
{
	if (Desktop::canUseSemiTransparentWindows())
	{
		theCurrentBubbleMessage->setAlwaysOnTop (true);
		theCurrentBubbleMessage->addToDesktop (0);
	}
	else
	{
		targetComponent->getTopLevelComponent()->addChildComponent (theCurrentBubbleMessage);
	}
	AttributedString text(textToShow);
	text.setJustification(Justification::centred);
	theCurrentBubbleMessage->showAt(targetComponent, text, 800, true, false);
}

String SequencerView::isOnScale(int value)
{
	String returnedString;
	if (theCurrentScale != nullptr)
	{
		int* notes = theCurrentScale->getNotes().getRawDataPointer();
		for(int j=0;j<theCurrentScale->getNotes().size();j++)
		{
			if (value >= 0)
			{
				if ((int)value % 12 == notes[j])
				{
					returnedString = theCurrentScale->getName();
					break;
				}
			}
			else if(value < 0)
			{
				if ((12 - (abs(value) % 12)) == notes[j])
				{
					returnedString = theCurrentScale->getName();
					break;
				}
			}
		}
	}
	return returnedString;
}

void SequencerView::sliderValueChanged(Slider* slider)
{
	int index = slider->getName().getTrailingIntValue();
	if(slider->getName().contains("Pitch"))
	{
		double value = slider->getValue();
		String bubbleMessage = String(value) + " " + isOnScale(value);
		if (showPopUp)
			showBubbleMessage(slider, bubbleMessage);
		theSequencerTree.getChild(index).setProperty("Pitch", value, theUndoManager);
	}
	else if(slider->getName().contains("Velocity"))
	{
		theSequencerTree.getChild(index).setProperty("Velocity", (int)slider->getValue(), theUndoManager);
	}
	else if(slider->getName().contains("Decay"))
	{
		theSequencerTree.getChild(index).setProperty("Decay", (int)slider->getValue(), theUndoManager);
	}
	else if(slider == theSequencerLength)
	{
		theSequencerTree.setProperty("Length", slider->getValue(), theUndoManager);
	}
	else if(slider == theShuffleSlider)
	{
		theSequencerTree.setProperty("Shuffle", slider->getValue(), theUndoManager);
	}
	else if(slider == theRangeSlider)
	{
		theSequencerTree.setProperty("Range", slider->getValue(), theUndoManager);
	}
	else if(slider == theOffsetSlider)
	{
		theSequencerTree.setProperty("Offset", slider->getValue(), theUndoManager);
	}
}

void SequencerView::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
	if(comboBoxThatHasChanged == theMidiOutputList)
	{
		String midiOutString = theMidiOutputList->getItemText(theMidiOutputList->getSelectedItemIndex());
		theSequencerTree.setProperty("MidiOutput", midiOutString, nullptr);
	}
	else if(comboBoxThatHasChanged == theRootOctaveList)
	{
		int id = comboBoxThatHasChanged->getSelectedItemIndex();
		theSequencerTree.setProperty("RootOctave", id, theUndoManager);
	}
	else if(comboBoxThatHasChanged == theRootNoteList)
	{
		int id = comboBoxThatHasChanged->getSelectedItemIndex();
		theSequencerTree.setProperty("RootNote", id, theUndoManager);
	}
	else if(comboBoxThatHasChanged == theChannelList)
	{
		int id = comboBoxThatHasChanged->getSelectedId();
		theSequencerTree.setProperty("Channel", id, theUndoManager);
	}
	else if(comboBoxThatHasChanged == theScaleList)
	{
		int id = comboBoxThatHasChanged->getSelectedId();
		if (id >= 2)
			theCurrentScale = theScales[id-2];
		else
			theCurrentScale = nullptr;
		theSequencerTree.setProperty("Scale", id, theUndoManager);
	}
}

void SequencerView::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
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
	else if(String(property) == "Range")
	{
		int range = 12 * (int)theSequencerTree.getProperty("Range");
		for(int i=0;i<16;i++)
		{
			theStepSliders[i]->setRange(0 - range, range, 1);
			theStepSliders[i]->repaint();
		}
		theRangeSlider->setValue(tree.getProperty(property));
	}
	else if(String(property) == "Shuffle")
	{
		theShuffleSlider->setValue(tree.getProperty(property));
	}
	else if(String(property) == "Length")
	{
		theSequencerLength->setValue(tree.getProperty(property));
	}
	else if(String(property) == "Channel")
	{
		theChannelList->setSelectedId(tree.getProperty(property));
	}
	else if(String(property) == "Status")
	{
		theOnOffButton->setToggleState(tree.getProperty(property), dontSendNotification);
	}
	else if(String(property) == "Offset")
	{
		theOffsetSlider->setValue(tree.getProperty(property), dontSendNotification);
	}
	else if(String(property) == "RootNote")
	{
		theRootNoteList->setSelectedItemIndex(tree.getProperty(property), dontSendNotification);
	}
	else if(String(property) == "RootOctave")
	{
		theRootOctaveList->setSelectedItemIndex(tree.getProperty(property), dontSendNotification);
	}
	else if(String(property) == "Scale")
	{
		int id = tree.getProperty(property);
		if (id >= 2)
			theCurrentScale = theScales[id-2];
		else
			theCurrentScale = nullptr;
		theScaleList->setSelectedId(id, dontSendNotification);
	}
	else
	{
		for (int i=0; i<16;i++)
		{
			if (tree == (theSequencerTree.getChild(i)))
			{
				if (String(property) == "Pitch")
				{
					theStepSliders[i]->setValue((int)tree.getProperty(property));
				}
				else if (String(property) == "State")
				{
					int state = (int)theSequencerTree.getChild(i).getProperty("State", dontSendNotification);
					theStateButtons[i]->setButtonText(getTextForEnum(state));
				}
				else if (String(property) == "Velocity")
					theVelocitySliders[i]->setValue((int)tree.getProperty(property));
				else if (String(property) == "Decay")
					theDecaySliders[i]->setValue((int)tree.getProperty(property));
			}
		}
	}
}

void SequencerView::updateSelectedMidiOut(String& midiOut)
{
	for (int i=0;i<theMidiOutputList->getNumItems();i++)
	{
		if (theMidiOutputList->getItemText(i) == midiOut)
		{
			theMidiOutputList->setSelectedItemIndex(i);
			break;
		}
	}
}

void SequencerView::updateNotesAndOctaves()
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
		theRootOctaveList->addItem(String(i), i+1);
}

void SequencerView::loadScales()
{
	theScaleList->addItem("No scaling", 1);
	theScaleList->addItem("Major", 2);
	theScaleList->addItem("Minor", 3);
	theScaleList->addItem("Pentatonic Major", 4);
	theScaleList->addItem("Pentatonic Minor", 5);
	theScaleList->setSelectedId(theSequencerTree.getProperty("Scale"));

	for (int i=1;i<theScaleList->getNumItems();i++)
	{
		theScales.add(new Scale(theScaleList->getItemText(i)));
	}
}



