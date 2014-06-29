/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "SequencerView.h"
#include "ControllerView.h"
#include "Randomiser.h"
#include "Slider.h"
#include "LookAndFeel.h"

extern File thePresetFolder;

SequencerView::SequencerView(ValueTree& sequencerTree, ControllerView* controllerView): theControllerView(controllerView), thePreviousStepPosition(-1)
{
	theUndoManager = new UndoManager();
	theSequencerTree = sequencerTree;
	thePosition = theSequencerTree.getProperty("Position");
	for(int i=0;i<theSequencerTree.getNumChildren();i++)
	{
		addAndMakeVisible(theStepSliders.add(new SeqSlider("Pitch" + String(i),this)));
		theStepSliders[i]->setSliderStyle(Slider::RotaryVerticalDrag);
		theStepSliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
		theStepSliders[i]->setTextBoxIsEditable(false);
		theStepSliders[i]->setScrollWheelEnabled(false);
		theStepSliders[i]->setColour(Slider::rotarySliderFillColourId, Colours::grey);
		theStepSliders[i]->setDoubleClickReturnValue(false, 0);
		theStepSliders[i]->setRange(0- 12 * (int)theSequencerTree.getProperty("Range"), 12 * (int)theSequencerTree.getProperty("Range"), 1);
		theStepSliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Pitch"));
		theStepSliders[i]->addListener (this);
		addAndMakeVisible(theVelocitySliders.add(new Slider("Velocity" + String(i))));
		theVelocitySliders[i]->setSliderStyle(Slider::LinearHorizontal);
		theVelocitySliders[i]->setTextBoxIsEditable(false);
		theVelocitySliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
		theVelocitySliders[i]->setRange(0, 127, 1);
		theVelocitySliders[i]->setScrollWheelEnabled(false);
		theVelocitySliders[i]->setColour(Slider::rotarySliderFillColourId, Colours::grey);
		theVelocitySliders[i]->setTextValueSuffix(" Velocity");
		theVelocitySliders[i]->setPopupDisplayEnabled(true, theControllerView);
		theVelocitySliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Velocity"));
		theVelocitySliders[i]->setColour(Slider::trackColourId, Colour::fromRGB(83, 85, 75));
		theVelocitySliders[i]->setColour(Slider::thumbColourId, Colour::fromRGB(30, 31, 83));
		theVelocitySliders[i]->addListener(this);
		addAndMakeVisible(theStateButtons.add(new TextButton("State" + String(i))));
		int state = (int)theSequencerTree.getChild(i).getProperty("State", dontSendNotification);
		if (state == ON) theStateButtons[i]->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreenBlue));
		else if (state == OFF) theStateButtons[i]->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourRedOrange));
		else if (state == JUMP) theStateButtons[i]->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourPurple));
		theStateButtons[i]->setButtonText(getTextForEnum(state));
		theStateButtons[i]->addListener(this);
		addAndMakeVisible(theDecaySliders.add(new Slider("Decay" + String(i))));
		theDecaySliders[i]->setSliderStyle(Slider::LinearHorizontal);
		theDecaySliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
		theDecaySliders[i]->setTextBoxIsEditable(false);
		theDecaySliders[i]->setScrollWheelEnabled(false);
		theDecaySliders[i]->setColour(Slider::rotarySliderFillColourId, Colours::grey);
		theDecaySliders[i]->setDoubleClickReturnValue(true, 0);
		theDecaySliders[i]->setTextValueSuffix(" ms Decay");
		theDecaySliders[i]->setPopupDisplayEnabled(true, theControllerView);
		theDecaySliders[i]->setRange(1, 200, 1);
		theDecaySliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Decay"));
		theDecaySliders[i]->setColour(Slider::trackColourId, Colour::fromRGB(83, 85, 75));
		theDecaySliders[i]->setColour(Slider::thumbColourId, Colour::fromRGB(104, 179, 94));
		theDecaySliders[i]->addListener (this);
		
		addAndMakeVisible(theLEDs.add(new StepView()));
	}
	
	int offset = theSequencerTree.getProperty("Offset");
	theStepSliders[offset]->setColour(Slider::rotarySliderFillColourId, Colours::green);
	
	addAndMakeVisible(theSequencerLength = new Slider("Length"));
	theSequencerLength->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	theSequencerLength->setRange(1, 32,1);
	theSequencerLength->setScrollWheelEnabled(false);
	theSequencerLength->setTextBoxStyle(Slider::NoTextBox, false, 30, 30);
	theSequencerLength->setTextValueSuffix(" steps");
	theSequencerLength->setPopupDisplayEnabled(true, theControllerView);
	theSequencerLength->setValue(theSequencerTree.getProperty("Length"));
	theSequencerLength->addListener(this);
	
	addAndMakeVisible(theRandomiser = new Randomiser(this, theSequencerTree));
	
	addAndMakeVisible(theCopyButton = new TextButton("Copy"));
	theCopyButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreen));
	theCopyButton->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(ColourGreenBlue));
	theCopyButton->addListener(this);
	
	addAndMakeVisible(thePasteButton = new TextButton("Paste"));
	thePasteButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreen));
	thePasteButton->addListener(this);
	
	addAndMakeVisible(theExportButton = new TextButton("Export"));
	theExportButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreen));
	theExportButton->addListener(this);
	
	addAndMakeVisible(theImportButton = new TextButton("Import"));
	theImportButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreen));
	theImportButton->addListener(this);
	
	addAndMakeVisible(theDeleteButton = new DeleteButton("Delete"));
	theDeleteButton->addListener(this);
	int sensitivity = 100;
	addAndMakeVisible(theShuffleSlider = new Slider("Shuffle"));
	theShuffleSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
	theShuffleSlider->setRange(0, 5, 1);
	theShuffleSlider->setTextValueSuffix(" Shuffle");
	theShuffleSlider->setMouseDragSensitivity(sensitivity);
	theShuffleSlider->setScrollWheelEnabled(false);
	theShuffleSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	theShuffleSlider->setValue(theSequencerTree.getProperty("Shuffle"));
	theShuffleSlider->setPopupDisplayEnabled(true, theControllerView);
	theShuffleSlider->addListener(this);
	
	addAndMakeVisible(theOffsetSlider = new Slider("Offset"));
	theOffsetSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
	theOffsetSlider->setTextValueSuffix(" Offset");
	theOffsetSlider->setScrollWheelEnabled(false);
	theOffsetSlider->setMouseDragSensitivity(sensitivity);
	theOffsetSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	theOffsetSlider->setValue(theSequencerTree.getProperty("Offset"));
	theOffsetSlider->setPopupDisplayEnabled(true, theControllerView);
	int length = theSequencerTree.getProperty("Length");
	if (length > 16) theOffsetSlider->setRange(0, 31, 1);
	else theOffsetSlider->setRange(0, 15, 1);
	theOffsetSlider->addListener(this);
	
	addAndMakeVisible(theRangeSlider = new Slider("Range"));
	theRangeSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
	theRangeSlider->setRange(1, 3, 1);
	theRangeSlider->setMouseDragSensitivity(sensitivity);
	theRangeSlider->setScrollWheelEnabled(false);
	theRangeSlider->setTextValueSuffix(" Range");
	theRangeSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	theRangeSlider->setPopupDisplayEnabled(true, theControllerView);
	theRangeSlider->setValue(theSequencerTree.getProperty("Range"));
	theRangeSlider->addListener(this);
	
	addAndMakeVisible(theSpeedList = new SeqComboBox("Speed"));
	theSpeedList->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(ColourLightGrey));
	theSpeedList->addSectionHeading("Speed");
	theSpeedList->addItem("1/1", 1);
	theSpeedList->addItem("1/2", 2);
	theSpeedList->addItem("1/4", 3);
	theSpeedList->addItem("1/8", 4);
	float speed = theSequencerTree.getProperty("Speed", 1);
	theSpeedList->setSelectedId(1/speed);
	theSpeedList->addListener(this);
	
	addAndMakeVisible(theOnOffButton = new ToggleButton(""));
	theOnOffButton->setToggleState(theSequencerTree.getProperty("Status"), dontSendNotification);
	theOnOffButton->addListener(this);

	addAndMakeVisible(theMidiOutputList = new SeqComboBox("Midi Output list"));
	theMidiOutputList->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(ColourLightGrey));
	theMidiOutputList->setTextWhenNothingSelected("Select a midi output");
	theMidiOutputList->setTextWhenNoChoicesAvailable("No midi output available");
	
	addAndMakeVisible(theChannelList = new SeqComboBox("Channel"));
	theChannelList->addSectionHeading("Midi channel");
	theChannelList->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(ColourLightGrey));
	for (int i=1;i<=16;i++)
		theChannelList->addItem(String(i), i);
	theChannelList->setSelectedId(theSequencerTree.getProperty("Channel"));
	theChannelList->addListener(this);
	
	refreshMidiList();
	addAndMakeVisible(theRootNoteList = new SeqComboBox("RootNoteList"));
	theRootNoteList->addSectionHeading("Root note");
	theRootNoteList->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(ColourLightGrey));
	addAndMakeVisible(theRootOctaveList = new SeqComboBox("RootOctaveList"));
	theRootOctaveList->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(ColourLightGrey));
	theRootOctaveList->addSectionHeading("Root octave");
	addAndMakeVisible(theScaleList = new SeqComboBox("Scale"));
	theScaleList->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(ColourLightGrey));
	theScaleList->addSectionHeading("Scales");
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
	theUndoManager->clearUndoHistory();
	setInterceptsMouseClicks(false, true);
	addKeyListener(this);
	
	randomiseAll();
}

SequencerView::~SequencerView()
{
	delete theUndoManager;
	theCurrentScale = nullptr;
}

void SequencerView::handleAsyncUpdate()
{

	theLEDs[thePosition]->update(true);
	if (thePreviousStepPosition != -1) theLEDs[thePreviousStepPosition]->update(false);
	thePreviousStepPosition = thePosition;
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
	float heigthDiv = getHeight() / 38.0f;
	float widthDiv = getWidth() / 132.0f;
	float mainSliderDivs = 6.0f;
	
	theRootNoteList->setBounds(widthDiv * 2, heigthDiv, widthDiv * 6, heigthDiv * 2);
	theRootOctaveList->setBounds(widthDiv * 8, heigthDiv, widthDiv * 6, heigthDiv * 2);
	
	theScaleList->setBounds(widthDiv * 16, heigthDiv, widthDiv * 10, heigthDiv * 2);
	theSpeedList->setBounds(widthDiv * 28, heigthDiv, widthDiv * 6, heigthDiv * 2);
	
	theRandomiser->setBounds(widthDiv * 36, heigthDiv, widthDiv * 21, heigthDiv * 5);
	
	theShuffleSlider->setBounds(widthDiv * 59, heigthDiv, widthDiv * 4, heigthDiv * 4);
	theRangeSlider->setBounds(widthDiv * 65, heigthDiv, widthDiv * 4, heigthDiv * 4);
	theOffsetSlider->setBounds(widthDiv * 71, heigthDiv, widthDiv * 4, heigthDiv * 4);
	theSequencerLength->setBounds(widthDiv * 77, heigthDiv, widthDiv * 4, heigthDiv * 4);
	
	theCopyButton->setBounds(widthDiv * 83, heigthDiv, widthDiv * 5, heigthDiv * 2);
	theImportButton->setBounds(widthDiv * 83, heigthDiv * 3, widthDiv * 5, heigthDiv * 2);
	thePasteButton->setBounds(widthDiv * 88, heigthDiv, widthDiv * 5, heigthDiv * 2);
	theExportButton->setBounds(widthDiv * 88, heigthDiv * 3, widthDiv * 5, heigthDiv * 2);
	
	theMidiOutputList->setBounds(widthDiv * 95, heigthDiv, widthDiv * 13, heigthDiv * 2);
	theChannelList->setBounds(widthDiv * 113, heigthDiv, widthDiv * 6, heigthDiv * 2);
	theOnOffButton->setBounds(widthDiv * 121, heigthDiv, widthDiv * 4, heigthDiv * 4);
	theDeleteButton->setBounds(widthDiv * 126, heigthDiv, widthDiv * 4, heigthDiv * 4);

	for(int i=0;i<16;i++)
	{
		theStepSliders[i]->setBounds((widthDiv * 3) + (widthDiv * 8 * i), heigthDiv * 6, widthDiv * mainSliderDivs, heigthDiv * mainSliderDivs);
		theVelocitySliders[i]->setBounds((widthDiv * 3) + (widthDiv * 8 * i), heigthDiv * 14.5, widthDiv * 6, heigthDiv * 1.5);
		theDecaySliders[i]->setBounds((widthDiv * 3) + (widthDiv * 8 * i), heigthDiv * 16.5, widthDiv * 6, heigthDiv * 1.5);
		theStateButtons[i]->setBounds((widthDiv * 3) + (widthDiv * 8 * i), heigthDiv * 18.5, widthDiv * 6, heigthDiv * 2);
		theLEDs[i]->setBounds((widthDiv * 2) + (widthDiv * 8 * i), heigthDiv * 20.5, widthDiv * 8, heigthDiv * 2);
	}
	for(int i=16;i<theSequencerTree.getNumChildren();i++)
	{
		theStepSliders[i]->setBounds((widthDiv * 3) + (widthDiv * 8 * (i-16)), heigthDiv * 23, widthDiv * mainSliderDivs, heigthDiv * mainSliderDivs);
		theVelocitySliders[i]->setBounds((widthDiv * 3) + (widthDiv * 8 * (i-16)), heigthDiv * 29.5, widthDiv * 6, heigthDiv * 1.5);
		theDecaySliders[i]->setBounds((widthDiv * 3) + (widthDiv * 8 * (i-16)), heigthDiv * 31.5, widthDiv * 6, heigthDiv * 1.5);
		theStateButtons[i]->setBounds((widthDiv * 3) + (widthDiv * 8 * (i-16)), heigthDiv * 33.5, widthDiv * 6, heigthDiv * 2);
		theLEDs[i]->setBounds((widthDiv * 2) + (widthDiv * 8 * (i-16)), heigthDiv * 35.5, widthDiv * 8, heigthDiv * 2);
	}
}

int randomise(int min, int max)
{
	return ((int)rand() % (max + abs(min))) - abs(min);
}

void SequencerView::trigMidiNote(int sliderIndex)
{
	theSequencerTree.setProperty("Trigger", sliderIndex, nullptr);
}

void SequencerView::randomiseAll()
{
	for (int i=0;i<theSequencerTree.getNumChildren();i++)
	{
		ValueTree child = theSequencerTree.getChild(i);
		if (theSequencerTree.getProperty("RandPitch"))
		{
			int min = 0 - (int)theSequencerTree.getProperty("Range") * 12;
			int max = (int)theSequencerTree.getProperty("Range") * 12;
			int pitch = randomise(min , max);
			while (isOnScale(pitch) == String() && theCurrentScale != nullptr)
			{
				pitch = randomise(min, max);
			}
			child.setProperty("Pitch", pitch, theUndoManager);
		}
		if (theSequencerTree.getProperty("RandState")) child.setProperty("State", rand() % 2, theUndoManager);
		if (theSequencerTree.getProperty("RandVelocity")) child.setProperty("Velocity", ((int)rand() % 127), theUndoManager);
		if (theSequencerTree.getProperty("RandDecay")) child.setProperty("Decay", ((int)rand() % 200), theUndoManager);
	}
}

void SequencerView::buttonClicked(Button* button)
{
	if (button == theCopyButton)
	{
		getCopyTree() = theSequencerTree.createCopy();
	}
	else if (button == thePasteButton)
	{
		if (!getCopyTree().isValid()) return;
		theSequencerTree.copyPropertiesFrom(getCopyTree(), theUndoManager);
		for (int i=0; i<theSequencerTree.getNumChildren(); i++)
		{
			ValueTree sourceChild = getCopyTree().getChild(i);
			ValueTree destinationChild = theSequencerTree.getChild(i);
			destinationChild.copyPropertiesFrom(sourceChild, theUndoManager);
		}
	}
	else if (button == theOnOffButton)
	{
		theSequencerTree.setProperty("Status", theOnOffButton->getToggleState(), nullptr);
	}
	else if (button == theDeleteButton)
	{
		int i = theSequencerTree.getParent().indexOf(theSequencerTree);
		theControllerView->removeSequencer(i);
	}
	
	else if (button == theImportButton)
	{
		FileChooser fileChooser ("Load preset file...", thePresetFolder, "*.seq");
		if (fileChooser.browseForFileToOpen())
		{
			File presetToLoad = fileChooser.getResult();
			FileInputStream inputStream(presetToLoad);
			ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
			theSequencerTree.copyPropertiesFrom(treeToLoad, theUndoManager);
			for (int i=0; i<theSequencerTree.getNumChildren(); i++)
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
			if (preset.exists()) preset.deleteFile();
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
		SeqSlider* sld = (SeqSlider*)slider;
		double value = slider->getValue();
		String bubbleMessage = String(value) + " " + isOnScale(value);
		sld->setMessage(bubbleMessage);
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
		int length = slider->getValue();
		if (length == 1)
		{
			theOffsetSlider->setValue(0);
			theOffsetSlider->setRange(0, 1, 1);
			return;
		}
		if (theOffsetSlider->getValue() > length - 1)
			theOffsetSlider->setValue(length-1);
		theOffsetSlider->setRange(0, length-1, 1);
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
		int index = theMidiOutputList->getSelectedItemIndex();
		if (index != -1)
		{
			String midiOutString = theMidiOutputList->getItemText(index);
			theSequencerTree.setProperty("MidiOutput", midiOutString, nullptr);
		}
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
	else if(comboBoxThatHasChanged == theSpeedList)
	{
		int id = comboBoxThatHasChanged->getSelectedId(); float speed;
		if (id == 1) speed = 1;
		else if (id == 2) speed = 0.5;
		else if (id == 3) speed = 0.25;
		else if (id == 4) speed = 0.125;
		theSequencerTree.setProperty("Speed", speed, theUndoManager);
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
		for(int i=0;i<theSequencerTree.getNumChildren();i++)
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
		int length = tree.getProperty(property);
		theSequencerLength->setValue(length);
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
		for(int i=0;i<32;i++)
		{
			theStepSliders[i]->setColour(Slider::rotarySliderFillColourId, Colours::grey);
		}
		int offset = tree.getProperty(property);
		theStepSliders[offset]->setColour(Slider::rotarySliderFillColourId, Colours::green);
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
	else if(String(property) == "Speed")
	{
		float speed = tree.getProperty(property); int index;
		if (speed == 1) index = 1;
		else if (speed == 0.5) index = 2;
		else if (speed == 0.25) index = 3;
		else if (speed == 0.125) index = 4;
		theSpeedList->setSelectedId(index, dontSendNotification);
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
		for (int i=0; i<theSequencerTree.getNumChildren();i++)
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
					if (state == ON) theStateButtons[i]->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreenBlue));
					else if (state == OFF) theStateButtons[i]->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourRedOrange));
					else if (state == JUMP) theStateButtons[i]->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourPurple));
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

const char* SequencerView::getTextForEnum(int enumVal)
{
	static const char * stateStrings[] = { "OFF", "ON", "JUMP" };
	return stateStrings[enumVal];
}
