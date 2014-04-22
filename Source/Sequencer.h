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
#include "MidiCore.h"

class SequencerAudioProcessor;
class MidiCore;
class NoteManager;

class Sequencer: public MidiInputCallback
{
public:
	Sequencer();
	~Sequencer();
	int* getPosition() { return &thePosition; }
	MidiCore* getMidiCore() { return theMidiCore; }
	
private:
	void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);
	void stop();
	void start();
	void setPosition(int beatPosition);
	ScopedPointer<MidiCore> theMidiCore;
	ScopedPointer<MidiInput> theMidiInput;
	double theTempo;
	int thePosition;
	int theSyncTime;
	int theRootNote;
	int count;
};


#endif  // SEQUENCER_H_INCLUDED
