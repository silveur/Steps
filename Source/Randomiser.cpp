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

#include "Randomiser.h"
#include "LookAndFeel.h"

extern File thePresetFolder;
extern File theDefaultPreset;
extern Colour textButtonTextColour;

Randomiser::Randomiser(SequencerView* sequencerView, ValueTree& sequencerTree): theSequencerView(sequencerView), theSequencerTree(sequencerTree)
{
	addAndMakeVisible(theRandomAllButton = new TextButton("Randomise"));
	theRandomAllButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(COLOUR_4));
	theRandomAllButton->setColour(TextButton::textColourOffId, textButtonTextColour);
	theRandomAllButton->addListener(this);
	
	addAndMakeVisible(theVelocityButton = new ToggleButton("Velocity"));
	theVelocityButton->setToggleState(theSequencerTree.getProperty("RandVelocity"), dontSendNotification);
	theVelocityButton->setColour(ToggleButton::textColourId, textButtonTextColour);
	theVelocityButton->addListener(this);
	
	addAndMakeVisible(theDecayButton = new ToggleButton("Decay"));
	theDecayButton->setToggleState(theSequencerTree.getProperty("RandDecay"), dontSendNotification);
	theDecayButton->setColour(ToggleButton::textColourId, textButtonTextColour);
	theDecayButton->addListener(this);
	
	addAndMakeVisible(thePitchButton = new ToggleButton("Pitch"));
	thePitchButton->setToggleState(theSequencerTree.getProperty("RandPitch"), dontSendNotification);
	thePitchButton->setColour(ToggleButton::textColourId, textButtonTextColour);
	thePitchButton->addListener(this);
	
	addAndMakeVisible(theStateButton = new ToggleButton("OnOff"));
	theStateButton->setToggleState(theSequencerTree.getProperty("RandState"), dontSendNotification);
	theStateButton->setColour(ToggleButton::textColourId, textButtonTextColour);
	theStateButton->addListener(this);
	
	addAndMakeVisible(theResetAllButton = new TextButton("Reset all"));
	theResetAllButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(COLOUR_5));
	theResetAllButton->addListener(this);
	
	theSequencerTree.addListener(this);
}

void Randomiser::resized()
{
	float widthDiv = getWidth() / 14.0f;
	
	thePitchButton->setBounds(0, 0, widthDiv * 2, getHeight());
	theVelocityButton->setBounds(thePitchButton->getRight() , 0, widthDiv * 2, getHeight());
	theDecayButton->setBounds(theVelocityButton->getRight(), 0, widthDiv * 2, getHeight());
	theStateButton->setBounds(theDecayButton->getRight(), 0, widthDiv * 2, getHeight());

	theRandomAllButton->setBounds(theStateButton->getRight(), 0, widthDiv * 6, getHeight());
}

bool Randomiser::altModifyer()
{
//	if (key.isKeyCode(82)) randomiseAll();
	DBG("KEY");
	return false;
}

void Randomiser::buttonClicked(Button* button)
{
	if (button == theRandomAllButton)
	{
		theSequencerView->randomiseAll();
	}
	else if (button == theVelocityButton)
	{
		theSequencerTree.setProperty("RandVelocity", button->getToggleStateValue(), nullptr);
	}
	else if (button == theDecayButton)
	{
		theSequencerTree.setProperty("RandDecay", button->getToggleStateValue(), nullptr);
	}
	else if (button == thePitchButton)
	{
		theSequencerTree.setProperty("RandPitch", button->getToggleStateValue(), nullptr);
	}
	else if (button == theStateButton)
	{
		theSequencerTree.setProperty("RandState", button->getToggleStateValue(), nullptr);
	}
	else if (button == theResetAllButton)
	{
		FileInputStream inputStream(theDefaultPreset);
		ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
		theSequencerTree.copyPropertiesFrom(treeToLoad, nullptr);
		for (int i=0; i<theSequencerTree.getNumChildren(); i++)
		{
			ValueTree sourceChild = treeToLoad.getChild(i);
			ValueTree destinationChild = theSequencerTree.getChild(i);
			destinationChild.copyPropertiesFrom(sourceChild, nullptr);
		}
	}
}

void Randomiser::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
	if(String(property) == "RandVelocity")
	{
		theVelocityButton->setToggleState(tree.getProperty(property), dontSendNotification);
	}
	else if(String(property) == "RandDecay")
	{
		theDecayButton->setToggleState(tree.getProperty(property), dontSendNotification);
	}
	else if(String(property) == "RandPitch")
	{
		thePitchButton->setToggleState(tree.getProperty(property), dontSendNotification);
	}
	else if(String(property) == "RandState")
	{
		theStateButton->setToggleState(tree.getProperty(property), dontSendNotification);
	}
}
