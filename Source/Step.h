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
	Step()
	{
		theStepTree = ValueTree("StepTree");
		thePitch = 0; theVelocity = 127; theState = ON;
		theStepTree.setProperty("Pitch", thePitch, nullptr);
		theStepTree.setProperty("Velocity", theVelocity, nullptr);
		theStepTree.setProperty("State", theState, nullptr);
		theStepTree.addListener(this);
	}
	
	~Step()	{}

	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
	{
		if(String(property) == "Pitch")
		{
			thePitch = tree.getProperty(property);
		}
		else if(String(property) == "Velocity")
		{
			theVelocity = tree.getProperty(property);
		}
		else if(String(property) == "State")
		{
			theState = tree.getProperty(property);
		}
	}
	
	ValueTree& getValueTree()
	{
		return theStepTree;
	}
	
	int theVelocity;
	int thePitch;
	bool theState;
	int theDecay;
	
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
	
private:
	ValueTree theStepTree;

};



#endif  // STEP_H_INCLUDED
