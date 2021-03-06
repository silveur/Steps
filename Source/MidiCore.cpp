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

#include "MidiCore.h"

MidiCore::MidiCore()
{
	theMidiOutput = nullptr;
}

void MidiCore::openMidiOutput(String& name)
{
	StringArray list = MidiOutput::getDevices();
	for (int i=0;i<list.size();i++)
	{
		if (list[i] == name)
		{
			killNotes();
			if (theMidiOutput != nullptr)
				delete theMidiOutput;
			theMidiOutput = MidiOutput::openDevice(i);
			theMidiOutput->startBackgroundThread();
			break;
		}
	}
}

MidiCore::~MidiCore()
{
	if(theMidiOutput != nullptr)
	{
		killNotes();
		theMidiOutput->stopBackgroundThread();
		delete theMidiOutput;
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

void MidiCore::outputMidi(const MidiMessage &msg, int delayMs)
{
	if(theMidiOutput != nullptr)
	{
		MidiBuffer buff(msg);
		theMidiOutput->sendBlockOfMessages(buff, Time::getMillisecondCounter() + delayMs, 44100);
	}
}

void MidiCore::outputMidi(const Array<MidiMessage> &msgs, int delayMs)
{
	if(theMidiOutput != nullptr)
	{
		for (int i=0;i<msgs.size();i++)
		{
			MidiBuffer buff(msgs[i]);
			theMidiOutput->sendBlockOfMessages(buff, Time::getMillisecondCounter() + delayMs, 44100);
		}
	}
}
