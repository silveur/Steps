/*
  ==============================================================================

    Randomiser.cpp
    Created: 12 Jun 2014 12:19:21pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#include "Randomiser.h"
#include "LookAndFeel.h"

extern File thePresetFolder;
extern File theDefaultPreset;

Randomiser::Randomiser(SequencerView* sequencerView, ValueTree& sequencerTree): theSequencerView(sequencerView), theSequencerTree(sequencerTree)
{
	addAndMakeVisible(theRandomAllButton = new TextButton("Randomise"));
	theRandomAllButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreen));
	theRandomAllButton->addListener(this);
	
	addAndMakeVisible(theVelocityButton = new ToggleButton("Velocity"));
	theVelocityButton->setToggleState(theSequencerTree.getProperty("RandVelocity"), dontSendNotification);
	theVelocityButton->addListener(this);
	
	addAndMakeVisible(theDecayButton = new ToggleButton("Decay"));
	theDecayButton->setToggleState(theSequencerTree.getProperty("RandDecay"), dontSendNotification);
	theDecayButton->addListener(this);
	
	addAndMakeVisible(thePitchButton = new ToggleButton("Pitch"));
	thePitchButton->setToggleState(theSequencerTree.getProperty("RandPitch"), dontSendNotification);
	thePitchButton->addListener(this);
	
	addAndMakeVisible(theStateButton = new ToggleButton("State"));
	theStateButton->setToggleState(theSequencerTree.getProperty("RandState"), dontSendNotification);
	theStateButton->addListener(this);
	
	addAndMakeVisible(theResetAllButton = new TextButton("Reset all"));
	theResetAllButton->setColour(TextButton::buttonColourId, SeqLookAndFeel::getColour(ColourGreen));
	theResetAllButton->addListener(this);
	
	theSequencerTree.addListener(this);
}

void Randomiser::resized()
{
	float heigthDiv = getHeight() / 6.0f;
	float widthDiv = getWidth() / 21.0f;
	
	theVelocityButton->setBounds(widthDiv * 1.8, 0, widthDiv * 3, heigthDiv * 3);
	theDecayButton->setBounds(widthDiv * 6.8, 0, widthDiv * 3, heigthDiv * 3);
	thePitchButton->setBounds(widthDiv * 11.8, 0, widthDiv * 3, heigthDiv * 3);
	theStateButton->setBounds(widthDiv * 16.8, 0, widthDiv * 3, heigthDiv * 3);

	theRandomAllButton->setBounds(0, heigthDiv * 4, getWidth()/2, heigthDiv * 2);
	theResetAllButton->setBounds(getWidth()/2.0f, heigthDiv * 4, getWidth()/2, heigthDiv * 2);
}

void Randomiser::paint(Graphics &g)
{
	g.setColour(Colours::grey);
	g.fillAll();
	
	g.setFont(10.0f);
	
	float heigthDiv = getHeight() / 6.0f;
	float widthDiv = getWidth() / 20.0f;
	
	g.setColour(Colours::black);
	g.drawFittedText("Velocity", widthDiv, heigthDiv * 2.4, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
	g.drawFittedText("Decay", widthDiv * 6, heigthDiv * 2.4, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
	g.drawFittedText("Pitch", widthDiv * 10.8f, heigthDiv * 2.4, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
	g.drawFittedText("State", widthDiv * 15.3f, heigthDiv * 2.4, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);

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
