/*
  ==============================================================================

    ClockGenerator.h
    Created: 14 May 2014 10:54:04am
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef CLOCKGENERATOR_H_INCLUDED
#define CLOCKGENERATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ClockGenerator: public Thread
{
public:
	ClockGenerator(): Thread("ClockGenerator"), theBPM(-1)
	{}
	
	~ClockGenerator()
	{
		
	}
	
	void run()
	{
		while(!threadShouldExit())
		{
			if (theBPM != -1)
			{
				
				
			}
			sleep(20);
		}
	}
	
	void setBPM(float bpm)
	{
		theBPM = bpm;
		
		
	}
	
private:
	float theBPM;
	
};

#endif  // CLOCKGENERATOR_H_INCLUDED
