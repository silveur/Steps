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
	setPosition(theProcessor->lastPosInfo);
	startThread();
}

void Sequencer::setPosition(AudioPlayHead::CurrentPositionInfo& info)
{
	theTempo = info.bpm;
	thePPQPosition = info.ppqPosition;
	int subPos = fmod(thePPQPosition, 1.0)*4;
	thePosition = abs(subPos + ((int)thePPQPosition % 4) * 4);
	theStepTime = 1.0 / (theTempo / 60.0) * 250;
	repositionSequencer();
}

void Sequencer::stop()
{
	for(int i=0; i<getNumClients();i++)
		removeTimeSliceClient(getClient(i));
	stopThread(theStepTime);
}

void Sequencer::repositionSequencer()
{
	float mod = fmod(((thePPQPosition-(int)thePPQPosition)),0.25);

	int syncTime = theStepTime - ((mod / 0.25) * theStepTime);
	if(syncTime == theStepTime)
		syncTime = 0;
	DBG("SyncTime: " +  String(syncTime));
	addTimeSliceClient(theNoteOnClient, syncTime);
}

int NoteOnClient::useTimeSlice()
{
	DBG("Step: " + String(theSequencer->thePosition));
	theSequencer->theMidiCore->noteOn(60 + theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch, theSequencer->theProcessor->theSteps[theSequencer->thePosition]->theVelocity);
	
	theSequencer->addTimeSliceClient(theSequencer->theNoteOffClient,80);
	theSequencer->nextNoteOff = theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch;
	theSequencer->theProcessor->setSequencerPosition(theSequencer->thePosition);
	theSequencer->thePosition = (theSequencer->thePosition + 1) % 16;
	DBG("Step after sent: " + String(theSequencer->thePosition));
	return -1;
}

int NoteOffClient::useTimeSlice()
{
	theSequencer->theMidiCore->noteOff(60 + theSequencer->nextNoteOff);
	return -1;
}


