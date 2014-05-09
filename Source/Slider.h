/*
  ==============================================================================

    Slider.h
    Created: 8 May 2014 8:53:24pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SeqSlider: public Slider
{
public:
	SeqSlider(const String& name): Slider(name)
	{
		theCurrentBubbleMessage = new BubbleMessageComponent();
		theMessage = String(getValue());
	}
	
	~SeqSlider() {}
	
	void startedDragging()
	{
		showBubbleMessage(this, theMessage);
	}

	void valueChanged()
	{
		if (getThumbBeingDragged() != -1)
			showBubbleMessage(this, theMessage);
	}
	
	void showBubbleMessage(Component *targetComponent, const String &textToShow)
	{
		if (Desktop::canUseSemiTransparentWindows())
		{
			theCurrentBubbleMessage->setAlwaysOnTop (true);
			theCurrentBubbleMessage->addToDesktop (0);
		}
		else
		{
			targetComponent->getTopLevelComponent()->addChildComponent (theCurrentBubbleMessage);
		}
		AttributedString text(textToShow);
		text.setJustification(Justification::centred);
		theCurrentBubbleMessage->showAt(targetComponent, text, 300, true, false);
	}
	
	void setMessage(String& msg)
	{
		theMessage = msg;
	}
	
private:
	ScopedPointer<BubbleMessageComponent> theCurrentBubbleMessage;
	String theMessage;
};



#endif  // SLIDER_H_INCLUDED
