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
		setInterceptsMouseClicks(false, true);
    }
    void paint (Graphics& g)
    {
        g.setColour (colour);
        g.fillEllipse (.0f, .0f, getWidth(), getHeight());
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
		theFollow->setBounds(X, getHeight()*0.1, getHeight()*0.8, getHeight()*0.8);
	}
	
	void timerCallback()
	{
		theFollow->setCentrePosition(X, (theFollow->getHeight() * 1.4)/2);
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
	int X;
};

#endif  // STEPSVIEW_H_INCLUDED
