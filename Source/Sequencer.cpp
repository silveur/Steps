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
	if(!isThreadRunning() && theTempo != -1)
		startThread();
	repositionSequencer();
}

void Sequencer::run()
{
	while(!threadShouldExit())
	{
		wait(-1);
		if(threadShouldExit()) return;
		sleep(theSyncTime);
		newStep();
	}
}

void Sequencer::repositionSequencer()
{
	int subPos = fmod(thePPQPosition, 1.0)*4;
	int newPos = subPos + ((int)thePPQPosition%4) * 4;
	thePosition = newPos;
	int timeBetweenSteps = 1.0 / (theTempo / 60.0) * 250;
	float mod = fmod(((thePPQPosition-(int)thePPQPosition)),0.25);
	float diff = timeBetweenSteps - ((mod / 0.25) * timeBetweenSteps);
	theSyncTime = diff;
	notify();
}

void Sequencer::newStep()
{
	if(thePosition < 0)
		return;
	if(thePosition != 0)
		theMidiCore->noteOff(60 + theProcessor->theSteps[thePosition-1]->thePitch);
	else if(thePosition == 0)
		theMidiCore->noteOff(60 + theProcessor->theSteps[theProcessor->theSequencerLength-1]->thePitch);
	
	theMidiCore->noteOn(60 + theProcessor->theSteps[thePosition]->thePitch, theProcessor->theSteps[thePosition]->theVelocity);
	thePosition = (thePosition + 1) % theProcessor->theSequencerLength;
	theProcessor->setSequencerPosition(thePosition);
}


