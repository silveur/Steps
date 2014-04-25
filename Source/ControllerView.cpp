/*
  ==============================================================================

	RootView.cpp
	Created: 23 Apr 2014 12:15:42pm
	Author:  Silvere Letellier

  ==============================================================================
*/

#include "ControllerView.h"
#include "HeaderView.h"

ControllerView::ControllerView(ValueTree& masterTree): theMasterTree(masterTree)
{
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		ValueTree sequenceTree = theMasterTree.getChild(i);
		theSequencerViews.add(new SequencerView(sequenceTree, this));
		addAndMakeVisible(theSequencerViews[i]);
	}
	theMainScreen = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
	addAndMakeVisible(theHeaderView = new HeaderView(this));
	updatePositions();
	theMasterTree.addListener(this);
}

ControllerView::~ControllerView()
{
	delete theHeaderView;
}

void ControllerView::updatePositions()
{
	int sequencerHeight = theMainScreen.getHeight() / 7;
	setSize(theMainScreen.getWidth()/2, theHeaderView->getHeight() + sequencerHeight * theSequencerViews.size());
	theHeaderView->setBounds(0, 0, getWidth(), sequencerHeight / 4);
	for (int i=0; i<theSequencerViews.size(); i++)
	{
		theSequencerViews[i]->setBounds(0, theHeaderView->getHeight() + (i * sequencerHeight), getWidth(), sequencerHeight);
	}
}

void ControllerView::updatePresetList()
{
	for (int i=0; i<theSequencerViews.size(); i++)
	{
		theSequencerViews[i]->updatePresetList();
	}
}

void ControllerView::resized()
{
	updatePositions();
}

const int ControllerView::getNumOfSequencer() const
{
	return theMasterTree.getNumChildren();
}

void ControllerView::addSequencer()
{
	ValueTree sequencerTree("Sequencer" + String(theMasterTree.getNumChildren()));
	theMasterTree.addChild(sequencerTree, -1, nullptr);
	theSequencerViews.add(new SequencerView(sequencerTree, this));
	addAndMakeVisible(theSequencerViews.getLast());
	updatePositions();
}

void ControllerView::removeSequencer()
{
	theMasterTree.removeChild(theMasterTree.getNumChildren()-1, nullptr);
	theSequencerViews.removeLast();
	updatePositions();
}
