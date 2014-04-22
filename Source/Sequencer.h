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

class Sequencer: public MidiInputCallback, public ValueTree::Listener
{
public:
	Sequencer();
	~Sequencer();
	MidiCore* getMidiCore() { return theMidiCore; }
	ValueTree& getSequencerTree() { return theSequencerTree; }
	
private:
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
	void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);
	void stop();
	void start();
	void setPosition(int beatPosition);
	ScopedPointer<MidiCore> theMidiCore;
	ScopedPointer<MidiInput> theMidiInput;
	OwnedArray<Step> theStepArray;
	ValueTree theSequencerTree;
	int thePosition;
	int theSyncTime;
	int theRootNote;
	int ppqCount;
};


#endif  // SEQUENCER_H_INCLUDED
