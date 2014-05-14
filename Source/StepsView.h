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
		setInterceptsMouseClicks(false, true);
    }
    void paint (Graphics& g)
    {
        g.setColour (colour);
        g.fillEllipse (2.0f, 2.0f, getWidth() - 4.0f, getHeight() - 4.0f);
    }

    Colour colour;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FollowComponent)
};

class StepView: public Component, public Timer
{
public:
	StepView(): X(0)
	{
		addAndMakeVisible(theFollow = new FollowComponent());
		setInterceptsMouseClicks(false, true);
		X = 25;
	}
	
	~StepView() {}
	
	void resized()
	{
		theFollow->setBounds(X, 0, 20, 20);
	}
	
	void timerCallback()
	{
		theFollow->setCentrePosition(X, 10);
		stopTimer();
	}
	
	void update(int x)
	{
		if (x != -1)
		{
			X = x + 10;
			theFollow->setVisible(true);
		}
		else
		{
			theFollow->setVisible(false);
		}
		startTimer(2);
	}

private:
	ScopedPointer<FollowComponent> theFollow;
	MessageManagerLock lock;
	int X;
};

#endif  // STEPSVIEW_H_INCLUDED
