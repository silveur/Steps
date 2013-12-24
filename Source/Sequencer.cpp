/*
  ==============================================================================

    Sequencer.cpp
    Created: 22 Dec 2013 11:52:14pm
    Author:  silvere letellier

  ==============================================================================
*/

#include "Sequencer.h"
#include "PluginProcessor.h"
#include "MidiCore.h"

Sequencer::Sequencer(SequencerAudioProcessor* processor): Thread("SequencerThread")
{
	theMidiCore = new MidiCore();
	theMidiCore->openMidiOutput(0);
	theProcessor = processor;
	theTempo = -1;
	thePosition = -1;
	thePreviousPosition = -1;
	setPriority(8);
}

Sequencer::~Sequencer()
{
	delete theMidiCore;
}

void Sequencer::setPosition(AudioPlayHead::CurrentPositionInfo& info)
{
	theTempo = info.bpm;
	thePPQPosition = info.ppqPosition;
	int subPos = (int)(fmod(thePPQPosition, 1.0)*4.0);
	thePosition = subPos + ((int)thePPQPosition%4) * 4;
	if(thePosition != thePreviousPosition)
		newStep();
	thePreviousPosition = thePosition;
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
	if(thePosition != 0)
		theMidiCore->noteOff(60 + theProcessor->theSteps[thePosition-1]->thePitch);
	else if(thePosition == 0)
		theMidiCore->noteOff(60 + theProcessor->theSteps[theProcessor->theSequencerLength-1]->thePitch);
	
	theMidiCore->noteOn(60 + theProcessor->theSteps[thePosition]->thePitch, theProcessor->theSteps[thePosition]->theVelocity);
}


