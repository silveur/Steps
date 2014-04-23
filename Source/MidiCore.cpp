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

MidiCore::MidiCore(String& midiOut)
{
	theMidiOutput = nullptr;
	openMidiOutput(midiOut);
}

void MidiCore::openMidiOutput(String& name)
{
	StringArray list = MidiOutput::getDevices();
	for (int i=0;i<list.size();i++)
	{
		if (list[i] == name)
		{
			if (theMidiOutput != nullptr)
				delete theMidiOutput;
			theMidiOutput = MidiOutput::openDevice(i);
			theMidiOutput->startBackgroundThread();
			break;
		}
	}
	if (theMidiOutput == nullptr)
	{
		
	}
}

MidiCore::~MidiCore()
{
	if(theMidiOutput != nullptr) delete theMidiOutput;
}

StringArray MidiCore::getMidiDevicesList()
{
	StringArray list = MidiOutput::getDevices();
	return list;
}

void MidiCore::noteOn(int noteNumber,int velocity)
{
	if (theMidiOutput != nullptr)
	{
		const MidiMessage midiMessage(0x90,noteNumber,velocity,0);
		outputMidi(midiMessage);
	}
}

void MidiCore::noteOff(int noteNumber)
{
	if (theMidiOutput != nullptr)
	{
		const MidiMessage midiMessage(0x80,noteNumber,127,0);
		outputMidi(midiMessage);
	}
}

void MidiCore::killNotes()
{
	if (theMidiOutput != nullptr)
	{
		for(int i=1;i<=16;i++)
		{
			outputMidi(MidiMessage::allNotesOff(i));
			outputMidi(MidiMessage::allSoundOff(i));
		}
	}
}

void MidiCore::outputMidi(const MidiMessage &msg)
{
	if(theMidiOutput != nullptr)
	{
		theMidiOutput->sendMessageNow(msg);
	}
}

void MidiCore::outputMidi(const MidiMessage &msg, int delayMs)
{
	if(theMidiOutput != nullptr)
	{
		MidiBuffer buff(msg);
		theMidiOutput->sendBlockOfMessages(buff, Time::getMillisecondCounter() + delayMs, 44100);
	}
}



