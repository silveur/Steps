/*
  ==============================================================================

    Sequencer.cpp
    Created: 22 Dec 2013 11:52:14pm
    Author:  silvere letellier

  ==============================================================================
*/

#include "Sequencer.h"
#include "PluginProcessor.h"

Sequencer::Sequencer()
{
	theMidiInput = MidiInput::createNewDevice("Sequencer", this);
	theMidiInput->start();
	theMidiCore = new MidiCore();
	thePosition = 0;
	theRootNote = 48;
	ppqCount = 0;
	theSequencerTree = ValueTree("SequencerTree");
	theSequencerTree.setProperty("Position", thePosition, nullptr);
	for (int i=0; i<16; i++)
	{
		theStepArray.add(new Step());
		theSequencerTree.addChild(theStepArray[i]->getValueTree(), -1, nullptr);
	}
	theSequencerTree.addListener(this);
}

Sequencer::~Sequencer()
{
}

void Sequencer::start()
{
	ppqCount = 0;
	thePosition = -1;
}

void Sequencer::stop()
{

}

void Sequencer::setPosition(int beatPosition)
{
}

void Sequencer::handleIncomingMidiMessage (MidiInput* source,
								const MidiMessage& message)
{
	if (message.isMidiClock())
	{
		if (ppqCount == 0)
		{
			thePosition = (thePosition+1)%16;
			
			if (theStepArray[thePosition]->theState)
			{
				Step* step = theStepArray[thePosition];
				MidiMessage onMsg = MidiMessage::noteOn(1, step->thePitch + theRootNote, (uint8)step->theVelocity);
				MidiMessage offMsg = MidiMessage::noteOff(1, step->thePitch + theRootNote, (uint8)step->theVelocity);
				theMidiCore->outputMidi(onMsg);
				theMidiCore->outputMidi(offMsg, 40);
			}
			theSequencerTree.setProperty("Position", thePosition, nullptr);
			DBG("New Position: " << thePosition);
		}
		
		ppqCount = (ppqCount+1) % 6;

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
}

void Sequencer::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
	if(String(property) == "MidiOutput")
	{
		String midiOut = tree.getProperty(property);
		theMidiCore->openMidiOutput(midiOut);
	}
}
