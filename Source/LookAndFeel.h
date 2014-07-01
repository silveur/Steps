/* =====================================================================
 
 * Steps - Midi sequencer>
 * Copyright (C) 2014  Silvere Letellier for Nummer Music
 * Contact: <silvere.letellier@gmail.com>
 
 -----------------------------------------------------------------------
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ===================================================================== */

#ifndef LOOKANDFEEL_H_INCLUDED
#define LOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

enum SequencerColours
{
	COLOUR_1 = 0,
	COLOUR_2,
	COLOUR_3,
	COLOUR_4,
	COLOUR_5,
	COLOUR_BACKGROUND
};

enum ColourTheme
{
	DARK,
	BRIGHT,
	WINE,
	ETSI
};

extern ColourTheme theColourTheme;
extern Colour textButtonTextColour;

class SeqLookAndFeel: public LookAndFeel_V3
{
public:
	SeqLookAndFeel() {}
	
	~SeqLookAndFeel() {}
	
	void drawToggleButton (Graphics& g, ToggleButton& button,
										   bool isMouseOverButton, bool isButtonDown)
	{
		Colour back = button.findColour (button.getToggleState() ? TextButton::textColourOnId
										 : TextButton::textColourOffId);
		if (button.getToggleState())
			back = getColour(SequencerColours::COLOUR_3);
		else
			back = getColour(SequencerColours::COLOUR_4);
		Colour baseColour (back.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
						   .withMultipliedAlpha (button.isEnabled() ? 0.9f : 0.5f));
		
		if (button.isMouseOver())
			baseColour = baseColour.contrasting (0.1f);
		
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
		
		Colour textColour = button.findColour (ToggleButton::textColourId);
		g.setColour(textColour);
		String text(button.getName().getCharPointer(), 1);
		g.drawFittedText(text, 0, 0, button.getWidth(), button.getHeight(), Justification::centred, 1);
	}
	
	void drawComboBox (Graphics& g, int _width, int _height, const bool /*isButtonDown*/,
									   int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box)
	{
		setColour (PopupMenu::backgroundColourId, SeqLookAndFeel::getColour(SequencerColours::COLOUR_4).darker(0.25f).withMultipliedAlpha(0.95f));
		setColour (PopupMenu::textColourId, textButtonTextColour);
		
		Colour baseColour (box.findColour (ComboBox::backgroundColourId).withMultipliedSaturation (box.hasKeyboardFocus (true) ? 1.3f : 0.9f)
						   .withMultipliedAlpha (box.isEnabled() ? 0.9f : 0.5f));
		
		if (box.isMouseOver())
			baseColour = baseColour.contrasting (0.1f);
		
		const float width  = box.getWidth() - 1.0f;
		const float height = box.getHeight() - 1.0f;
		
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
		
		g.setColour (getColour(COLOUR_4));
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
		radius = min / 2.1f;
		min -= offset;
		const float centreX = x + width * 0.5f;
		const float centreY = y + height * 0.5f;
		const float rx = centreX - radius;
		const float ry = centreY - radius;
		const float rw = radius * 2.0f;
		const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		if (radius > 12.0f)
		{
			const float thickness = 0.6f;

			if (slider.isEnabled())
				g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));
			else
				g.setColour (Colour (0x80808080));
			Path outlineArc;
			outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
			outlineArc.closeSubPath();
			g.fillPath(outlineArc);
			
			if (slider.isEnabled())
				g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (slider.isMouseOver() ? 0.7f : 1.0f));
			else
				g.setColour (Colour (0x80808080));
			
			Path filledArc;
			filledArc.addPieSegment (rx, ry, rw, rw, 6.28319f, angle, thickness);
			g.fillPath (filledArc);

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
		}
		else
		{
			if (slider.isEnabled())
				g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (slider.isMouseOver() ? 0.7f : 1.0f));
			else
				g.setColour (Colour (0x80808080));
			
			Path p;
			p.addEllipse (-0.2f * rw, -0.2f * rw, rw * 0.4f, rw * 0.4f);
			PathStrokeType (rw * 0.1f).createStrokedPath (p, p);
			
			p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
			PathStrokeType (rw * 0.1f).createStrokedPath (p, p);
			
			g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));
			
			Path p2;
			if (slider.isEnabled())
				g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));
			else
				g.setColour (Colour (0x80808080));
			p2.addLineSegment (Line<float> (0.0f, 0.0f, 0.0f, -radius * 0.9f), rw * 0.1f);
			g.fillPath (p2, AffineTransform::rotation (angle).translated (centreX, centreY));
		}
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
		const float sliderRadius = (float) (getSliderThumbRadius (slider)) * 0.9f;
				
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
			g.fillEllipse(kx - sliderRadius, 1 + ky - sliderRadius, -2.5 +  sliderRadius * 2.0f, -2.5 + sliderRadius * 2.0f);
		}
	}
	
	Font getPopupMenuFont()
	{
		return (Font ("Helvetica Neue",12.0000f, Font::plain));
	}
	
	Font getTextButtonFont (TextButton &)
	{
		return (Font ("Helvetica Neue",12.0000f, Font::plain));
	}
	
	Font getComboboxFont (ComboBox &)
	{
		return (Font ("Helvetica Neue",12.0000f, Font::plain));
	}
	
	Font getLabelFont (Label &)
	{
		return (Font ("Helvetica Neue",12.0000f, Font::plain));
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
		if (theColourTheme == DARK)
		{
			switch (colour)
			{
				case COLOUR_1: return Colour::fromRGBA(27, 29, 38, intAlpha);
				case COLOUR_2: return Colour::fromRGBA(66, 89, 85, intAlpha);
				case COLOUR_3: return Colour::fromRGBA(119, 140, 122, intAlpha);
				case COLOUR_4: return Colour::fromRGBA(241, 242, 216, intAlpha);
				case COLOUR_5: return Colour::fromRGBA(191, 189, 159, intAlpha);
				case COLOUR_BACKGROUND: return Colour::fromRGBA(191, 189, 159, intAlpha);
				default: return Colour();
			}
		}
		else if (theColourTheme == BRIGHT)
		{
			switch (colour)
			{
				case COLOUR_1: return Colour::fromRGBA(51, 77, 92, intAlpha);
				case COLOUR_2: return Colour::fromRGBA(69, 178, 157, intAlpha);
				case COLOUR_3: return Colour::fromRGBA(239, 201, 76, intAlpha);
				case COLOUR_4: return Colour::fromRGBA(226, 122, 63, intAlpha);
				case COLOUR_5: return Colour::fromRGBA(223, 73, 73, intAlpha);
				case COLOUR_BACKGROUND: return Colours::white;
				default: return Colour();
			}
		}
		else if (theColourTheme == WINE)
		{
			switch (colour)
			{
				
				case COLOUR_1: return Colour::fromRGBA(47, 52, 59, intAlpha);
				case COLOUR_2: return Colour::fromRGBA(112, 48, 48, intAlpha);
				case COLOUR_3: return Colour::fromRGBA(126, 130, 122, intAlpha);
				case COLOUR_4: return Colour::fromRGBA(227, 205, 164, intAlpha);
				case COLOUR_5: return Colour::fromRGBA(199, 121, 102, intAlpha);
				case COLOUR_BACKGROUND: return Colour::fromRGBA(199, 121, 102, intAlpha);
				default: return Colour();
			}
		}
		else if (theColourTheme == ETSI)
		{
			switch (colour)
			{
					
				case COLOUR_2: return Colour::fromRGBA(173, 154, 39, intAlpha);
				case COLOUR_1: return Colour::fromRGBA(48, 67, 69, intAlpha);
				case COLOUR_3: return Colour::fromRGBA(120, 154, 161, intAlpha);
				case COLOUR_4: return Colour::fromRGBA(160, 213, 214, intAlpha);
				case COLOUR_5: return Colour::fromRGBA(220, 235, 221, intAlpha);
				case COLOUR_BACKGROUND: return Colour::fromRGBA(220, 235, 221, intAlpha);
				default: return Colour();
			}
		}

		else
			return Colour();
	}
	
};



#endif  // LOOKANDFEEL_H_INCLUDED
