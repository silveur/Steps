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

class HeaderView: public Component, ButtonListener
{
public:
	HeaderView(ControllerView* controllerView): theControllerView(controllerView)
	{
		theMainLabel = "Sequencer";
		addAndMakeVisible(theAddSequencerButton = new TextButton("Add Sequencer"));
		theAddSequencerButton->addListener(this);
		addAndMakeVisible(theRemoveSequencerButton = new TextButton("Remove Sequencer"));
		theRemoveSequencerButton->addListener(this);
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
		else if (buttonThatWasClicked == theRemoveSequencerButton)
		{
			if (theControllerView->getNumOfSequencer() > 1)
				theControllerView->removeSequencer();
		}
		else if (buttonThatWasClicked == theUndoButton)
		{
			theUndoManager->undo();
		}
		else if (buttonThatWasClicked == theRedoButton)
		{
			theUndoManager->redo();
		}
		else if (buttonThatWasClicked == theKickBackButton)
		{
			theControllerView->kickBack();
		}
		else if (buttonThatWasClicked == theImportAllButton)
		{
			FileChooser fileChooser ("Load preset file...",
									 thePresetFolder,
									 "*.masterseq");
			if (fileChooser.browseForFileToOpen())
			{
				ValueTree masterTree = theControllerView->getMasterTree();
				while (masterTree.getNumChildren())
				{
					theControllerView->removeSequencer();
				}
				File presetToLoad = fileChooser.getResult();
				FileInputStream inputStream(presetToLoad);
				ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
				treeToLoad.removeProperty("MidiOutput", nullptr);
				for (int i=0;i<treeToLoad.getNumChildren();i++)
				{
					ValueTree treeToAdd = treeToLoad.getChild(i);
					theControllerView->addSequencer(treeToAdd);
				}
			}

		}
		else if (buttonThatWasClicked == theExportAllButton)
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
		repaint();
	}

	void paint(Graphics& g)
	{
		g.setColour(Colours::black);
		g.drawRect(getBounds());
	}
	
	void resized()
	{
		theAddSequencerButton->setBounds(10, getHeight()/4, getWidth()/12, getHeight()/2);
		theRemoveSequencerButton->setBounds(theAddSequencerButton->getRight(), getHeight()/4, getWidth()/12, getHeight()/2);
		theUndoButton->setBounds(theRemoveSequencerButton->getRight(), getHeight()/4, getWidth()/20, getHeight()/2);
		theRedoButton->setBounds(theUndoButton->getRight(), getHeight()/4, getWidth()/20, getHeight()/2);
		theExportAllButton->setBounds(theRedoButton->getRight(), theRedoButton->getY(), getWidth()/16, getHeight()/2);
		theImportAllButton->setBounds(theExportAllButton->getRight(), theExportAllButton->getY(), getWidth()/16, getHeight()/2);
		theKickBackButton->setBounds(theImportAllButton->getRight(), theImportAllButton->getY(), getWidth()/20, getHeight()/2);
	}
	
private:
	ScopedPointer<TextButton> theAddSequencerButton;
	ScopedPointer<TextButton> theRemoveSequencerButton;
	ScopedPointer<TextButton> theUndoButton;
	ScopedPointer<TextButton> theRedoButton;
	ScopedPointer<TextButton> theExportAllButton;
	ScopedPointer<TextButton> theImportAllButton;
	ScopedPointer<TextButton> theKickBackButton;
	ControllerView* theControllerView;
	String theMainLabel;
};



#endif  // HEADERVIEW_H_INCLUDED
