/* =====================================================================
 
 * Steps - Midi sequencer>
 * Copyright (C) 2014  Silvere Letellier for Nummer Music
 * Contact: <silvere.letellier@gmail.com>
 
 -----------------------------------------------------------------------
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ===================================================================== */

#ifndef STEP_H_INCLUDED
#define STEP_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

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
		theStepTree.setProperty("State", ON, nullptr);
		theStepTree.setProperty("Decay", theDecay, nullptr);
	}
	
	void loadFromTree()
	{
		thePitch = theStepTree.getProperty("Pitch");
		theVelocity = theStepTree.getProperty("Velocity");
		theState = (StepStates)(int)theStepTree.getProperty("State");
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
			theState = (StepStates)(int)tree.getProperty(property);
		}
		else if(String(property) == "Decay")
		{
			theDecay = tree.getProperty(property);
		}
	}
	
	int theVelocity;
	int thePitch;
	int theDecay;
	StepStates theState;
	
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}

	ValueTree theStepTree;
	friend class Sequencer;

};

#endif  // STEP_H_INCLUDED
