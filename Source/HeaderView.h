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
	}
	~HeaderView()
	{
		theControllerView = nullptr;
	}
	
	void buttonClicked(Button* buttonThatWasClicked)
	{
		if (buttonThatWasClicked == theAddSequencerButton)
		{
			theControllerView->addSequencer();
		}
		else if (buttonThatWasClicked == theRemoveSequencerButton)
		{
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
	}

	void paint(Graphics& g)
	{
		g.setColour(Colours::black);
		g.drawRect(getBounds());
		g.setFont(22);
		g.drawText(theMainLabel, getWidth()/2.2, getHeight()/4, getWidth()/6, getHeight()/10, Justification::left, true);
		g.setFont(14);
		g.drawText(String("Num of Sequencers: " + String(theControllerView->getNumOfSequencer())), getWidth()/1.5, getHeight()/4, getWidth()/6, getHeight()/10, Justification::left, true);

	}
	
	void resized()
	{
		theAddSequencerButton->setBounds(10, getHeight()/4, getWidth()/10, getHeight()/2);
		theRemoveSequencerButton->setBounds(theAddSequencerButton->getRight(), getHeight()/4, getWidth()/10, getHeight()/2);
		theUndoButton->setBounds(theRemoveSequencerButton->getRight(), getHeight()/4, getWidth()/10, getHeight()/2);
		theRedoButton->setBounds(theUndoButton->getRight(), getHeight()/4, getWidth()/10, getHeight()/2);
	}
	
private:
	ScopedPointer<TextButton> theAddSequencerButton;
	ScopedPointer<TextButton> theRemoveSequencerButton;
	ScopedPointer<TextButton> theUndoButton;
	ScopedPointer<TextButton> theRedoButton;
	ControllerView* theControllerView;
	String theMainLabel;
};



#endif  // HEADERVIEW_H_INCLUDED
