/* =====================================================================
 
 * Steps - Midi sequencer
 * Copyright (C) 2014  Silvere Letellier for Nummer Music
 * Contact: <silvere.letellier@gmail.com>
 
 -----------------------------------------------------------------------
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 ===================================================================== */

#ifndef HEADERVIEW_H_INCLUDED
#define HEADERVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ControllerView.h"
#include "LookAndFeel.h"

extern UndoManager* theUndoManager;
extern Colour textButtonTextColour;

class HeaderView: public Component, ButtonListener, public ComboBoxListener, public ValueTree::Listener, SliderListener
{
public:
	HeaderView(ControllerView* controllerView, ValueTree& preferenceTree): theControllerView(controllerView)
	{
		thePreferenceTree = preferenceTree;
		
		addAndMakeVisible(theAddSequencerButton = new TextButton("Add Sequencer"));
		theAddSequencerButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(COLOUR_4));
		theAddSequencerButton->setColour(TextButton::textColourOffId, textButtonTextColour);
		theAddSequencerButton->addListener(this);
		
		addAndMakeVisible(theUndoButton = new TextButton("Undo"));
		theUndoButton->addListener(this);
		addAndMakeVisible(theRedoButton = new TextButton("Redo"));
		theRedoButton->addListener(this);
		
		addAndMakeVisible(theExportAllButton = new TextButton("Export all"));
		theExportAllButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(COLOUR_4));
		theExportAllButton->setColour(TextButton::textColourOffId, textButtonTextColour);
		theExportAllButton->addListener(this);
		
		addAndMakeVisible(theImportAllButton = new TextButton("Import all"));
		theImportAllButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(COLOUR_4));
		theImportAllButton->setColour(TextButton::textColourOffId, textButtonTextColour);
		theImportAllButton->addListener(this);

		addAndMakeVisible(theClockSourceList = new ComboBox("Clock source"));
		theClockSourceList->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(COLOUR_4));
		theClockSourceList->setColour(ComboBox::textColourId, textButtonTextColour);
		theClockSourceList->addSectionHeading("Clock source");
		theClockSourceList->addItem("External", 1);
		theClockSourceList->addItem("Internal", 2);
		bool clockMode = thePreferenceTree.getProperty("ClockMode", 0);
		theClockSourceList->setSelectedItemIndex(clockMode);
		theClockSourceList->addListener(this);
		
		addAndMakeVisible(theMasterClockList = new ComboBox("MasterClock"));
		theMasterClockList->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(COLOUR_4));
		theMasterClockList->setColour(ComboBox::textColourId, textButtonTextColour);
		theMasterClockList->addSectionHeading("Clock output");
		theMasterClockList->setTextWhenNothingSelected("Clock output");
		theMasterClockList->setTextWhenNoChoicesAvailable("No midi output available");
		StringArray midiList = MidiOutput::getDevices();
		theMasterClockList->addItem("--", 1);
		for(int i=0;i<midiList.size();i++)
		{
			if (midiList[i] != "Steps") theMasterClockList->addItem(midiList[i], i+2);
		}
		theMasterClockList->setSelectedItemIndex(thePreferenceTree.getProperty("MasterClock"));
		theMasterClockList->addListener(this);

		addAndMakeVisible(theBPMSlider = new Slider("BPM"));
		theBPMSlider->setTextBoxStyle(Slider::NoTextBox, false, 60, 50);
		theBPMSlider->setRange(60, 180, 0.1);
		theBPMSlider->setScrollWheelEnabled(false);
		theBPMSlider->setSliderStyle(Slider::RotaryVerticalDrag);
		theBPMSlider->setValue(thePreferenceTree.getProperty("BPM", 120));
		theBPMSlider->setColour(Slider::rotarySliderFillColourId, SeqLookAndFeel::getColour(COLOUR_2));
		theBPMSlider->setColour(Slider::rotarySliderOutlineColourId, SeqLookAndFeel::getColour(COLOUR_4));
		theBPMSlider->addListener(this);
		
		setInterceptsMouseClicks(false, true);
		thePreferenceTree.addListener(this);
	}
	~HeaderView() {}
	
	void buttonClicked(Button* buttonThatWasClicked)
	{
		if (buttonThatWasClicked == theAddSequencerButton)
		{
			ValueTree tree;
			theControllerView->addSequencer(tree);
		}
		else if (buttonThatWasClicked == theImportAllButton)
		{
			theControllerView->importAll();
		}
		else if (buttonThatWasClicked == theExportAllButton)
		{
			theControllerView->exportAll();
		}
		repaint();
	}
	
	void comboBoxChanged(ComboBox* box)
	{
		if (box == theClockSourceList)
		{
			thePreferenceTree.setProperty("ClockMode", box->getSelectedItemIndex(), nullptr);
		}
		else if (box == theMasterClockList)
		{
			thePreferenceTree.setProperty("MasterClock", box->getSelectedItemIndex(), nullptr);
		}
	}
	
	void sliderValueChanged(Slider* slider)
	{
		if (slider == theBPMSlider)
		{
			thePreferenceTree.setProperty("BPM", slider->getValue(), nullptr);
		}
	}

	void paint(Graphics& g)
	{
		g.setColour(textButtonTextColour);
		
		float heigthDiv = getHeight() / 4.0f;
		float widthDiv = getWidth() / 130.0f;
		
		g.setFont (Font ("Helvetica neue",16.0000f, Font::plain));
		g.drawFittedText(String( String(theBPMSlider->getValue()) + String(" bpm")), widthDiv * 109, heigthDiv , widthDiv * 5, heigthDiv * 2, Justification::centred, 1);
		
		g.setColour(Colours::grey);
		g.drawLine(0, getHeight(), getWidth(), getHeight(), 1.0f);
	}
	
	void resized()
	{
		float heigthDiv = getHeight() / 4.0f;
		float widthDiv = getWidth() / 130.0f;
		
		theAddSequencerButton->setBounds(widthDiv * 2, heigthDiv, widthDiv * 10, heigthDiv * 2);
		theImportAllButton->setBounds(widthDiv * 14, heigthDiv, widthDiv * 10, heigthDiv * 2);
		theExportAllButton->setBounds(widthDiv * 26, heigthDiv, widthDiv * 10, heigthDiv * 2);
		
		theMasterClockList->setBounds(widthDiv * 93, heigthDiv, widthDiv * 12, heigthDiv * 2);
		theBPMSlider->setBounds(widthDiv * 106, heigthDiv, heigthDiv * 2, heigthDiv * 2);
		theClockSourceList->setBounds(widthDiv * 116, heigthDiv, widthDiv * 12, heigthDiv * 2);
		repaint();
	}
	
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
	{
		if (String(property) == "BPM")
		{
			theBPMSlider->setValue(tree.getProperty(property));
			repaint();
		}
		else if (String(property) == "MasterClock")
		{
			theMasterClockList->setSelectedItemIndex(tree.getProperty(property));
		}
	}
	
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
	
private:
	ScopedPointer<TextButton> theAddSequencerButton;
	ScopedPointer<TextButton> theUndoButton;
	ScopedPointer<TextButton> theRedoButton;
	ScopedPointer<TextButton> theExportAllButton;
	ScopedPointer<TextButton> theImportAllButton;
	ScopedPointer<Slider> theBPMSlider;
	ScopedPointer<ComboBox> theClockSourceList;
	ScopedPointer<ComboBox> theMasterClockList;
	ControllerView* theControllerView;
	ValueTree thePreferenceTree;
};

#endif  // HEADERVIEW_H_INCLUDED
