/*
  ==============================================================================

    StepsView.h
    Created: 24 Apr 2014 10:50:15am
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef STEPSVIEW_H_INCLUDED
#define STEPSVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

struct FollowComponent  : public Component
{
    FollowComponent()
    {
		colour = Colours::green;
        setSize (20, 20);
    }
    void paint (Graphics& g)
    {
        g.setColour (colour);
        g.fillEllipse (2.0f, 2.0f, getWidth() - 4.0f, getHeight() - 4.0f);
    }

    Colour colour;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FollowComponent)
};

class StepView: public Component, public Thread
{
public:
	StepView(): Thread("StepView"), X(0)
	{
		addAndMakeVisible(theFollow = new FollowComponent());
		startThread(0);
	}
	
	~StepView() { delete theFollow; stopThread(500); }
	
	void resized()
	{
		theFollow->setVisible(false);
	}
	
	void run()
	{
		while(!threadShouldExit())
		{
			{
				const MessageManagerLock mmLock;
				theFollow->setCentrePosition(X, 10);
			}
			wait(1000);
		}
	}
	
	void update(int x)
	{
		if (x != -1)
		{
			X = x + 10;
			theFollow->setVisible(true);
			notify();
		}
		else
		{
			theFollow->setVisible(false);
		}
	}

private:
	FollowComponent* theFollow;
	MessageManagerLock lock;
	int X;
};

#endif  // STEPSVIEW_H_INCLUDED
