/*
  ==============================================================================

    RootView.cpp
    Created: 23 Apr 2014 12:15:42pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#include "RootView.h"
#include "HeaderView.h"

RootView::RootView(Master* master): theMaster(master)
{
	OwnedArray<Sequencer>& seqArray = theMaster->getSequencerArray();
	for (int i=0; i<seqArray.size(); i++)
	{
		theSequencerViews.add(new SequencerView(seqArray[i]));
		addAndMakeVisible(theSequencerViews[i]);
	}
	theMainScreen = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
	addAndMakeVisible(theHeaderView = new HeaderView(this));
	updatePositions();
}

RootView::~RootView()
{
	
}

void RootView::paint(Graphics& g)
{
	
}

void RootView::updatePositions()
{
	setSize(theMainScreen.getWidth()/2, (theMainScreen.getHeight()/32) + (theMainScreen.getHeight()/6) * theSequencerViews.size());
	for (int i=0; i<theSequencerViews.size(); i++)
	{
		theSequencerViews[i]->setBounds(0, (theHeaderView->getHeight()) + (i * (getHeight()/theSequencerViews.size())), getWidth(), (theMainScreen.getHeight()/6 ) * theSequencerViews.size());
	}
}

void RootView::resized()
{
	theHeaderView->setBounds(0, 0, getWidth(), theMainScreen.getHeight()/32);
	updatePositions();
}

void RootView::addSequencer()
{
	theSequencerViews.add(new SequencerView(theMaster->addSequencer()));
	addAndMakeVisible(theSequencerViews.getLast());
	updatePositions();
}

void RootView::removeSequencer()
{
	OwnedArray<Sequencer>& seqArray = theMaster->getSequencerArray();
	seqArray.removeLast();
	theSequencerViews.removeLast();
	updatePositions();
}
