/*
  ==============================================================================

    MainWindow.h
    Created: 22 Apr 2014 12:37:54pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ControllerView.h"
#include "LookAndFeel.h"

extern File thePresetFolder;

class MainWindow: public ResizableWindow, public KeyListener, public ValueTree::Listener
{
public:
	MainWindow(ValueTree& masterTree, ValueTree& preferenceTree): ResizableWindow("MainWindow", true)
	{
		LookAndFeel::setDefaultLookAndFeel(theLookAndFeel = new SeqLookAndFeel());
		thePreferenceFile = File((File::getSpecialLocation(File::userApplicationDataDirectory)).getFullPathName()+"/Preferences/Steps/pref");
		if (!thePreferenceFile.exists()) thePreferenceFile.create();
		thePreferenceTree = preferenceTree;
		theMasterTree = masterTree;
		FileInputStream inputStream(thePreferenceFile);
		ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
		if (treeToLoad.isValid())
		{
			thePreferenceTree.copyPropertiesFrom(treeToLoad, nullptr);
			int x = thePreferenceTree.getProperty("X", 50); int y = thePreferenceTree.getProperty("Y", 50);
			setBounds(x, y, 0, 0);
			theColourTheme = (ColourTheme)(int)preferenceTree.getProperty("ColourTheme");
		}
		else
			setBounds(100, 100, 100, 100);
		thePresetFolder = thePreferenceTree.getProperty("PresetFolder");
		theControllerView = new ControllerView(masterTree, thePreferenceTree);
		setContentOwned(theControllerView, true);
		theState = false;
		thePreferenceTree.addListener(this);
		setVisible(true);
		addKeyListener(this);
	}
	
	~MainWindow()
	{
		thePreferenceTree.setProperty("X", getX(), nullptr);
		thePreferenceTree.setProperty("Y", getY(), nullptr);
		thePreferenceTree.setProperty("W", getWidth(), nullptr);
		thePreferenceTree.setProperty("H", getHeight(), nullptr);
		thePreferenceTree.setProperty("State", false, nullptr);
		thePreferenceTree.setProperty("ColourTheme", (int)theColourTheme, nullptr);
		if (thePreferenceFile.exists()) thePreferenceFile.deleteFile();
		FileOutputStream outputStream(thePreferenceFile);
		thePreferenceTree.writeToStream(outputStream);
	}
	
	void paint(Graphics& g)
	{
		g.setColour(SeqLookAndFeel::getColour(COLOUR_BACKGROUND));
		g.fillAll();
	}
		
	void closeButtonPressed()
	{		
		JUCEApplication::getInstance()->systemRequestedQuit();
	}
	
	bool keyPressed(const KeyPress &key, Component *originatingComponent)
	{
		if (key.isKeyCode(78))
		{
			ModifierKeys mod = ModifierKeys::getCurrentModifiersRealtime();
			if (mod == ModifierKeys::commandModifier)
			{
				ValueTree tree;
				theControllerView->addSequencer(tree);
			}
		}
		else if(key.isKeyCode(32))
		{
			theState = !theState;
			thePreferenceTree.setProperty("State", theState, nullptr);
		}
		return true;
	}

	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
	{
		if (String(property) == "ColourTheme")
		{
			theColourTheme = (ColourTheme)(int)tree.getProperty(property, false);
			clearContentComponent();
			theControllerView = new ControllerView(theMasterTree, thePreferenceTree);
			setContentOwned(theControllerView, true);
		}
	}
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
	
private:
	File thePreferenceFile;
	ValueTree thePreferenceTree;
	ValueTree theMasterTree;
	ScopedPointer<SeqLookAndFeel> theLookAndFeel;
	ControllerView* theControllerView;
	bool theState;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};



#endif  // MAINWINDOW_H_INCLUDED
