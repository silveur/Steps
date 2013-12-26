/*
  ==============================================================================

    Sequencer.h
    Created: 22 Dec 2013 11:52:14pm
    Author:  silvere letellier

  ==============================================================================
*/

#ifndef SEQUENCER_H_INCLUDED
#define SEQUENCER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Step.h"
class SequencerAudioProcessor;
class MidiCore;
class NoteOnClient;
class NoteOffClient;
class Sequencer: public TimeSliceThread
{
public:
	Sequencer(SequencerAudioProcessor* processor);
	~Sequencer();
	
	void setPosition(AudioPlayHead::CurrentPositionInfo& info);
//	void run();
	void stop(){stopThread(200);}
	void start(){}
	void newStep();
	void repositionSequencer();
	MidiCore* theMidiCore;
	SequencerAudioProcessor* theProcessor;
	NoteOnClient* theNoteOnClient;
	NoteOffClient* theNoteOffClient;
	double theTempo;
	double thePPQPosition;
	int thePosition;
	int theSyncTime;
	bool isPlaying;
};

class NoteOnClient: public TimeSliceClient
{
public:
	NoteOnClient(){}
	~NoteOnClient(){}
	int useTimeSlice();
	Sequencer* theSequencer;
};

class NoteOffClient: public TimeSliceClient
{
public:
	NoteOffClient(){}
	~NoteOffClient(){}
	int useTimeSlice();
	Sequencer* theSequencer;
};

#endif  // SEQUENCER_H_INCLUDED
