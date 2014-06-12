/*
  ==============================================================================

    Randomiser.cpp
    Created: 12 Jun 2014 12:19:21pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#include "Randomiser.h"

extern File thePresetFolder;
extern File theDefaultPreset;

Randomiser::Randomiser(SequencerView* sequencerView, ValueTree& sequencerTree): theSequencerView(sequencerView), theSequencerTree(sequencerTree)
{
	addAndMakeVisible(theRandomAllButton = new TextButton("Randomise"));
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
	theResetAllButton->addListener(this);
	
	theSequencerTree.addListener(this);
}

void Randomiser::resized()
{
	int heigthDiv = getHeight() / 8;
	int widthDiv = getWidth() / 4;
	theVelocityButton->setBounds(0, 0, widthDiv, heigthDiv*4);
	theDecayButton->setBounds(theVelocityButton->getRight(), theVelocityButton->getY(), widthDiv, heigthDiv*4);
	thePitchButton->setBounds(theDecayButton->getRight(), theDecayButton->getY(), widthDiv, heigthDiv*4);
	theStateButton->setBounds(thePitchButton->getRight(), thePitchButton->getY(), widthDiv, heigthDiv*4);
	
	theRandomAllButton->setBounds(0, heigthDiv*4, getWidth()/2, heigthDiv*4);
	theResetAllButton->setBounds(theRandomAllButton->getRight(), theRandomAllButton->getY(), getWidth()/2, heigthDiv*4);
}

void Randomiser::paint(Graphics &g)
{
	g.setColour(Colours::grey);
	g.fillAll();
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
