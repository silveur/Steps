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
#include "Master.h"

class MainWindow: public DocumentWindow
{
public:
	MainWindow(ValueTree& masterTree)  : DocumentWindow ("Sequencer",
														Colours::lightgrey,
														DocumentWindow::allButtons)
	{
		setContentOwned (new ControllerView(masterTree), true);
		setUsingNativeTitleBar(true);
		setResizable(false, false);
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
