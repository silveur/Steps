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
	thePosition = 0;
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
	isPlaying = true;
	theThreadDelay = 1 / (theTempo / 60.0) * 250;
	repositionSequencer();
	startThread();
}

void Sequencer::run()
{
	while(!threadShouldExit() && theTempo != -1)
	{
		if(isSyncing)
		{
			wait(theSyncTime);
		}
		newStep();
		wait(theThreadDelay);
		
	}
}

void Sequencer::repositionSequencer()
{
	isSyncing = true;
	theSyncTime = 0;
}

void Sequencer::newStep()
{
	if(thePosition != 0)
		theMidiCore->noteOff(60 + theProcessor->theSteps[thePosition-1]->thePitch);
	else if(thePosition == 0)
		theMidiCore->noteOff(60 + theProcessor->theSteps[theProcessor->theSequencerLength-1]->thePitch);
	
	theMidiCore->noteOn(60 + theProcessor->theSteps[thePosition]->thePitch, theProcessor->theSteps[thePosition]->theVelocity);
	theProcessor->setSequencerPosition(thePosition);
	thePosition = (thePosition + 1) % theProcessor->theSequencerLength;
}


