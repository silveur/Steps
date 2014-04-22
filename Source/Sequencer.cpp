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
	theRootNote = 0;
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
		ppqCount++;
		if (ppqCount == 6)
		{
			thePosition = (thePosition+1)%16;
			theSequencerTree.setProperty("Position", thePosition, nullptr);
			DBG("New Position: " << thePosition);
			ppqCount = 0;
		}
	}
	else if(message.isSongPositionPointer())
	{
		ppqCount =0;
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

void Sequencer::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
	if(String(property) == "MidiOutput")
	{
		String midiOut = tree.getProperty(property);
		theMidiCore->openMidiOutput(midiOut);
	}
}
