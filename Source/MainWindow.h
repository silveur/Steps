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

class MainWindow: public DocumentWindow, public KeyListener
{
public:
	MainWindow(ValueTree& masterTree): DocumentWindow (String("Sequencer V" + String(ProjectInfo::versionString)),
														Colours::lightgrey,
														DocumentWindow::allButtons)
	{
		setContentOwned (new ControllerView(masterTree), true);
		setUsingNativeTitleBar(true);
		setResizable(false, false);
		thePreferenceFile = File((File::getSpecialLocation(File::userApplicationDataDirectory)).getFullPathName()+"/Preferences/Nummer/pref");
		if (!thePreferenceFile.exists()) thePreferenceFile.create();
		thePreferenceTree = ValueTree("Preferences");
		FileInputStream inputStream(thePreferenceFile);
		ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
		if (treeToLoad.isValid())
		{
			thePreferenceTree.copyPropertiesFrom(treeToLoad, nullptr);
			int x = thePreferenceTree.getProperty("X"); int y = thePreferenceTree.getProperty("Y");
			int w = thePreferenceTree.getProperty("W"); int h = thePreferenceTree.getProperty("H");
			setBounds(x, y, w, h);
		}
		setVisible(true);
		addKeyListener(this);
	}
	
	~MainWindow()
	{
		thePreferenceTree.setProperty("X", getX(), nullptr);
		thePreferenceTree.setProperty("Y", getY(), nullptr);
		thePreferenceTree.setProperty("W", getWidth(), nullptr);
		thePreferenceTree.setProperty("H", getHeight(), nullptr);
		if (thePreferenceFile.exists()) thePreferenceFile.deleteFile();
		FileOutputStream outputStream(thePreferenceFile);
		thePreferenceTree.writeToStream(outputStream);
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
				ControllerView* view = (ControllerView*)getContentComponent();
				ValueTree tree;
				view->addSequencer(tree);
			}
		}
		return true;
	}
	
private:
	File thePreferenceFile;
	ValueTree thePreferenceTree;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};



#endif  // MAINWINDOW_H_INCLUDED
