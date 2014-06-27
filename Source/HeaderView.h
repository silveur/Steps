/*
  ==============================================================================

    HeaderView.h
    Created: 23 Apr 2014 3:41:16pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef HEADERVIEW_H_INCLUDED
#define HEADERVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ControllerView.h"
#include "LookAndFeel.h"
#include "ComboBox.h"

extern UndoManager* theUndoManager;

class HeaderView: public Component, ButtonListener, public ComboBoxListener, public ValueTree::Listener, SliderListener
{
public:
	HeaderView(ControllerView* controllerView, ValueTree& preferenceTree): theControllerView(controllerView)
	{
		theMainLabel = "Sequencer";
		thePreferenceTree = preferenceTree;
		addAndMakeVisible(theAddSequencerButton = new TextButton("Add Sequencer"));
		theAddSequencerButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreen));
		theAddSequencerButton->addListener(this);
		addAndMakeVisible(theUndoButton = new TextButton("Undo"));
		theUndoButton->addListener(this);
		addAndMakeVisible(theRedoButton = new TextButton("Redo"));
		theRedoButton->addListener(this);
		addAndMakeVisible(theExportAllButton = new TextButton("Export all"));
		theExportAllButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreen));
		theExportAllButton->addListener(this);
		addAndMakeVisible(theImportAllButton = new TextButton("Import all"));
		theImportAllButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreen));
		theImportAllButton->addListener(this);
		addAndMakeVisible(theKickBackButton = new TextButton("Rewind"));
		theKickBackButton->addListener(this);
		addAndMakeVisible(theClockSourceList = new SeqComboBox("Clock source"));
		theClockSourceList->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(ColourLightGrey));
		theClockSourceList->addSectionHeading("Clock source");
		theClockSourceList->addItem("External", 1);
		theClockSourceList->addItem("Internal", 2);
		bool clockMode = thePreferenceTree.getProperty("ClockMode", 0);
		theClockSourceList->setSelectedItemIndex(clockMode);
		theClockSourceList->addListener(this);
		
		addAndMakeVisible(theMasterClockList = new SeqComboBox("MasterClock"));
		theMasterClockList->setColour(ComboBox::backgroundColourId, SeqLookAndFeel::getColour(ColourLightGrey));
		theMasterClockList->addSectionHeading("Clock output");
		theMasterClockList->setTextWhenNothingSelected("Clock output");
		theMasterClockList->setTextWhenNoChoicesAvailable("No midi output available");
		StringArray midiList = MidiOutput::getDevices();
		theMasterClockList->addItem("--", 1);
		for(int i=0;i<midiList.size();i++)
		{
			if (midiList[i] != "Sequencer") theMasterClockList->addItem(midiList[i], i+2);
		}
		theMasterClockList->setSelectedItemIndex(thePreferenceTree.getProperty("MasterClock"));
		theMasterClockList->addListener(this);

		addAndMakeVisible(theBPMSlider = new Slider("BPM"));
		theBPMSlider->setTextBoxStyle(Slider::NoTextBox, false, 60, 50);
		theBPMSlider->setRange(60, 180, 0.1);
		theBPMSlider->setScrollWheelEnabled(false);
		theBPMSlider->setSliderStyle(Slider::RotaryVerticalDrag);
		theBPMSlider->setValue(thePreferenceTree.getProperty("BPM", 120));
		theBPMSlider->addListener(this);
		theBPMSlider->setVisible(clockMode);
		
		setInterceptsMouseClicks(false, true);
		thePreferenceTree.addListener(this);
	}
	~HeaderView()
	{
		theControllerView = nullptr;
	}
	
	void buttonClicked(Button* buttonThatWasClicked)
	{
		if (buttonThatWasClicked == theAddSequencerButton)
		{
			ValueTree tree;
			theControllerView->addSequencer(tree);
		}
		else if (buttonThatWasClicked == theKickBackButton)
		{
			theControllerView->kickBack();
		}
		else if (buttonThatWasClicked == theImportAllButton)
		{
			importAll();
		}
		else if (buttonThatWasClicked == theExportAllButton)
		{
			exportAll();
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
	
	void exportAll()
	{
		FileChooser fileChooser ("Save as...",
								 thePresetFolder,
								 "*.masterseq");
		if (fileChooser.browseForFileToSave(false))
		{
			File preset = File(fileChooser.getResult().getFullPathName());
			FileOutputStream outputStream(preset);
			ValueTree masterTree = theControllerView->getMasterTree();
			masterTree.writeToStream(outputStream);
		}
	}
	
	void importAll()
	{
		FileChooser fileChooser ("Load preset file...",
								 thePresetFolder,
								 "*.masterseq");
		if (fileChooser.browseForFileToOpen())
		{
			ValueTree masterTree = theControllerView->getMasterTree();
			while (masterTree.getNumChildren())
			{
				theControllerView->removeSequencer(-1);
			}
			File presetToLoad = fileChooser.getResult();
			FileInputStream inputStream(presetToLoad);
			ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
			for (int i=0;i<treeToLoad.getNumChildren();i++)
			{
				ValueTree treeToAdd = treeToLoad.getChild(i);
				theControllerView->addSequencer(treeToAdd);
			}
		}
	}

	void paint(Graphics& g)
	{
		g.setColour(Colour::fromRGB(198, 201, 180));
		g.fillAll();
		
		g.setColour(Colours::black);
		
		float heigthDiv = getHeight() / 8.0f;
		float widthDiv = getWidth() / 132.0f;
		
		g.setFont(11);
		g.drawFittedText("Clock output", widthDiv * 88, heigthDiv * 5, widthDiv * 17, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText(String(theBPMSlider->getValue()), widthDiv * 107, heigthDiv * 5, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Clock source", widthDiv * 113, heigthDiv * 5, widthDiv * 17, heigthDiv * 2, Justification::centred, 1);
		
		g.setColour(Colours::grey);
		g.drawLine(0, getHeight(), getWidth(), getHeight(), 1.0f);
	}
	
	void resized()
	{
		float heigthDiv = getHeight() / 8.0f;
		float widthDiv = getWidth() / 132.0f;
		
		theAddSequencerButton->setBounds(widthDiv * 2, heigthDiv, widthDiv * 12, heigthDiv * 4);
		theImportAllButton->setBounds(widthDiv * 16, heigthDiv, widthDiv * 10, heigthDiv * 4);
		theExportAllButton->setBounds(widthDiv * 28, heigthDiv, widthDiv * 10, heigthDiv * 4);
		
		theMasterClockList->setBounds(widthDiv * 88, heigthDiv, widthDiv * 17, heigthDiv * 4);
		theBPMSlider->setBounds(widthDiv * 107, heigthDiv, widthDiv * 4, heigthDiv * 4);
		theClockSourceList->setBounds(widthDiv * 113, heigthDiv, widthDiv * 17, heigthDiv * 4);
		repaint();
	}
	
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
	{
		if (String(property) == "BPM")
		{
			theBPMSlider->setValue(tree.getProperty(property));
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
	ScopedPointer<TextButton> theKickBackButton;
	ScopedPointer<Slider> theBPMSlider;
	ScopedPointer<SeqComboBox> theClockSourceList;
	ScopedPointer<SeqComboBox> theMasterClockList;
	ControllerView* theControllerView;
	ValueTree thePreferenceTree;
	String theMainLabel;
};



#endif  // HEADERVIEW_H_INCLUDED
