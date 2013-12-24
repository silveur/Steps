/*
  ==============================================================================

    Sequencer.cpp
    Created: 22 Dec 2013 11:52:14pm
    Author:  silvere letellier

  ==============================================================================
*/

#include "Sequencer.h"
#include "PluginProcessor.h"

Sequencer::Sequencer(SequencerAudioProcessor* processor): Thread("SequencerThread")
{
	theProcessor = processor;
	theTempo = -1;
	thePosition = -1;
	setPriority(8);
}

void Sequencer::setPosition(AudioPlayHead::CurrentPositionInfo& info)
{
	thePPQPosition = info.ppqPosition;
	int subPos = fmod(thePPQPosition, 1.0)*4;
	thePosition = subPos + ((int)thePPQPosition%4) * 4;
	theTempo = info.bpm;
	startThread();
}

void Sequencer::run()
{
	while(!threadShouldExit() && theTempo != -1 && thePosition != -1)
	{
		theProcessor->setSequencerPosition(thePosition);
		sleep(20);
	}
}

void Sequencer::newStep()
{
	
	
	
}


