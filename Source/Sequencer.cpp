/* =====================================================================
 
 * Steps - Midi sequencer
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 ===================================================================== */

#include "Sequencer.h"

OwnedArray<Suite> Suite::theSuites = OwnedArray<Suite>();

Sequencer::Sequencer(ValueTree& sequencerTree): theSequencerTree(sequencerTree), theCurrentSuite(nullptr)
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
		theSpeed = 1;
		theOnOffStatus = ON;
		theTriggerMode = SEQUENCER;
		initSequencerTree();
		for (int i=0; i<32; i++)
		{
			ValueTree stepTree = sequencerTree.getChild(i);
			stepTree = ValueTree("Step" + String(i));
			theStepArray.add(new Step(stepTree));
			sequencerTree.addChild(stepTree, -1, nullptr);
		}
	}
	theSequencerTree.addListener(this);
}

void Sequencer::triggerNote(int stepIndex = -1)
{
	Step* step;
	if (stepIndex == -1) step = theStepArray[thePosition];
	else step = theStepArray[stepIndex];
	
	int note = 24 + (step->thePitch) + theRootNote + (12*theRootOctave);
	
	if (theCurrentSuite != nullptr && theCurrentSuite->getSuiteType() == CHORD)
	{
		Array<MidiMessage> notesOn, notesOff;
		for (int i=0;i<theCurrentSuite->getNotes().size();i++)
			notesOn.add(MidiMessage::noteOn(theChannel, note + theCurrentSuite->getNotes()[i], (uint8)step->theVelocity));
		theMidiCore->outputMidi(notesOn);
		
		for (int i=0;i<theCurrentSuite->getNotes().size();i++)
			notesOff.add(MidiMessage::noteOff(theChannel, note + theCurrentSuite->getNotes()[i], (uint8)step->theVelocity));
		theMidiCore->outputMidi(notesOff, step->theDecay);
	}
	else
	{
		MidiMessage onMsg = MidiMessage::noteOn(theChannel, note, (uint8)step->theVelocity);
		MidiMessage offMsg = MidiMessage::noteOff(theChannel, note, (uint8)step->theVelocity);
		theMidiCore->outputMidi(onMsg);
		theMidiCore->outputMidi(offMsg, step->theDecay);
	}
}

void Sequencer::initSequencerTree()
{
	theSequencerTree.setProperty("TriggerMode", theTriggerMode, nullptr);
	theSequencerTree.setProperty("Length", theLength, nullptr);
	theSequencerTree.setProperty("RootNote", theRootNote, nullptr);
	theSequencerTree.setProperty("RootOctave", theRootOctave, nullptr);
	theSequencerTree.setProperty("Shuffle", theShuffle, nullptr);
	theSequencerTree.setProperty("Range", theRange, nullptr);
	theSequencerTree.setProperty("Channel", theChannel, nullptr);
	theSequencerTree.setProperty("Status", theOnOffStatus, nullptr);
	theSequencerTree.setProperty("Offset", theOffset, nullptr);
	theSequencerTree.setProperty("Suite", 0, nullptr);
	theSequencerTree.setProperty("Speed", theSpeed, nullptr);
	theSequencerTree.setProperty("RandVelocity", true, nullptr);
	theSequencerTree.setProperty("RandDecay", true, nullptr);
	theSequencerTree.setProperty("RandPitch", true, nullptr);
	theSequencerTree.setProperty("RandState", true, nullptr);
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
	theSpeed = theSequencerTree.getProperty("Speed");
	theTriggerMode = (TriggerMode)(int)theSequencerTree.getProperty("TriggerMode");
	int suiteToFind = theSequencerTree.getProperty("Suite");
	theCurrentSuite = Suite::getSuiteWithId(suiteToFind - 1);
	String midiOutput = theSequencerTree.getProperty("MidiOutput").toString();
	theMidiCore->openMidiOutput(midiOutput);
	for (int i=0; i<32; i++)
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
	while (theStepArray[thePosition]->theState == JUMP)	thePosition = (thePosition+1) % theLength;
	if (theStepArray[thePosition]->theState == ON) triggerNote();
	theSequencerTree.setProperty("Position", thePosition, nullptr);
}

void Sequencer::handleIncomingMidiMessage(const MidiMessage& message)
{
	if (message.isMidiClock() && !isIdle && theOnOffStatus)
	{
		thePpqCount = (thePpqCount+1) % (int)(6.0f/theSpeed);
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
	else if(String(property) == "Speed")
	{
		theSpeed = tree.getProperty(property);
	}
	else if(String(property) == "Trigger")
	{
		int index= tree.getProperty(property);
		if (index != -1)
		{
			triggerNote(index);
			tree.setProperty(property, -1, nullptr);
		}
	}
	else if(String(property) == "Suite")
	{
		int suiteToFind = tree.getProperty(property);
		if(suiteToFind > 0) theCurrentSuite = Suite::getSuiteWithId(suiteToFind-1);
		else theCurrentSuite = nullptr;
	}
}
