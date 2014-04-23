/*
  ==============================================================================

    RootView.h
    Created: 23 Apr 2014 12:15:42pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef ROOTVIEW_H_INCLUDED
#define ROOTVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Master.h"
#include "SequencerView.h"

class HeaderView;

class RootView: public Component
{
public:
	RootView(Master* master);
	~RootView();
	void resized();
	void paint(Graphics& g);
	void addSequencer();
	void removeSequencer();
	void updatePositions();
	OwnedArray<SequencerView> theSequencerViews;
private:
	Rectangle<int> theMainScreen;
	
	ScopedPointer<HeaderView> theHeaderView;
	Master* theMaster;
};



#endif  // ROOTVIEW_H_INCLUDED
