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

void Sequencer::setPosition()
{
	
}

void Sequencer::handleIncomingMidiMessage (MidiInput* source,
								const MidiMessage& message)
{
	if (message.isMidiClock())
	{
		
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
