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
#include "Sequencer.h"
#include "SequencerView.h"

class RootView: public Component
{
public:
	RootView(OwnedArray<Sequencer>& sequencerArray);
	~RootView();
	void resized();
	
private:
	Rectangle<int> theMainScreen;
	OwnedArray<SequencerView> theSequencerViews;
};



#endif  // ROOTVIEW_H_INCLUDED
