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
	LookAndFeel::setDefaultLookAndFeel(theLookAndFeel = new SeqLookAndFeel());
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
	int sequencerHeight = theMainScreen.getHeight() / 5;
	theHeaderView->setBounds(0, 0, getWidth(), sequencerHeight / 4);
	if (theSequencerViews.size() == 0)
	{
//		setSize(theMainScreen.getWidth()/1.1, 100);
	}
	else
		setSize(theMainScreen.getWidth()/1.5, theHeaderView->getHeight() + sequencerHeight * theSequencerViews.size());
	
	for (int i=0; i<theSequencerViews.size(); i++)
	{
		theSequencerViews[i]->setBounds(0, theHeaderView->getHeight() + (i * sequencerHeight), getWidth(), sequencerHeight);
	}
}

void ControllerView::resized()
{
	updatePositions();
}

void ControllerView::kickBack()
{
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		theMasterTree.getChild(i).setProperty("KickBack", 1, nullptr);
	}
}

const int ControllerView::getNumOfSequencer() const
{
	return theMasterTree.getNumChildren();
}

void ControllerView::addSequencer(ValueTree& sequencerTreeToAdd)
{
	if(sequencerTreeToAdd.isValid())
	{
		ValueTree copiedTree = sequencerTreeToAdd.createCopy();
		theMasterTree.addChild(copiedTree, -1, nullptr);
		theSequencerViews.add(new SequencerView(copiedTree, this));
		addAndMakeVisible(theSequencerViews.getLast());
		updatePositions();
	}
	else
	{
		ValueTree sequencerTree("Sequencer" + String(theMasterTree.getNumChildren()));
		theMasterTree.addChild(sequencerTree, -1, nullptr);
		int index = theMasterTree.indexOf(sequencerTree);
		if (index > 0) theSequencerViews[index-1]->updateChainBox();
		theSequencerViews.add(new SequencerView(sequencerTree, this));
		addAndMakeVisible(theSequencerViews.getLast());
		updatePositions();
	}
}

void ControllerView::removeSequencer()
{
	int index = theMasterTree.getNumChildren()-1;
	theMasterTree.removeChild(index, nullptr);
	theSequencerViews.remove(index);
	updatePositions();
}
