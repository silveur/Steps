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
		theStepImage = ImageFileFormat::loadFrom(BinaryData::button_minus_png, BinaryData::button_minus_pngSize);
	}
	~StepView()
	{
	}
	
	void update(int x)
	{
		X = x;
		repaint();
	}
	
	void paint(Graphics& g)
	{
		g.drawImageAt(theStepImage, X, 0);
	}
	
private:
	Image theStepImage;
	int X;
};



#endif  // STEPSVIEW_H_INCLUDED
