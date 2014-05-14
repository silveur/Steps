/*
  ==============================================================================

    ClockSource.cpp
    Created: 14 May 2014 11:27:14am
    Author:  Silvere Letellier

  ==============================================================================
*/

#include "ClockSource.h"
#include "Master.h"

void ClockSource::run()
{
	DBG("Starting Thread");
	while(!threadShouldExit())
	{
		MidiMessage start = MidiMessage::midiStart();
		theMaster->handleIncomingMidiMessage (nullptr, start);
		if (theBPM != -1)
		{
			
			
		}
		sleep(20);
	}
	DBG("Stopping Thread");
}
