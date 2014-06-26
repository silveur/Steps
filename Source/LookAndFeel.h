/*
  ==============================================================================

    LookAndFeel.h
    Created: 28 Apr 2014 9:17:11am
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef LOOKANDFEEL_H_INCLUDED
#define LOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

enum SequencerColours
{
	ColourDarkGrey = 0,
	ColourLightGrey,
	ColourBlueGrey,
	ColourLightBlue,
	ColourRed,
	ColourViolet,
	ColourBlue,
	ColourDarkBlue,
	ColourGreen,
	ColourRedOrange,
	ColourGreenBlue,
	ColourOrange,
	ColourOrangeYellow,
	ColourRichBlue,
	ColourYellow,
	ColourPurple,
	ColourYellowGreen,
};

class SeqLookAndFeel: public LookAndFeel_V3
{
public:
	SeqLookAndFeel()
	{
		
	}
	
	~SeqLookAndFeel()
	{
		
	}
	
	void drawComboBox (Graphics& g, int width, int height, const bool /*isButtonDown*/,
									   int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box)
	{
		g.fillAll (box.findColour (ComboBox::backgroundColourId));
		
		const Colour buttonColour (box.findColour (ComboBox::buttonColourId));
		
		if (box.isEnabled() && box.hasKeyboardFocus (false))
		{
			g.setColour (buttonColour);
			g.drawRect (0, 0, width, height, 2);
		}
		else
		{
			g.setColour (box.findColour (ComboBox::outlineColourId));
			g.drawRect (0, 0, width, height);
		}
		
		const float arrowX = 0.3f;
		const float arrowH = 0.2f;
		
		Path p;
		p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.45f + arrowH),
					   buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.45f,
					   buttonX + buttonW * arrowX,          buttonY + buttonH * 0.45f);
		
		g.setColour (box.findColour (ComboBox::arrowColourId).withMultipliedAlpha (box.isEnabled() ? 1.0f : 0.3f));
		g.fillPath (p);
	}

	void drawDocumentWindowTitleBar (DocumentWindow& window, Graphics& g, int, int, int, int, const Image*, bool)
	{
		g.fillAll(Colours::white);
	}

	void drawBubble(Graphics& g, BubbleComponent& comp, const Point< float > &tip, const Rectangle< float > &body)
	{
		Path p;
		p.addBubble (body.reduced (0.5f), body.getUnion (Rectangle<float> (tip.x, tip.y, 1.0f, 1.0f)),
					 tip, 5.0f, jmin (15.0f, body.getWidth() * 0.2f, body.getHeight() * 0.2f));
		
		g.setColour (Colours::white);
		g.fillPath (p);
	}
	
	void drawButtonText (Graphics& g, TextButton& button, bool /*isMouseOverButton*/, bool /*isButtonDown*/)
	{
		Font font (getTextButtonFont (button));
		g.setFont (font);
		g.setColour (button.findColour (button.getToggleState() ? TextButton::textColourOnId
										: TextButton::textColourOffId)
					 .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));
		
		const int yIndent = jmin (4, button.proportionOfHeight (0.3f));
		const int cornerSize = jmin (button.getHeight(), button.getWidth()) / 2;
		
		const int fontHeight = roundToInt (font.getHeight() * 0.6f);
		const int leftIndent  = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
		const int rightIndent = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
		
		g.drawFittedText (button.getButtonText(),
						  leftIndent,
						  yIndent,
						  button.getWidth() - leftIndent - rightIndent,
						  button.getHeight() - yIndent * 2,
						  Justification::centred, 2);
	}
	
	void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                           Slider& slider) override
	{
		bool isSym; int numPositions; int interval;
		
		interval = slider.getInterval();
		numPositions = abs(slider.getMinimum()) + slider.getMaximum();
		if ((slider.getMinimum() + slider.getMaximum()) == 0 ? isSym = true : isSym = false);
		float sliderHeight = slider.getHeight();
		float sliderWidth = slider.getWidth();
		
		g.setColour(Colours::black);
		g.drawEllipse(x, y, width, height, 0.4);
		g.setColour(Colour::fromRGB(83, 17, 21));
		g.fillEllipse(slider.getWidth()*0.05, slider.getHeight()*0.05, slider.getWidth()*0.9, slider.getHeight()*0.9);
		
		g.setColour(Colours::white);
		g.drawEllipse(slider.getWidth()*0.2, slider.getHeight()*0.2, slider.getWidth()*0.6, slider.getHeight()*0.6f, 0.4f);
		g.drawEllipse(slider.getWidth()*0.22, slider.getHeight()*0.22, slider.getWidth()*0.56f, slider.getHeight()*0.56f, 0.4f);
					
		const float radius = jmin (width / 2, height / 2) - 5.0f;
		const float centreX = x + width * 0.5f;
		const float centreY = y + height * 0.5f;
		const float rx = centreX - radius;
		const float ry = centreY - radius;
		const float rw = radius * 2.0f;
		const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
		
//			if (slider.isEnabled())
//				g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 0.7f : 1.0f));
//			else
//				g.setColour (Colour (0x80808080));
		
		const float thickness = 0.7f;
		
		{
			g.setColour(getColour(SequencerColours::ColourBlueGrey));
			Path filledArc;
			float begin;
			if (isSym) begin = 6.28319;
			else begin = rotaryStartAngle;
			filledArc.addPieSegment (rx, ry, rw, rw, begin, angle, thickness);
			g.fillPath (filledArc);
		}
	}
	
	void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
										   float sliderPos, float minSliderPos, float maxSliderPos,
										   const Slider::SliderStyle style, Slider& slider)
	{
		g.fillAll (slider.findColour (Slider::backgroundColourId));
		const float fx = (float) x, fy = (float) y, fw = (float) width, fh = (float) height;
		Path p;
		
		if (style == Slider::LinearBarVertical)
			p.addRectangle (fx, sliderPos, fw, 1.0f + fh - sliderPos);
		else
			p.addRectangle (fx, fy, sliderPos - fx, fh);
		
		Colour baseColour (slider.findColour (Slider::thumbColourId)
						   .withMultipliedSaturation (slider.isEnabled() ? 1.0f : 0.5f)
						   .withMultipliedAlpha (0.8f));
		
		g.setGradientFill (ColourGradient (baseColour.brighter (0.08f), 0.0f, 0.0f,
										   baseColour.darker (0.08f), 0.0f, (float) height, false));
		g.fillPath (p);
		
		g.setColour (baseColour.darker (0.2f));
		
		if (style == Slider::LinearBarVertical)
			g.fillRect (fx, sliderPos, fw, 1.0f);
		else
			g.fillRect (sliderPos, fy, 1.0f, fh);
		
		g.drawRect(slider.getLocalBounds());
	}
	
	static Colour getColour(const SequencerColours colour, float alpha=1.0f)
	{
		uint8 intAlpha = (int) (alpha * 255.0f);
		switch (colour)
		{
			case ColourDarkGrey: return Colour::fromRGBA(47, 46, 47, intAlpha);
			case ColourLightGrey: return Colour::fromRGBA(209, 211, 211, intAlpha);
			case ColourBlueGrey: return Colour::fromRGBA(81, 117, 139, intAlpha);
			case ColourLightBlue: return Colour::fromRGBA(156, 183, 211, intAlpha);
			case ColourRed: return Colour::fromRGBA(230, 63, 82, intAlpha);
			case ColourViolet: return Colour::fromRGBA(180, 87, 161, intAlpha);
			case ColourBlue: return Colour::fromRGBA(57, 127, 186, intAlpha);
			case ColourDarkBlue: return Colour::fromRGBA(30, 40, 40, intAlpha);
			case ColourGreen: return Colour::fromRGBA(101, 180, 72, intAlpha);
			case ColourRedOrange: return Colour::fromRGBA(228, 118, 72, intAlpha);
			case ColourGreenBlue: return Colour::fromRGBA(63, 180, 165, intAlpha);
			case ColourOrange: return Colour::fromRGBA(227, 157, 69, intAlpha);
			case ColourOrangeYellow: return Colour::fromRGBA(225, 186, 64, intAlpha);
			case ColourRichBlue: return Colour::fromRGBA(38, 84, 165, intAlpha);
			case ColourYellow: return Colour::fromRGBA(227, 215, 67, intAlpha);
			case ColourPurple: return Colour::fromRGBA(101, 86, 165, intAlpha);
			case ColourYellowGreen: return Colour::fromRGBA(186, 203, 64, intAlpha);
			default: return Colour();
		}
	}
};



#endif  // LOOKANDFEEL_H_INCLUDED
