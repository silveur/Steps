/*
  ==============================================================================

    Sequencer.h
    Created: 22 Dec 2013 11:52:14pm
    Author:  silvere letellier

  ==============================================================================
*/

#ifndef SEQUENCER_H_INCLUDED
#define SEQUENCER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Sequencer
{
public:
	Sequencer(){}
	~Sequencer(){}
	
	void setPosition(AudioPlayHead::CurrentPositionInfo& info);
	
private:
	
	
	
};

#endif  // SEQUENCER_H_INCLUDED
