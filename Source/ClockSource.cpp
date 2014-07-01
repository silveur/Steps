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

#include "ClockSource.h"
#include "Master.h"

void ClockSource::run()
{
	msg = MidiMessage::midiStart();
	send();
	while(!threadShouldExit())
	{
		msg = MidiMessage::midiClock();
		send();
		theStepTime = 1.0 / (theBPM / 15.0f);
		wait(1000 * (theStepTime/6.0f));
	}
	msg = MidiMessage::midiStop();
	send();
}

void ClockSource::send() const
{
	theMaster->handleIncomingMidiMessage(nullptr, msg);
}
