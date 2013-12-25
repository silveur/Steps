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

class Sequencer: Thread
{
public:
	Sequencer(SequencerAudioProcessor* processor);
	~Sequencer();
	
	void setPosition(AudioPlayHead::CurrentPositionInfo& info);
	void run();
	void stop(){stopThread(500);}
	void newStep();
	void repositionSequencer();
private:
	MidiCore* theMidiCore;
	SequencerAudioProcessor* theProcessor;
	double theTempo;
	double thePPQPosition;
	int thePosition;
	int theThreadDelay;
	int theSyncTime;
	bool isPlaying;
	bool isSyncing;
};

#endif  // SEQUENCER_H_INCLUDED
