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
		thePitch = theStepTree.getProperty("Pitch", 0);
		theVelocity = theStepTree.getProperty("Velocity", 127);
		theState = theStepTree.getProperty("State", ON);
		theDecay = theStepTree.getProperty("Decay", 40);
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
		else if(String(property) == "Decay")
		{
			theDecay = tree.getProperty(property);
		}
	}
	
	ValueTree& getValueTree()
	{
		return theStepTree;
	}
	
private:
	static void initStepTree(ValueTree& tree)
	{
		tree.setProperty("Pitch", 0, nullptr);
		tree.setProperty("Velocity", 127, nullptr);
		tree.setProperty("State", ON, nullptr);
		tree.setProperty("Decay", 40, nullptr);
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
