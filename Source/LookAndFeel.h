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

		
		if (slider.isEnabled())
			g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 0.7f : 1.0f));
		else
			g.setColour (Colour (0x80808080));
		
		const float thickness = 0.7f;
		
		{
			Path filledArc;
			filledArc.addPieSegment (rx, ry, rw, rw, 3.76991, 8.79646, thickness);
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
		
	}

private:
	
};



#endif  // LOOKANDFEEL_H_INCLUDED
