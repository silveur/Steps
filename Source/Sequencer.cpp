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
	wait = false;
	thePosition = 0;
	theRootNote = 0;
	theNoteOnClient = new NoteOnClient();
	theNoteOffClient = new NoteOffClient();
	theNoteOnClient->theSequencer = this;
	theNoteOffClient->theSequencer = this;
}

Sequencer::~Sequencer()
{
	delete theMidiCore;
}

void Sequencer::start(int noteNumber)
{
	for(int i=0; i<getNumClients();i++)	removeTimeSliceClient(getClient(i));
	theRootNote = noteNumber;
	thePosition = 0;
	wait = false;
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
	for(int i=0; i<getNumClients();i++)
	{
		if(getClient(i) == theNoteOnClient)
			removeTimeSliceClient(theNoteOnClient);
		else if(getClient(i) == theNoteOffClient)
			theNoteOffClient->useTimeSlice();
	}
}

void Sequencer::repositionSequencer()
{
	double mod = fmod(((thePPQPosition-(int)thePPQPosition)),0.25);
	theSyncTime = theStepTime - ((mod / 0.25) * theStepTime);
	if(theSyncTime <= 5)
	{
		addTimeSliceClient(theNoteOnClient, theSyncTime);
		wait = true;
	}
	
	if(!wait)
	{
		DBG("SyncTime: " + String(theSyncTime));
		addTimeSliceClient(theNoteOnClient, theSyncTime);
	}
}

int NoteOnClient::useTimeSlice()
{
	if(theSequencer->getNumClients() > 1)
	{
		DBG("Sync time: " + String(theSequencer->theSyncTime));
		theSequencer->removeTimeSliceClient(theSequencer->theNoteOffClient);
		theSequencer->theNoteOffClient->useTimeSlice();
		return -1;
	}
	theSequencer->theMidiCore->noteOn(theSequencer->theRootNote + theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch, theSequencer->theProcessor->theSteps[theSequencer->thePosition]->theVelocity);
	
	DBG("Note On for step " + String(theSequencer->thePosition) + ": " + String(theSequencer->theRootNote + theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch));
	
	theSequencer->addTimeSliceClient(theSequencer->theNoteOffClient,40);
	
	theSequencer->theNoteOffClient->nextNoteOff = theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch + theSequencer->theRootNote;
	theSequencer->thePosition = (theSequencer->thePosition + 1) % 16;
	theSequencer->theProcessor->setSequencerPosition(theSequencer->thePosition);
	theSequencer->wait = false;
	return -1;
}

int NoteOffClient::useTimeSlice()
{
	DBG("Note Off for step " + String(theSequencer->thePosition) + ": " + String(60 + nextNoteOff));
	DBG("------------------------------");
	theSequencer->theMidiCore->noteOff(nextNoteOff);
	return -1;
}


