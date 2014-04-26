/*
  ==============================================================================

	Sequencer.cpp
	Created: 22 Dec 2013 11:52:14pm
	Author:  silvere letellier

  ==============================================================================
*/

#include "Sequencer.h"

Sequencer::Sequencer(ValueTree& sequencerTree): theSequencerTree(sequencerTree)
{
	theMidiCore = new MidiCore();
	if (theSequencerTree.getNumProperties() > 0)
	{
		loadFromTree();
	}
	else
	{
		theChannel = 1;
		thePosition = 0;
		thePpqCount = 0;
		theLength = 16;
		theRootNote = 0;
		theRootOctave = 3;
		theShuffle = 0;
		theRange = 1;
		theOffset = 0;
		theOnOffStatus = ON;
		initSequencerTree();
		for (int i=0; i<16; i++)
		{
			ValueTree stepTree = sequencerTree.getChild(i);
			stepTree = ValueTree("Step" + String(i));
			theStepArray.add(new Step(stepTree));
			sequencerTree.addChild(stepTree, -1, nullptr);
		}
	}
	theSequencerTree.addListener(this);
}

Sequencer::~Sequencer()
{
}

void Sequencer::initSequencerTree()
{
	theSequencerTree.setProperty("Length", theLength, nullptr);
	theSequencerTree.setProperty("RootNote", theRootNote, nullptr);
	theSequencerTree.setProperty("RootOctave", theRootOctave, nullptr);
	theSequencerTree.setProperty("Shuffle", theShuffle, nullptr);
	theSequencerTree.setProperty("Range", theRange, nullptr);
	theSequencerTree.setProperty("Channel", theChannel, nullptr);
	theSequencerTree.setProperty("Status", theOnOffStatus, nullptr);
	theSequencerTree.setProperty("Offset", theOffset, nullptr);
}

void Sequencer::loadFromTree()
{
	theLength = theSequencerTree.getProperty("Length");
	theRootNote = theSequencerTree.getProperty("RootNote");
	theRootOctave = theSequencerTree.getProperty("RootOctave");
	theShuffle = theSequencerTree.getProperty("Shuffle");
	theRange = theSequencerTree.getProperty("Range");
	theChannel = theSequencerTree.getProperty("Channel");
	theOnOffStatus = theSequencerTree.getProperty("Status");
	theOffset = theSequencerTree.getProperty("Offset");
	
	for (int i=0; i<16; i++)
	{
		ValueTree stepTree = theSequencerTree.getChild(i);
		theStepArray.add(new Step(stepTree));
	}
}

void Sequencer::start()
{
	thePpqCount = -1;
	thePosition = -1 + theOffset;
	isIdle = false;
	waitForShuffle = false;
}

void Sequencer::stop()
{
	thePpqCount = -1;
	thePosition = -1;
	theMidiCore->killNotes();
	isIdle = true;
	waitForShuffle = false;
}

void Sequencer::carryOn()
{
	thePpqCount = -1;
	thePosition = -1;
	isIdle = false;
	waitForShuffle = false;
}

void Sequencer::triggerStep()
{
	thePosition = (thePosition+1) % theLength;
	if (theStepArray[thePosition]->theState)
	{
		Step* step = theStepArray[thePosition];
		MidiMessage onMsg = MidiMessage::noteOn(theChannel, (24 + (theRange*step->thePitch) + theRootNote) + (12*theRootOctave), (uint8)step->theVelocity);
		MidiMessage offMsg = MidiMessage::noteOff(theChannel, (24 + (theRange*step->thePitch) + theRootNote) + (12*theRootOctave), (uint8)step->theVelocity);
		theMidiCore->outputMidi(onMsg);
		theMidiCore->outputMidi(offMsg, step->theDecay);
	}
	theSequencerTree.setProperty("Position", thePosition, nullptr);
}

void Sequencer::handleIncomingMidiMessage(const MidiMessage& message)
{
	if (message.isMidiClock() && !isIdle && theOnOffStatus)
	{
		thePpqCount = (thePpqCount+1) % 6;
		if( waitForShuffle && (thePpqCount == theShuffle))
		{
			triggerStep();
			waitForShuffle = false;
		}
		else if (!thePpqCount)
		{
			if (!((thePosition +1) %2) || !theShuffle)
			{
				triggerStep();
			}
			else if((thePosition + 1) %2)
			{
				waitForShuffle = true;
			}
		}
	}
	
	else if(message.isSongPositionPointer())
	{

	}
	else if (message.isMidiStart())
	{
		start();
	}
	else if (message.isMidiStop())
	{
		stop();
	}
	else if (message.isMidiContinue())
	{
		carryOn();
	}
}

void Sequencer::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
	if(String(property) == "MidiOutput")
	{
		String midiOut = tree.getProperty(property);
		theMidiCore->openMidiOutput(midiOut);
	}
	else if(String(property) == "Length")
	{
		theLength = tree.getProperty(property);
	}
	else if(String(property) == "Offset")
	{
		theOffset = tree.getProperty(property);
	}
	else if(String(property) == "RootOctave")
	{
		theRootOctave = tree.getProperty(property);
	}
	else if(String(property) == "RootNote")
	{
		theRootNote = tree.getProperty(property);
	}
	else if(String(property) == "Shuffle")
	{
		theShuffle = tree.getProperty(property);
	}
	else if(String(property) == "Range")
	{
		theRange = tree.getProperty(property);
	}
	else if(String(property) == "Channel")
	{
		theChannel = tree.getProperty(property);
	}
	else if(String(property) == "Status")
	{
		theOnOffStatus = tree.getProperty(property);
	}
	else if(String(property) == "KickBack")
	{
		thePosition = -1;
		tree.setProperty(property, 0, nullptr);
	}
}
