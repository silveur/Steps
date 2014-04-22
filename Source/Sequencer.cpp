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
	theTempo = -1;
	thePosition = 0;
	theRootNote = 0;
	count =0;
}

Sequencer::~Sequencer()
{
}

void Sequencer::start()
{

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
		count++;
		if (count == 6)
		{
			thePosition = (thePosition+1)%16;
			DBG("New Position: " << thePosition);
			count = 0;
		}
	}
	else if(message.isSongPositionPointer())
	{
		count =0;
		thePosition = 0;
		int beatPosition = message.getSongPositionPointerMidiBeat();
		setPosition(beatPosition);
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
