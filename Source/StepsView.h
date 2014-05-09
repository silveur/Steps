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

class StepView: public Component
{
public:
	StepView()
	{
		thePosition = 0;
		setVisible(true);
	}
	~StepView() {}
	
	void update(int x)
	{
		thePosition = x;
		repaint();
	}
	void paint(Graphics& g)
	{
		if (thePosition != -1)
			g.drawEllipse(thePosition, 0, 20, 20, 2);
	}
	
private:
	int thePosition;
};



#endif  // STEPSVIEW_H_INCLUDED
