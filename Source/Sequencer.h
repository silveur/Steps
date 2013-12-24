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
class SequencerAudioProcessor;
class MidiCore;

class Sequencer: Thread
{
public:
	Sequencer(SequencerAudioProcessor* processor);
	~Sequencer();
	
	void setPosition(AudioPlayHead::CurrentPositionInfo& info);
	void run();
	void start(){startThread();}
	void stop(){stopThread(200);}
	void newStep();
	
private:
	MidiCore* theMidiCore;
	double theTempo;
	double thePPQPosition;
	int thePosition;
	int thePreviousPosition;
	SequencerAudioProcessor* theProcessor;
};

#endif  // SEQUENCER_H_INCLUDED
