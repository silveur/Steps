/*
  ==============================================================================

    Master.h
    Created: 23 Apr 2014 12:36:28pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef MASTER_H_INCLUDED
#define MASTER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sequencer.h"

class Master: public MidiInputCallback
{
public:
	Master()
	{
		theMidiInput = MidiInput::createNewDevice("Sequencer", this);
		for (int i=0;i<3;i++)
		{
			theSequencerArray.add(new Sequencer(i));
		}
		theMidiInput->start();
	}
	~Master()
	{
		theMidiInput->stop();
	}
	
	void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
	{
		for (int i=0;i<theSequencerArray.size();i++)
		{
			theSequencerArray[i]->handleIncomingMidiMessage(message);
		}
	}
	
	OwnedArray<Sequencer>& getSequencerArray()
	{
		return theSequencerArray;
	}
	
private:
	OwnedArray<Sequencer> theSequencerArray;
	ScopedPointer<MidiInput> theMidiInput;
};



#endif  // MASTER_H_INCLUDED
