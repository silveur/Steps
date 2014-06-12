/*
  ==============================================================================

    Randomiser.h
    Created: 12 Jun 2014 12:15:42pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef RANDOMISER_H_INCLUDED
#define RANDOMISER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "SequencerView.h"

class Randomiser: public Component, public ButtonListener, public ValueTree::Listener
{
public:
	Randomiser(SequencerView* sequencerView, ValueTree& sequencerTree);
	~Randomiser() {}
	void resized();
	void paint(Graphics& g);
	void buttonClicked(Button* button);
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
private:
	SequencerView* theSequencerView;
	ValueTree theSequencerTree;
	ScopedPointer<TextButton> theRandomAllButton;
	ScopedPointer<TextButton> theResetAllButton;
	ScopedPointer<ToggleButton> theVelocityButton;
	ScopedPointer<ToggleButton> theDecayButton;
	ScopedPointer<ToggleButton> thePitchButton;
	ScopedPointer<ToggleButton> theStateButton;
};



#endif  // RANDOMISER_H_INCLUDED
