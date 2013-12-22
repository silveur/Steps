/*
  ==============================================================================

    Step.h
    Created: 22 Dec 2013 12:32:36pm
    Author:  silvere letellier

  ==============================================================================
*/

#ifndef STEP_H_INCLUDED
#define STEP_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#define OFF	false	
#define ON	true

class Step: public ValueTree::Listener
{
public:
	Step(ValueTree& stepTree)
	{
		theStepTree = stepTree;
		thePitch = theStepTree.getProperty("Pitch");
		theVelocity = theStepTree.getProperty("Velocity");
		theState = theStepTree.getProperty("State");
		theStepTree.addListener(this);
	}
	~Step()
	{}
	int theVelocity;
	int thePitch;
	bool theState;
	ValueTree theStepTree;
	
	void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
	{
		if(String(property) == "Pitch")
		{
			thePitch = treeWhosePropertyHasChanged.getProperty(property);
		}
		else if(String(property) == "Velocity")
		{
			theVelocity = treeWhosePropertyHasChanged.getProperty(property);
		}
		else if(String(property) == "State")
		{
			theState = treeWhosePropertyHasChanged.getProperty(property);
		}
	}
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
};



#endif  // STEP_H_INCLUDED
