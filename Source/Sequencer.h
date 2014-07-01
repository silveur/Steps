/* =====================================================================
 
 * Steps - Midi sequencer>
 * Copyright (C) 2014  Silvere Letellier for Nummer Music
 * Contact: <silvere.letellier@gmail.com>
 
 -----------------------------------------------------------------------
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ===================================================================== */

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
	~Sequencer() {}
	void handleIncomingMidiMessage (const MidiMessage& message);
	void stop();
	void start();
	
private:
	void triggerNote(int stepIndex);
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
	friend class Master;
};

#endif  // SEQUENCER_H_INCLUDED
