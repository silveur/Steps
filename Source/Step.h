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

class Step
{
public:
	Step(): theVelocity(0), thePitch(0), theState(OFF)
	{}

	int theVelocity;
	int thePitch;
	bool theState;
	
};



#endif  // STEP_H_INCLUDED
