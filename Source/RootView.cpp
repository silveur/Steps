/*
  ==============================================================================

    RootView.cpp
    Created: 23 Apr 2014 12:15:42pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#include "RootView.h"

RootView::RootView(OwnedArray<Sequencer>& sequencerArray)
{
	for (int i=0; i<sequencerArray.size(); i++)
	{
		theSequencerViews.add(new SequencerView(sequencerArray[i]));
		addAndMakeVisible(theSequencerViews[i]);
	}
	theMainScreen = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
	setSize(theMainScreen.getWidth()/2, (sequencerArray.size() + 1) * 120);
}

RootView::~RootView()
{
	
}

void RootView::resized()
{
	for (int i=0; i<theSequencerViews.size(); i++)
	{
		theSequencerViews[i]->setBounds(0, i*(getHeight()/theSequencerViews.size()), getWidth(), getHeight() / theSequencerViews.size());
	}
}