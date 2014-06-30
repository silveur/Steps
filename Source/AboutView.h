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
		theWebSite->setFont(Font ("Helvetica Neue",14.0000f, Font::plain), false, Justification::left);
		theWebSite->setColour(HyperlinkButton::textColourId, SeqLookAndFeel::getColour(COLOUR_1));
	}
	
	void paint(Graphics& g)
	{
		g.setFont (Font ("Helvetica Neue",14.0000f, Font::plain));
		g.setColour(SeqLookAndFeel::getColour(COLOUR_1));
		
		float heigthDiv = getHeight() / 16.0f;
		float widthDiv = getWidth() / 32.0f;
		
		g.drawFittedText("Steps V" + String(ProjectInfo::versionString), widthDiv * 14, heigthDiv * 4, widthDiv * 10, heigthDiv * 2, Justification::left, 1);
		g.drawFittedText("Original idea & programming: Silvere Letellier", widthDiv * 14, heigthDiv * 5, widthDiv * 10, heigthDiv * 2, Justification::left, 1);
		g.drawFittedText("With the help of: Danny White, Emmnanuel Corre, Guillaume De Oubeda, Julian Salaun", widthDiv * 14, heigthDiv * 6, widthDiv * 20, heigthDiv * 2, Justification::left, 2);
		g.drawFittedText("Special thanks to Julian Storer", widthDiv * 14, heigthDiv * 8, widthDiv * 20, heigthDiv * 2, Justification::left, 2);
	}

	void mouseDown (const MouseEvent &event)
	{
		theControllerView->aboutViewClicked();
	}
	
	void resized()
	{
		float heigthDiv = getHeight() / 16.0f;
		float widthDiv = getWidth() / 32.0f;
		theSlider->setBounds(widthDiv * 10, heigthDiv * 3, widthDiv * 3, widthDiv * 3);
		theWebSite->setBounds(widthDiv * 14, heigthDiv * 10, widthDiv * 20, heigthDiv * 2);
	}
	

private:
	ControllerView* theControllerView;
	ScopedPointer<Slider> theSlider;
	Image theLogo;
	ScopedPointer<HyperlinkButton> theWebSite;
};

#endif  // ABOUTVIEW_H_INCLUDED
