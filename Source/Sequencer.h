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

class Sequencer: public ValueTree::Listener
{
public:
	Sequencer(ValueTree& sequencerTree);
	~Sequencer();
	MidiCore* getMidiCore() { return theMidiCore; }
	ValueTree& getSequencerTree() { return theSequencerTree; }
	void handleIncomingMidiMessage (const MidiMessage& message);
	
private:
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
	void stop();
	void start();
	void carryOn();
	void triggerStep();
	ScopedPointer<MidiCore> theMidiCore;
	OwnedArray<Step> theStepArray;
	ValueTree theSequencerTree;
	
	int thePosition;
	int theSyncTime;
	int theRootNote;
	int theRootOctave;
	int thePpqCount;
	int theLength;
	int theRange;
	int theShuffle;
	bool isIdle;
	bool waitForShuffle;
};


#endif  // SEQUENCER_H_INCLUDED
