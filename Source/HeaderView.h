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

extern UndoManager* theUndoManager;

class HeaderView: public Component, ButtonListener, public ComboBoxListener, public ValueTree::Listener, SliderListener
{
public:
	HeaderView(ControllerView* controllerView, ValueTree& preferenceTree): theControllerView(controllerView)
	{
		theMainLabel = "Sequencer";
		thePreferenceTree = preferenceTree;
		addAndMakeVisible(theAddSequencerButton = new TextButton("Add Sequencer"));
		theAddSequencerButton->addListener(this);
		addAndMakeVisible(theUndoButton = new TextButton("Undo"));
		theUndoButton->addListener(this);
		addAndMakeVisible(theRedoButton = new TextButton("Redo"));
		theRedoButton->addListener(this);
		addAndMakeVisible(theExportAllButton = new TextButton("Export all"));
		theExportAllButton->addListener(this);
		addAndMakeVisible(theImportAllButton = new TextButton("Import all"));
		theImportAllButton->addListener(this);
		addAndMakeVisible(theKickBackButton = new TextButton("Rewind"));
		theKickBackButton->addListener(this);
		addAndMakeVisible(theClockSourceList = new ComboBox("Clock source"));
		theClockSourceList->addItem("External Clock", 1);
		theClockSourceList->addItem("Internal Clock", 2);
		bool clockMode = thePreferenceTree.getProperty("ClockMode", 0);
		theClockSourceList->setSelectedItemIndex(clockMode);
		theClockSourceList->addListener(this);
		
		addAndMakeVisible(theBPMSlider = new Slider("BPM"));
		theBPMSlider->setTextBoxStyle(Slider::TextBoxLeft, false, 60, 50);
		theBPMSlider->setRange(30, 180, 0.1);
		theBPMSlider->setScrollWheelEnabled(false);
		theBPMSlider->setSliderStyle(Slider::RotaryVerticalDrag);
		theBPMSlider->setValue(thePreferenceTree.getProperty("BPM"));
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
			treeToLoad.setProperty("MidiOutput", String(), nullptr);
			for (int i=0;i<treeToLoad.getNumChildren();i++)
			{
				ValueTree treeToAdd = treeToLoad.getChild(i);
				treeToAdd.setProperty("MidiOutput", String(), nullptr);
				theControllerView->addSequencer(treeToAdd);
			}
		}
	}

	void paint(Graphics& g) {}
	
	void resized()
	{
		theAddSequencerButton->setBounds(10, getHeight()/4, getWidth()/12, getHeight()/2);
		theExportAllButton->setBounds(theAddSequencerButton->getRight(), theAddSequencerButton->getY(), getWidth()/16, getHeight()/2);
		theImportAllButton->setBounds(theExportAllButton->getRight(), theExportAllButton->getY(), getWidth()/16, getHeight()/2);
		theKickBackButton->setBounds(theImportAllButton->getRight(), theImportAllButton->getY(), getWidth()/20, getHeight()/2);
		
		theBPMSlider->setBounds(getWidth()/1.3, 0, getHeight()*3, getHeight());
		theClockSourceList->setBounds(theBPMSlider->getRight(), 0, 110, getHeight());
	}
	
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
	{
		if (String(property) == "ClockMode")
		{
			ClockMode theClockMode = (ClockMode)(bool)tree.getProperty(property);
			if (theClockMode == INTERNAL)
			{
				theBPMSlider->setVisible(true);
			}
			else if (theClockMode == EXTERNAL)
			{
				theBPMSlider->setVisible(false);
			}
		}
		else if (String(property) == "BPM")
		{
			theBPMSlider->setValue(tree.getProperty(property));
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
	ScopedPointer<ComboBox> theClockSourceList;
	ControllerView* theControllerView;
	ValueTree thePreferenceTree;
	String theMainLabel;
};



#endif  // HEADERVIEW_H_INCLUDED
