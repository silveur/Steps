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

int Sequencer::theStepTime = 0;

Sequencer::Sequencer(SequencerAudioProcessor* processor): TimeSliceThread("SequencerThread")
{
	theMidiCore = new MidiCore();
	theMidiCore->openMidiOutput(0);
	theProcessor = processor;
	theTempo = -1;
	thePosition = 0;
	theNoteOnClient = new NoteOnClient();
	theNoteOffClient = new NoteOffClient();
	theNoteOnClient->theSequencer = this;
	theNoteOffClient->theSequencer = this;
}

Sequencer::~Sequencer()
{
	delete theMidiCore;
}

void Sequencer::start()
{
	for(int i=0; i<getNumClients();i++)	removeTimeSliceClient(getClient(i));
	thePosition = 0;
	setPosition(theProcessor->lastPosInfo);
	addTimeSliceClient(theNoteOnClient, 0);
	startThread();
}

void Sequencer::setPosition(AudioPlayHead::CurrentPositionInfo& info)
{
	theTempo = info.bpm;
	thePPQPosition = info.ppqPosition;
	theStepTime = 1.0 / (theTempo / 60.0) * 250;
	repositionSequencer();
}

void Sequencer::stop()
{
	for(int i=0; i<getNumClients();i++)	removeTimeSliceClient(getClient(i));
	stopThread(theStepTime);
}

void Sequencer::repositionSequencer()
{
	float mod = fmod(((thePPQPosition-(int)thePPQPosition)),0.25);
	int syncTime = theStepTime - ((mod / 0.25) * theStepTime);
	if(syncTime == theStepTime)	syncTime = 0;
//	for(int i=0; i<getNumClients();i++)
//	{
//		if(getClient(i) == theNoteOnClient)
//			return; 
//	}
//	addTimeSliceClient(theNoteOnClient, syncTime);
}

int NoteOnClient::useTimeSlice()
{
	theSequencer->theMidiCore->noteOn(60 + theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch, theSequencer->theProcessor->theSteps[theSequencer->thePosition]->theVelocity);
	
	theSequencer->addTimeSliceClient(theSequencer->theNoteOffClient,80);
	theSequencer->theNoteOffClient->nextNoteOff = theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch;
	
	theSequencer->thePosition = (theSequencer->thePosition + 1) % 16;
	theSequencer->theProcessor->setSequencerPosition(theSequencer->thePosition);
	
	return theSequencer->theStepTime;
}

int NoteOffClient::useTimeSlice()
{
	theSequencer->theMidiCore->noteOff(60 + nextNoteOff);
	return -1;
}


