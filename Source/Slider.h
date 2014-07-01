/* Steps - Midi sequencer>
 * Copyright (C) 2014  Silvere Letellier for Nummer Music
 * Contact: <silvere.letellier@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
