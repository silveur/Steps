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
	
	void drawToggleButton (Graphics& g, ToggleButton& button,
										   bool isMouseOverButton, bool isButtonDown)
	{
		float min, radius; float offset = 3.0f;
		float width = button.getWidth(); float height = button.getHeight();
		if (width < height ? min = width : min = height);
		radius = min / 2.0f;
		min -= offset;
		
		float ellipseX = (width / 2.0f) - radius + (offset/2.0f);
		float ellipseY = (height / 2.0f) - radius + (offset/2.0f);
		
		g.fillEllipse(ellipseX, ellipseY, min, min);
		g.setColour(Colours::white);
		Path p;
		if (button.getToggleState())
		{
			p.startNewSubPath (ellipseX + (min/3.0f), ellipseY + (min/2.0f));
			p.lineTo (ellipseX + (min/2.0f), ellipseY + (min*0.7f));
			p.startNewSubPath (ellipseX + (min/2.2f),ellipseY + (min*0.7f));
			p.lineTo (ellipseX + (min*0.7f), ellipseY + (min*0.3f));
			p.closeSubPath();
			g.strokePath(p, PathStrokeType (3.0f));
		}
		else
		{
			p.startNewSubPath (ellipseX + (min/3.0f), ellipseY + (min/3.0f));
			p.lineTo (ellipseX + (min*0.666f), ellipseY + (min*0.666f));
			
			p.startNewSubPath (ellipseX + (min/3.0f), ellipseY + (min*0.666f));
			p.lineTo (ellipseX + (min*0.666f), ellipseY + (min/3.0f));

			p.closeSubPath();
			g.strokePath(p, PathStrokeType (3.0f));
		}
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
	
	void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
											   bool isMouseOverButton, bool isButtonDown)
	{
		Colour baseColour (backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
						   .withMultipliedAlpha (button.isEnabled() ? 0.9f : 0.5f));
		
		if (isButtonDown || isMouseOverButton)
			baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.1f);

		const float width  = button.getWidth() - 1.0f;
		const float height = button.getHeight() - 1.0f;
		
		if (width > 0 && height > 0)
		{
			Path outline;
			outline.addRectangle (0.5f, 0.5f, width, height);
			const float mainBrightness = baseColour.getBrightness();
			const float mainAlpha = baseColour.getFloatAlpha();
			
			g.setGradientFill (ColourGradient (baseColour.brighter (0.2f), 0.0f, 0.0f,
											   baseColour.darker (0.25f), 0.0f, height, false));
			g.fillPath (outline);
			
			g.setColour (Colours::white.withAlpha (0.4f * mainAlpha * mainBrightness * mainBrightness));
			g.strokePath (outline, PathStrokeType (1.0f), AffineTransform::translation (0.0f, 1.0f)
						  .scaled (1.0f, (height - 1.6f) / height));
			
			g.setColour (Colours::black.withAlpha (0.4f * mainAlpha));
			g.strokePath (outline, PathStrokeType (1.0f));
		}
	}
	
	void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                           Slider& slider) override
	{
		bool isSym; int numPositions; int interval;
		
		interval = slider.getInterval();
		numPositions = abs(slider.getMinimum()) + slider.getMaximum();
		if ((slider.getMinimum() + slider.getMaximum()) == 0 ? isSym = true : isSym = false);
		
		float min, radius; float offset = 3.0f;
		float _width = slider.getWidth(); float _height = slider.getHeight();
		if (_width < _height ? min = _width : min = _height);
		radius = min / 2.0f;
		min -= offset;
		
		float ellipseX = (width / 2.0f) - radius + (offset/2.0f);
		float ellipseY = (height / 2.0f) - radius + (offset/2.0f);
		
		g.setColour(Colours::black);
		g.drawEllipse(ellipseX, ellipseY, min, min, 1.0f);
		g.setColour(Colour::fromRGB(83, 17, 21));
		g.fillEllipse(ellipseX + (min*0.05),ellipseY +(min*0.05), min*0.9, min*0.9);
		
//		g.drawRect(ellipseX, ellipseY, min, min);
		
		g.setColour(Colours::white);
		g.drawEllipse(ellipseX +(min*0.2f), ellipseY +(min*0.2f), min*0.6, min*0.6f, 0.4f);
		g.drawEllipse(ellipseX +(min*0.25f), ellipseY +(min*0.25f), min*0.5f, min*0.5f, 0.4f);
					
		const float centreX = x + width * 0.5f;
		const float centreY = y + height * 0.5f;
		const float rx = centreX - radius;
		const float ry = centreY - radius;
		const float rw = radius * 2.0f;
		const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		const float thickness = 0.85f;
		
		{
			g.setColour(getColour(SequencerColours::ColourPurple));
			Path filledArc;
			float begin;
			if (isSym) begin = 6.28319;
			else begin = rotaryStartAngle;
			filledArc.addPieSegment (rx, ry, rw, rw, begin, angle, thickness);
			g.fillPath (filledArc);
		}
//		float xPos = centreX * cosf(angle);
//		float yPos = centreY * sinf(angle);
		
//		g.fillEllipse(centreX + xPos, centreY + yPos, 10, 10);
	}
	
	void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
										   float sliderPos, float minSliderPos, float maxSliderPos,
										   const Slider::SliderStyle style, Slider& slider)
	{
		drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
		drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
	}
	
	void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
													 float /*sliderPos*/,
													 float /*minSliderPos*/,
													 float /*maxSliderPos*/,
													 const Slider::SliderStyle /*style*/, Slider& slider)
	{
		const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
		
		const Colour trackColour (slider.findColour (Slider::trackColourId));
		const Colour gradCol1 (trackColour.overlaidWith (Colour (slider.isEnabled() ? 0x13000000 : 0x09000000)));
		const Colour gradCol2 (trackColour.overlaidWith (Colour (0x06000000)));
		Path indent;
		
		if (slider.isHorizontal())
		{
			const float iy = y + height * 0.5f - sliderRadius * 0.5f;
			
			g.setGradientFill (ColourGradient (gradCol1, 0.0f, iy,
											   gradCol2, 0.0f, iy + sliderRadius, false));
			
			indent.addRoundedRectangle (0, iy, x + width + sliderRadius, sliderRadius, 5.0f);
		}
		else
		{
			const float ix = x + width * 0.5f - sliderRadius * 0.5f;
			
			g.setGradientFill (ColourGradient (gradCol1, ix, 0.0f,
											   gradCol2, ix + sliderRadius, 0.0f, false));
			
			indent.addRoundedRectangle (ix, y - sliderRadius * 0.5f, sliderRadius, height + sliderRadius, 5.0f);
		}
		
		g.fillPath (indent);
		
		g.setColour (trackColour.contrasting (0.5f));
		g.strokePath (indent, PathStrokeType (0.5f));
	}
	
	void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
												float sliderPos, float minSliderPos, float maxSliderPos,
												const Slider::SliderStyle style, Slider& slider)
	{
		const float sliderRadius = (float) (getSliderThumbRadius (slider));
				
		if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
		{
			float kx, ky;
			
			if (style == Slider::LinearVertical)
			{
				kx = x + width * 0.5f;
				ky = sliderPos;
			}
			else
			{
				kx = sliderPos;
				ky = y + height * 0.5f;
			}
			Colour cl = slider.findColour (Slider::thumbColourId);
			g.setColour(cl);
			g.fillEllipse(0.5f + kx - sliderRadius, 1 + ky - sliderRadius, -2.5 +  sliderRadius * 2.0f, -2.5 + sliderRadius * 2.0f);
		}
	}
	
	Font getPopupMenuFont()
	{
		return (Font ("Helvetica neue",15.0000f, Font::plain));
	}
	
	void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
											const bool isSeparator, const bool isActive,
											const bool isHighlighted, const bool isTicked,
											const bool hasSubMenu, const String& text,
											const String& shortcutKeyText,
											const Drawable* icon, const Colour* const textColourToUse)
	{
		if (isSeparator)
		{
			Rectangle<int> r (area.reduced (5, 0));
			r.removeFromTop (r.getHeight() / 2 - 1);
			
			g.setColour (Colour (0x33000000));
			g.fillRect (r.removeFromTop (1));
			
			g.setColour (Colour (0x66ffffff));
			g.fillRect (r.removeFromTop (1));
		}
		else
		{
			Colour textColour (findColour (PopupMenu::textColourId));
			
			if (textColourToUse != nullptr)
				textColour = *textColourToUse;
			
			Rectangle<int> r (area.reduced (1));
			
			if (isHighlighted)
			{
				g.setColour (findColour (PopupMenu::highlightedBackgroundColourId));
				g.fillRect (r);
				
				g.setColour (findColour (PopupMenu::highlightedTextColourId));
			}
			else
			{
				g.setColour (textColour);
			}
			
			if (! isActive)
				g.setOpacity (0.3f);
			
			Font font (getPopupMenuFont());
			
			const float maxFontHeight = area.getHeight() / 1.3f;
			
			if (font.getHeight() > maxFontHeight)
				font.setHeight (maxFontHeight);
			
			g.setFont (font);
			
			Rectangle<float> iconArea (r.removeFromLeft ((r.getHeight() * 3) / 4).reduced (4).toFloat());
			
			if (icon != nullptr)
			{
				icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
			}
			else if (isTicked)
			{
				const Path tick (getTickShape (1.0f));
				float width = (iconArea.getWidth() > iconArea.getHeight() ? iconArea.getHeight() : iconArea.getWidth());
				g.fillEllipse(iconArea.getX(), iconArea.getY() + width /2.0f, width, width);
			}
			
			if (hasSubMenu)
			{
				const float arrowH = 0.6f * getPopupMenuFont().getAscent();
				
				const float x = (float) r.removeFromRight ((int) arrowH).getX();
				const float halfH = (float) r.getCentreY();
				
				Path p;
				p.addTriangle (x, halfH - arrowH * 0.5f,
							   x, halfH + arrowH * 0.5f,
							   x + arrowH * 0.6f, halfH);
				
				g.fillPath (p);
			}
			
			r.removeFromRight (3);
			g.drawFittedText (text, r, Justification::centredLeft, 1);
			
			if (shortcutKeyText.isNotEmpty())
			{
				Font f2 (font);
				f2.setHeight (f2.getHeight() * 0.75f);
				f2.setHorizontalScale (0.95f);
				g.setFont (f2);
				
				g.drawText (shortcutKeyText, r, Justification::centredRight, true);
			}
		}
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
