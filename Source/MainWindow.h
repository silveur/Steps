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
#include "Interface.h"

class MainWindow    : public DocumentWindow
{
public:
	MainWindow(Sequencer* sequencer)  : DocumentWindow ("MainWindow",
														Colours::lightgrey,
														DocumentWindow::allButtons)
	{
		setContentOwned (new Interface(sequencer), true);
		setUsingNativeTitleBar(true);
		centreWithSize (getWidth(), getHeight());
		setVisible (true);
	}
	
	void closeButtonPressed()
	{
		
		JUCEApplication::getInstance()->systemRequestedQuit();
	}
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};



#endif  // MAINWINDOW_H_INCLUDED
