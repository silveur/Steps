/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainWindow.h"
#include "Master.h"

class SequencerApplication  : public JUCEApplication
{
public:
    SequencerApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    void initialise (const String& commandLine) override
    {
		theSequencerMaster = new Master();
        mainWindow = new MainWindow(theSequencerMaster->getSequencerArray());
    }

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

private:
    ScopedPointer<MainWindow> mainWindow;
	ScopedPointer<Master> theSequencerMaster;
};

START_JUCE_APPLICATION (SequencerApplication)
