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
#include "LookAndFeel.h"

class HeaderView;

class ControllerView: public Component, public ValueTree::Listener, public KeyListener
{
public:
	ControllerView(ValueTree& masterTree);
	~ControllerView();
	void resized();
	void kickBack();
	void addSequencer(ValueTree& sequencerTreeToAdd);
	void removeSequencer();
	void updatePositions();
	const int getNumOfSequencer() const;
	ValueTree& getMasterTree() { return theMasterTree; }
	Rectangle<int>& getScreenSize() { return theMainScreen; }
	bool keyPressed (const KeyPress &key, Component *originatingComponent)
	{
		for (int i=0; i<theMasterTree.getNumChildren(); i++)
		{
			
			theSequencerViews[i]->setBounds(0, totalHeigth, sequencerWidth, sequencer32Height);
		}
		if (key.isKeyCode(90) && key.getModifiers() == ModifierKeys::commandModifier)
		{
			theUndoManager->undo();
		}
		else if (key.isKeyCode(90) && key.getModifiers() == (ModifierKeys::commandModifier | ModifierKeys::shiftModifier))
		{
			theUndoManager->redo();
		}
		return true;
	}

	
private:
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property){}
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& child){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& child){}
	void valueTreeChildOrderChanged (ValueTree& parent){}
	void valueTreeParentChanged (ValueTree& tree){}
	OwnedArray<SequencerView> theSequencerViews;
	ScopedPointer<SeqLookAndFeel> theLookAndFeel;
	Rectangle<int> theMainScreen;
	HeaderView* theHeaderView;
	ValueTree theMasterTree;
};



#endif  // ROOTVIEW_H_INCLUDED
