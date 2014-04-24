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
	theMasterTree = theMaster->getMasterTree();
	OwnedArray<Sequencer>& seqArray = theMaster->getSequencerArray();
	for (int i=0; i<seqArray.size(); i++)
	{
		ValueTree sequenceTree = seqArray[i]->getSequencerTree();
		theSequencerViews.add(new SequencerView(sequenceTree, this));
		addAndMakeVisible(theSequencerViews[i]);
	}
	theMainScreen = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
	addAndMakeVisible(theHeaderView = new HeaderView(this));
	updatePositions();
	theMasterTree.addListener(this);
}

RootView::~RootView()
{
	delete theHeaderView;
	theMaster = nullptr;
}

void RootView::updatePositions()
{
	int sequencerHeight = theMainScreen.getHeight() / 7;
	
	setSize(theMainScreen.getWidth()/2, theHeaderView->getHeight() + sequencerHeight * theSequencerViews.size());
	theHeaderView->setBounds(0, 0, getWidth(), sequencerHeight / 4);
	for (int i=0; i<theSequencerViews.size(); i++)
	{
		theSequencerViews[i]->setBounds(0, theHeaderView->getHeight() + (i * sequencerHeight), getWidth(), sequencerHeight);
	}
}

void RootView::updatePresetList()
{
	for (int i=0; i<theSequencerViews.size(); i++)
	{
		theSequencerViews[i]->updatePresetList();
	}
}

void RootView::resized()
{
	updatePositions();
}

int RootView::getNumOfSequencer()
{
	return theMasterTree.getNumChildren();
}

void RootView::addSequencer()
{
	theMaster->addSequencer();
}

void RootView::removeSequencer()
{
	theMaster->deleteSequencer();
}

void RootView::valueTreeChildAdded (ValueTree& parentTree, ValueTree& child)
{
	if (parentTree == theMasterTree)
	{
		theSequencerViews.add(new SequencerView(child, this));
		addAndMakeVisible(theSequencerViews.getLast());
		updatePositions();
	}
}

void RootView::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& child)
{
	if (parentTree == theMasterTree)
	{
		theSequencerViews.removeLast();
		updatePositions();
	}
}
