/*
  ==============================================================================

    MidiCore.cpp
    Created: 13 Mar 2013 11:57:22pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#include "MidiCore.h"


MidiCore::MidiCore()
{
    theMidiOutput = nullptr;
}

void MidiCore::openMidiOutput(int index)
{
	if(MidiOutput::getDevices().size() >= index)
		theMidiOutput = MidiOutput::openDevice(index);
}

MidiCore::~MidiCore()
{
	if(theMidiOutput != nullptr) delete theMidiOutput;
}

void MidiCore::noteOn(int noteNumber,int velocity)
{
    const MidiMessage midiMessage(0x90,noteNumber,velocity,0);
    outputMidi(midiMessage);
}

void MidiCore::noteOff(int noteNumber)
{
    const MidiMessage midiMessage(0x80,noteNumber,127,0);
    outputMidi(midiMessage);
}

void MidiCore::killNotes()
{
	for(int i=0;i<NUM_CHANNELS_MAX;i++)
	{
		outputMidi(MidiMessage::allNotesOff(i));
		outputMidi(MidiMessage::allSoundOff(i));
	}
}

void MidiCore::outputMidi(const MidiMessage &msg)
{
	if(theMidiOutput != nullptr)
	{
		theMidiOutput->sendMessageNow(msg);
	}
}



