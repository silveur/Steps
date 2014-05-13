/*
  ==============================================================================

    AboutView.h
    Created: 13 May 2014 11:19:49pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef ABOUTVIEW_H_INCLUDED
#define ABOUTVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ControllerView.h"
class AboutView: public Component
{
public:
	AboutView(ControllerView* view): theControllerView(view)
	{
		text.setText(String(" From classic four-to-the floor to outer-space arrangements, Nummer layout raw drum work and emotive atmospheres with finesse. After forming in 2010, the London-based duo hit record store catalogues for the first time in 2014 with their debut EP Beyond Time Interpretations on Peur Bleue records. "), NotificationType::dontSendNotification);
		text.setJustificationType(Justification::centred);
		addAndMakeVisible(&text);
		text.addMouseListener(this, true);
	}
	
	void paint(Graphics& g)
	{
//		g.setFont(12);
//		g.drawText(, getWidth()/8, getHeight()/4, getWidth()/1.5, getHeight()/2, Justification::centred, true);
	}
	void resized()
	
	{
		text.setBounds(getBounds());
	}
	void mouseDown (const MouseEvent &event)
	{
		theControllerView->aboutViewClicked();
	}

private:
	Label text;
	ControllerView* theControllerView;
};



#endif  // ABOUTVIEW_H_INCLUDED
