/*
  ==============================================================================

    ComboBox.h
    Created: 27 Jun 2014 3:07:39pm
    Author:  silvere letellier

  ==============================================================================
*/

#ifndef COMBOBOX_H_INCLUDED
#define COMBOBOX_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "LookAndFeel.h"

class SeqComboBox: public ComboBox
{
public:
	SeqComboBox(const String& componentName): ComboBox(componentName)
	{
		
	}
	
	~SeqComboBox() {}
	
//	void paint(Graphics &g)
//	{
//		ComboBox::paint(g);
//		float rounding = 0.4f;
//		float lineThickness = 1.0f;
//		g.setColour(SeqLookAndFeel::getColour(ColourBlue));
//		g.drawRoundedRectangle(lineThickness, lineThickness, getWidth()-2*lineThickness, getHeight()-2*lineThickness, 1.5*rounding, lineThickness);
//	}
	
private:
	bool active;
	bool buttonMode;
};



#endif  // COMBOBOX_H_INCLUDED
