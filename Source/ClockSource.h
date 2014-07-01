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

#ifndef CLOCKSOURCE_H_INCLUDED
#define CLOCKSOURCE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Master;

class ClockSource: public Thread
{
public:
	ClockSource(Master* master): Thread("ClockGenerator"), theMaster(master), theBPM(-1) {}

	~ClockSource()
	{
		stopThread(200);
	}
	
	void run();
	void send() const;
	
private:
	Master* theMaster;
	MidiMessage msg;
	float theStepTime;
	float theBPM;
	friend class Master;
};

#endif  // CLOCKSOURCE_H_INCLUDED
