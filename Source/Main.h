/*
  ==============================================================================

    main.h
    Created: 10 May 2014 1:17:17pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainWindow.h"
#include "Master.h"

class PackageHandler;
class SequencerApplication  : public JUCEApplication, public Timer
{
public:
	SequencerApplication() {}
	
	const String getApplicationName() override       { return ProjectInfo::projectName; }
	const String getApplicationVersion() override    { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override       { return true; }
	
	void initialise (const String& commandLine) override;
	void timerCallback();
	void shutdown() override
	{
		mainWindow = nullptr;
	}
	
	void systemRequestedQuit() override
	{
		quit();
	}
	
	void anotherInstanceStarted (const String& commandLine) override
	{
	}
	
	void updateCallback();
	
private:
	ScopedPointer<MainWindow> mainWindow;
	ScopedPointer<Master> theSequencerMaster;
	ValueTree thePreferenceTree;
	#ifndef OLD_MAC
	PackageHandler* thePackageHandler;
	#endif
};



#endif  // MAIN_H_INCLUDED
