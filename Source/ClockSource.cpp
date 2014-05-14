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
