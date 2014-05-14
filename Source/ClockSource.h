/*
  ==============================================================================

    ClockSource.h
    Created: 14 May 2014 11:27:14am
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef CLOCKSOURCE_H_INCLUDED
#define CLOCKSOURCE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Master;

class ClockSource: public Thread
{
public:
	ClockSource(Master* master): Thread("ClockGenerator"), theMaster(master), theBPM(-1)
	{
	}
	
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
