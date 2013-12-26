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

Sequencer::Sequencer(SequencerAudioProcessor* processor): TimeSliceThread("SequencerThread")
{
	theMidiCore = new MidiCore();
	theMidiCore->openMidiOutput(0);
	theProcessor = processor;
	theTempo = -1;
	thePosition = 0;
	theNoteOnClient = new NoteOnClient();
	theNoteOffClient = new NoteOffClient();
	addTimeSliceClient(theNoteOnClient, 5);
	theNoteOnClient->theSequencer = this;
	theNoteOffClient->theSequencer = this;
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
	if(theTempo != -1)
		startThread();
//	if(!isThreadRunning() && theTempo != -1)
//		startThread();
//	repositionSequencer();
}

//void Sequencer::run()
//{
////	while(!threadShouldExit())
////	{
////		wait(-1);
////		if(threadShouldExit()) return;
////		sleep(theSyncTime);
////		newStep();
////	}
//}

void Sequencer::repositionSequencer()
{
//	int subPos = fmod(thePPQPosition, 1.0)*4;
//	int newPos = subPos + ((int)thePPQPosition%4) * 4;
//	thePosition = newPos;
//	int timeBetweenSteps = 1.0 / (theTempo / 60.0) * 250;
//	float mod = fmod(((thePPQPosition-(int)thePPQPosition)),0.25);
//	float diff = timeBetweenSteps - ((mod / 0.25) * timeBetweenSteps);
//	theSyncTime = diff;
//	notify();
}

void Sequencer::newStep()
{
}

int NoteOnClient::useTimeSlice()
{
	theSequencer->theMidiCore->noteOn(60 + theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch, theSequencer->theProcessor->theSteps[theSequencer->thePosition]->theVelocity);
	int stepTime = 1.0 / (theSequencer->theTempo / 60.0) * 250;
	theSequencer->addTimeSliceClient(theSequencer->theNoteOffClient,stepTime/2);
	
	return stepTime;
}

int NoteOffClient::useTimeSlice()
{
	theSequencer->theMidiCore->noteOff(60 + theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch);
	theSequencer->theProcessor->setSequencerPosition(theSequencer->thePosition);
	theSequencer->thePosition = (theSequencer->thePosition + 1) % theSequencer->theProcessor->theSequencerLength;
	theSequencer->removeTimeSliceClient(this);
	return -1;
}


