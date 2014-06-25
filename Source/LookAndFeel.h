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
		const float radius = jmin (width / 2, height / 2) - 2.0f;
		const float centreX = x + width * 0.5f;
		const float centreY = y + height * 0.5f;
		const float rx = centreX - radius;
		const float ry = centreY - radius;
		const float rw = radius * 2.0f;
		const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
		
		bool isSym;
		if ((slider.getMinimum() + slider.getMaximum()) == 0)
		{
			isSym = true;
		}
		else isSym = false;
		
		if (slider.isEnabled())
			g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 0.7f : 1.0f));
		else
			g.setColour (Colour (0x80808080));
		
		const float thickness = 0.7f;
		
		{
			Path filledArc;
			float begin;
			if (isSym) begin = 6.28319;
			else begin = rotaryStartAngle;
			filledArc.addPieSegment (rx, ry, rw, rw, begin, angle, thickness);
			g.fillPath (filledArc);
		}
		
		if (thickness > 0)
		{
			const float innerRadius = radius * 0.2f;
			Path p;
			p.addTriangle (-innerRadius, 0.0f,
						   0.0f, -radius * thickness * 1.1f,
						   innerRadius, 0.0f);
			
			p.addEllipse (-innerRadius, -innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
			
			g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));
		}
		
		if (slider.isEnabled())
			g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));
		else
			g.setColour (Colour (0x80808080));
		
		Path outlineArc;
		outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
		outlineArc.closeSubPath();
		
		g.strokePath (outlineArc, PathStrokeType (slider.isEnabled() ? (isMouseOver ? 2.0f : 1.2f) : 0.3f));
		
		g.setColour(Colours::black);
		g.drawRect(x, y, width, height);
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

private:
	
};



#endif  // LOOKANDFEEL_H_INCLUDED
