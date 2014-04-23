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
#include "RootView.h"

class HeaderView: public Component, ButtonListener
{
public:
	HeaderView(RootView* rootView): theRootView(rootView)
	{
		theMainLabel = "Sequencer";
		addAndMakeVisible(theAddSequencerButton = new TextButton("Add Sequencer"));
		theAddSequencerButton->addListener(this);
		addAndMakeVisible(theRemoveSequencerButton = new TextButton("Remove Sequencer"));
		theRemoveSequencerButton->addListener(this);
	}
	~HeaderView()
	{
		
	}
	
	void buttonClicked(Button* buttonThatWasClicked)
	{
		if (buttonThatWasClicked == theAddSequencerButton)
		{
			theRootView->addSequencer();
		}
		else if (buttonThatWasClicked == theRemoveSequencerButton)
		{
			theRootView->removeSequencer();
		}
	}
	
	void paint(Graphics& g)
	{
		g.setColour(Colours::black);
		g.drawRect(getBounds());
		g.setFont(22);
		g.drawText(theMainLabel, getWidth()/2.2, getHeight()/4, getWidth()/6, getHeight()/10, Justification::left, true);
		g.setFont(14);
		g.drawText(String("Num of Sequencers: " + String(theRootView->theSequencerViews.size())), getWidth()/1.5, getHeight()/4, getWidth()/6, getHeight()/10, Justification::left, true);

	}
	
	void resized()
	{
		theAddSequencerButton->setBounds(getWidth()/8, getHeight()/4, getWidth()/10, getHeight()/2);
		theRemoveSequencerButton->setBounds(theAddSequencerButton->getRight(), getHeight()/4, getWidth()/10, getHeight()/2);
	}
	
private:
	ScopedPointer<TextButton> theAddSequencerButton;
	ScopedPointer<TextButton> theRemoveSequencerButton;
	String theMainLabel;
	RootView* theRootView;
};



#endif  // HEADERVIEW_H_INCLUDED
