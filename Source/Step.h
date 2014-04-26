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

extern UndoManager* theUndoManager;

class Step: public ValueTree::Listener
{
public:
	Step(ValueTree& stepTree): theStepTree(stepTree)
	{
		if (theStepTree.getNumProperties() > 0)
		{
			loadFromTree();
		}
		else
		{
			thePitch = 0;
			theVelocity = 127;
			theState = ON;
			theDecay = 40;
			initStepTree();
			theStepTree.addListener(this);
		}
	}
	
	~Step() {}

	ValueTree& getValueTree()
	{
		return theStepTree;
	}
	
private:
	void initStepTree()
	{
		theStepTree.setProperty("Pitch", thePitch, nullptr);
		theStepTree.setProperty("Velocity", theVelocity, nullptr);
		theStepTree.setProperty("State", theState, nullptr);
		theStepTree.setProperty("Decay", theDecay, nullptr);
	}
	
	void loadFromTree()
	{
		thePitch = theStepTree.getProperty("Pitch");
		theVelocity = theStepTree.getProperty("Velocity");
		theState = theStepTree.getProperty("State");
		theDecay = theStepTree.getProperty("Decay");
	}
	
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
		else if(String(property) == "Decay")
		{
			theDecay = tree.getProperty(property);
		}
	}
	
	int theVelocity;
	int thePitch;
	int theDecay;
	bool theState;
	
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}

	ValueTree theStepTree;
	friend class Sequencer;

};

#endif  // STEP_H_INCLUDED
