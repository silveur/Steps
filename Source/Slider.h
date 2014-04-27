/*
  ==============================================================================

    Slider.h
    Created: 27 Apr 2014 8:15:39pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Scales.h"

class SeqSlider: public Slider
{
public:
	SeqSlider(const String& name): Slider(name)
	{
		theLastValue = 0;
	}
	
	~SeqSlider()
	{}
	
	void registerScale(Scale* scaleToUse)
	{
		theScale = scaleToUse;
	}
	
	void clearScale()
	{
		theScale = nullptr;
	}
	
//	void valueChanged()
//	{
//		if (theScale != nullptr)
//		{
//
//			for(int i=0;i<theScale->getNotes().size();i++)
//			{
//				if (getValue() == theScale->getNotes()[i])
//				{
//					DBG("Scale note at index:" << i);
//					DBG("Slider value:" << getValue());
//					setTextValueSuffix("Major");
//					return;
//				}
//			}
//		}
//		setTextValueSuffix("");
//		
////		DBG("Value:" << getValue());
////		int attemptedValue = getValue();
////		int result;
////		if (theScale == nullptr)
////		{
////			theLastValue = attemptedValue;
////		}
////		else
////		{
////			//			DBG("Previous value:" << theLastValue);
////			if (attemptedValue > theLastValue)
////			{
////				result = snapUp(attemptedValue);
////				DBG("Snaped to:" << result);
////			}
////			else if( attemptedValue < theLastValue)
////			{
////				result = snapDown(attemptedValue);
////			}
////			else
////			{
////				result = attemptedValue;
////			}
////			theLastValue = result;
////			//			DBG("New Value:" << theLastValue);
////			setValue(result);
////		}
//	}
//	
//	double snapValue(double attemptedValue, Slider::DragMode dragMode)
//	{
////		attemptedValue = (int)attemptedValue;
////		int result;
////		if (theScale == nullptr)
////		{
////			theLastValue = attemptedValue;
////			return (int)attemptedValue;
////		}
////		else
////		{
//////			DBG("Previous value:" << theLastValue);
////			if (attemptedValue > theLastValue)
////			{
////				result = snapUp(attemptedValue);
////				DBG("Snaped to:" << result);
////			}
////			else if( attemptedValue < theLastValue)
////			{
////				result = snapDown(attemptedValue);
////			}
////			else
////			{
////				result = attemptedValue;
////			}
////			theLastValue = result;
//////			DBG("New Value:" << theLastValue);
////			setValue(result);
////			return (int)result;
////		}
//		return attemptedValue;
//	}
	
private:
	Scale* theScale;
	double theLastValue;
};



#endif  // SLIDER_H_INCLUDED
