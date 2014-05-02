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
	int sequencer16Height = theMainScreen.getHeight() / 4.6;
	int sequencer32Height = theMainScreen.getHeight() / 2.6;
	int totalHeigth = 0;
	int sequencerWidth = theMainScreen.getWidth() / 1.5;
	theHeaderView->setBounds(0, 0, sequencerWidth, theMainScreen.getHeight() / 24);
	totalHeigth += theHeaderView->getHeight();
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		if ((int)theMasterTree.getChild(i).getProperty("Length") > 16)
		{
			theSequencerViews[i]->setBounds(0, totalHeigth, sequencerWidth, sequencer32Height);
		}
		else if ((int)theMasterTree.getChild(i).getProperty("Length") < 17)
		{
			theSequencerViews[i]->setBounds(0, totalHeigth, sequencerWidth, sequencer16Height);
		}
		totalHeigth += theSequencerViews[i]->getHeight();
	}
	if (theMasterTree.getNumChildren() != 0)
		setSize(sequencerWidth, totalHeigth);
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
