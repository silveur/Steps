/*
  ==============================================================================

    DeleteButton.h
    Created: 27 Jun 2014 12:19:42pm
    Author:  silvere letellier

  ==============================================================================
*/

#ifndef DELETEBUTTON_H_INCLUDED
#define DELETEBUTTON_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class DeleteButton: public TextButton
{
public:
	DeleteButton(const String& buttonName): TextButton(buttonName)
	{
		
	}
	
	~DeleteButton(){}
	
	void paint(Graphics& g)
	{
		float min, radius; float offset = 3.0f;
		float width = getWidth(); float height = getHeight();
		if (width < height ? min = width : min = height);
		radius = min / 2.0f;
		min -= offset;
		
		float ellipseX = (width / 2.0f) - radius + (offset/2.0f);
		float ellipseY = (height / 2.0f) - radius + (offset/2.0f);
		
		g.fillEllipse(ellipseX, ellipseY, min, min);
		g.setColour(Colours::white);
		Path p;

		p.startNewSubPath (ellipseX + (min*0.25f), ellipseY + (min*0.5f));
		p.lineTo (ellipseX + (min*0.75f), ellipseY + (min*0.5f));
		p.closeSubPath();
		g.strokePath(p, PathStrokeType (getHeight()*0.1f));
	}
	
	
};



#endif  // DELETEBUTTON_H_INCLUDED
