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
	AboutView()
	{
		theLogo = ImageFileFormat::loadFrom(BinaryData::IconSmall_png, BinaryData::IconSmall_pngSize);
		addAndMakeVisible(theSlider = new Slider(""));
		theSlider->setSliderStyle(Slider::RotaryVerticalDrag);
		theSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
		theSlider->setTextBoxIsEditable(false);
		theSlider->setScrollWheelEnabled(true);
		theSlider->setColour(Slider::rotarySliderFillColourId, SeqLookAndFeel::getColour(COLOUR_1));
		theSlider->setColour(Slider::rotarySliderOutlineColourId, SeqLookAndFeel::getColour(COLOUR_2));
		theSlider->setDoubleClickReturnValue(true, 0);
		theSlider->setRange(-12, 12);
		theSlider->setValue(3);
		
		addAndMakeVisible(theWebSite = new HyperlinkButton("http://www.nummermusic.com", URL("http://www.nummermusic.com")));
		theWebSite->setFont(Font ("Helvetica Neue",13.0000f, Font::plain), false, Justification::left);
		theWebSite->setColour(HyperlinkButton::textColourId, SeqLookAndFeel::getColour(COLOUR_1));
	}
	
	void paint(Graphics& g)
	{
		g.setFont (Font ("Helvetica Neue",13.0000f, Font::plain));
		g.setColour(SeqLookAndFeel::getColour(COLOUR_1));
		
		float heigthDiv = getHeight() / 16.0f;
		
		g.drawText("Steps V" + String(ProjectInfo::versionString), getWidth() / 3, heigthDiv * 1, getWidth()/3 * 2, heigthDiv * 2, Justification::left, 2);
		g.drawText("Programming: Silvere Letellier", getWidth() / 3, heigthDiv * 3, (getWidth()/3) * 2, heigthDiv * 2, Justification::left, 3);
		g.drawText("With the help of: Danny White,", getWidth()/3, heigthDiv * 4, (getWidth()/3) * 2, heigthDiv * 2, Justification::left, 2);
		g.drawText("Emmnanuel Corre, Julian Salaun,", getWidth()/3, heigthDiv * 5, (getWidth()/3) * 2, heigthDiv * 2, Justification::left, 2);
		g.drawText("Guillaume De Oubeda", getWidth()/3, heigthDiv * 6, (getWidth()/3) * 2, heigthDiv * 2, Justification::left, 2);
		g.drawText("Special thanks to Julian Storer", getWidth()/3, heigthDiv * 8, getWidth() / 2, heigthDiv * 2, Justification::left, 2);
		g.drawText("Copyright 2014 Nummer Music", getWidth()/3, heigthDiv * 10, getWidth()/3 * 2, heigthDiv * 2, Justification::left, 2);
		g.drawText("All rights reserved.", getWidth()/3, heigthDiv * 11, getWidth()/3 * 2, heigthDiv * 2, Justification::left, 2);
		
	}

	void resized()
	{
		float heigthDiv = getHeight() / 16.0f;
		float widthDiv = getWidth() / 32.0f;
		theSlider->setBounds(widthDiv, getHeight() / 2 - (getHeight()/4), getHeight()/2, getHeight()/2);
		theWebSite->setBounds(getWidth() / 3, heigthDiv * 13, widthDiv * 20, heigthDiv * 2);
	}
	

private:
	ScopedPointer<Slider> theSlider;
	Image theLogo;
	ScopedPointer<HyperlinkButton> theWebSite;
};

class AboutWindow: public DocumentWindow
{
public:
	AboutWindow():DocumentWindow("About",SeqLookAndFeel::getColour(SequencerColours::COLOUR_BACKGROUND), DocumentWindow::closeButton)
	{
		setUsingNativeTitleBar(true);
		theAboutView = new AboutView();
		theAboutView->setSize(200, 200);
		setContentOwned(theAboutView, true);
	}
	
	void closeButtonPressed()
	{
		setVisible(false);
	}
	
	~AboutWindow(){}
	
private:
	ScopedPointer<AboutView> theAboutView;
};

#endif  // ABOUTVIEW_H_INCLUDED
