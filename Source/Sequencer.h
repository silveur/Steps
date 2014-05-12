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
	void handleIncomingMidiMessage (const MidiMessage& message);
	void stop();
	void start();
	
private:
	void loadFromTree();
	void initSequencerTree();
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}

	void carryOn();
	void triggerStep();
	ScopedPointer<MidiCore> theMidiCore;
	OwnedArray<Step> theStepArray;
	ValueTree theSequencerTree;
	
	int theChannel;
	int thePosition;
	int theSyncTime;
	int theRootNote;
	int theRootOctave;
	int thePpqCount;
	int theLength;
	int theRange;
	int theShuffle;
	int theOffset;
	float theSpeed;
	bool theOnOffStatus;
	bool isIdle;
	bool waitForShuffle;
};


#endif  // SEQUENCER_H_INCLUDED
