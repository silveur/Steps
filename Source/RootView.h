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

class RootView: public Component, public ValueTree::Listener
{
public:
	RootView(Master* master);
	~RootView();
	void resized();
	void addSequencer();
	void removeSequencer();
	void updatePositions();
	int getNumOfSequencer();
	void updatePresetList();
	
private:
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property){}
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& child);
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& child);
	void valueTreeChildOrderChanged (ValueTree& parent){}
	void valueTreeParentChanged (ValueTree& tree){}
	OwnedArray<SequencerView> theSequencerViews;
	Rectangle<int> theMainScreen;
	HeaderView* theHeaderView;
	Master* theMaster;
	ValueTree theMasterTree;
};



#endif  // ROOTVIEW_H_INCLUDED