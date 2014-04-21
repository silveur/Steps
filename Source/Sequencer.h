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

class Sequencer
{
public:
	Sequencer(SequencerAudioProcessor* processor, MidiCore* midiCore);
	~Sequencer();
	
	void processBlock(AudioSampleBuffer& buffer, AudioPlayHead::CurrentPositionInfo& info, MidiBuffer& messages);
	void stop();
	void newStep();
	void repositionSequencer();
	MidiCore* theMidiCore;
	SequencerAudioProcessor* theProcessor;
	double theTempo;
	double thePPQPosition;
	int thePosition;
	static int theStepTime;
	int theSyncTime;
	int theRootNote;
	int beat;
	int step;
};

#endif  // SEQUENCER_H_INCLUDED
