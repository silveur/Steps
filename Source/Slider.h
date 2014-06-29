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
#include "SequencerView.h"

class SeqSlider: public Slider, SliderListener
{
public:
	SeqSlider(const String& name, SequencerView* seqView): Slider(name), theSequencerView(seqView), isBeingDragged(false)
	{
		theCurrentBubbleMessage = new BubbleMessageComponent();
		theMessage = String(getValue());
		addListener(this);
	}
	
	~SeqSlider() {}
	
	void startedDragging()
	{
		showBubbleMessage(this, theMessage);
	}
	
	void mouseUp(const MouseEvent& event) override
	{
		isBeingDragged = false;
	}
	
	void sliderDragStarted (Slider*)
	{
		isBeingDragged = true;
	}

	void sliderValueChanged (Slider* slider) {}

	void valueChanged()
	{
		if (isBeingDragged)
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
		theCurrentBubbleMessage->showAt(targetComponent, text, 800, true, false);
	}
	
	void setMessage(String& msg)
	{
		theMessage = msg;
	}
	
private:
	ScopedPointer<BubbleMessageComponent> theCurrentBubbleMessage;
	SequencerView* theSequencerView;
	String theMessage;
	bool isBeingDragged;
};



#endif  // SLIDER_H_INCLUDED
