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

Sequencer::Sequencer(SequencerAudioProcessor* processor, MidiCore* midiCore): Thread("SequencerThread")
{
	theMidiCore = midiCore;
	theProcessor = processor;
	theTempo = -1;
	thePosition = 0;
	theRootNote = 0;
	theNoteOnClient = new NoteOnClient();
	theNoteOffClient = new NoteOffClient();
	theNoteOnClient->theSequencer = this;
	theNoteOffClient->theSequencer = this;
	theNoteManager = new NoteManager();
}

Sequencer::~Sequencer()
{
	delete theMidiCore;
}

void Sequencer::start()
{
//	for(int i=0; i<getNumClients();i++)	removeTimeSliceClient(getClient(i));
	theRootNote = 48;
	thePosition = 0;
//	setPosition(theProcessor->lastPosInfo);
//	addTimeSliceClient(theNoteOnClient, 0);
	startThread();
}

void Sequencer::setPosition(AudioPlayHead::CurrentPositionInfo& info)
{
	theTempo = info.bpm;
	thePPQPosition = info.ppqPosition;
	theStepTime = 1.0 / (theTempo / 60.0) * 250;
//	DBG("Position:" + String(info.ppqPosition));
	repositionSequencer();
}

void Sequencer::stop()
{
//	for(int i=0; i<getNumClients();i++)
//	{
//		if(getClient(i) == theNoteOnClient)
//			removeTimeSliceClient(theNoteOnClient);
//		else if(getClient(i) == theNoteOffClient)
//			theNoteOffClient->useTimeSlice();
//	}
	stopThread(30);
}

void Sequencer::run()
{
	theSyncTime = theStepTime - 1;
	while (!threadShouldExit())
	{
		theSyncTime++;
		if(theSyncTime == theStepTime)
		{
			theNoteManager->addTimeSliceClient(theNoteOnClient);
			theNoteManager->addTimeSliceClient(theNoteOffClient, 40);
			
			theSyncTime = 0;
		}
		wait(1);
	}
}

void Sequencer::repositionSequencer()
{
	double mod = fmod(((thePPQPosition-(int)thePPQPosition)),0.25);
	theSyncTime = ((mod / 0.25) * theStepTime);
}

int NoteOnClient::useTimeSlice()
{

    if(theSequencer->theProcessor->theSteps[theSequencer->thePosition]->theState)
	theSequencer->theMidiCore->noteOn(theSequencer->theRootNote + theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch, theSequencer->theProcessor->theSteps[theSequencer->thePosition]->theVelocity);
//	DBG("Note On for step " + String(theSequencer->thePosition) + ": " + String(theSequencer->theRootNote + theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch));
	theSequencer->theNoteOffClient->nextNoteOff = theSequencer->theProcessor->theSteps[theSequencer->thePosition]->thePitch + theSequencer->theRootNote;
	theSequencer->thePosition = (theSequencer->thePosition + 1) % theSequencer->theProcessor->theSequencerLength;
	theSequencer->theProcessor->setSequencerPosition(theSequencer->thePosition);
	return -1;
}

int NoteOffClient::useTimeSlice()
{
//	DBG("Note Off: " + String(nextNoteOff));
//	DBG("------------------------------");
	theSequencer->theMidiCore->noteOff(nextNoteOff);
	return -1;
}


