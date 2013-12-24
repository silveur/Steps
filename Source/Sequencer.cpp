/*
  ==============================================================================

    Sequencer.cpp
    Created: 22 Dec 2013 11:52:14pm
    Author:  silvere letellier

  ==============================================================================
*/

#include "Sequencer.h"

Sequencer::Sequencer(): Thread("SequencerThread")
{
	theTempo = -1;
	thePosition = -1;
}

void Sequencer::setPosition(AudioPlayHead::CurrentPositionInfo& info)
{
	thePosition = info.ppqPosition;
	theTempo = info.bpm;
}

void Sequencer::run()
{
	while(!threadShouldExit())
	{
		
		sleep(200);
	}
	
	
}