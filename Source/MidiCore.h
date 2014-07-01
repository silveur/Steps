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

#ifndef __MIDICORE_H_8D40ACBD__
#define __MIDICORE_H_8D40ACBD__

#include "../JuceLibraryCode/JuceHeader.h"

class MidiCore
{
public:
	MidiCore();
	~MidiCore();
	
	void killNotes();
	void outputMidi(const MidiMessage& msg);
	void openMidiOutput(String& name);
	void outputMidi(const MidiMessage &msg, int delayMs);
	
private:
	MidiOutput* theMidiOutput;
};

#endif  // __MIDICORE_H_8D40ACBD__
