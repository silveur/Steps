/* =====================================================================
 
 * Steps - Midi sequencer
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 ===================================================================== */

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainWindow.h"
#include "Master.h"

class Updater;
class SequencerApplication  : public JUCEApplication, public Timer
{
public:
	SequencerApplication() {}
	
	const String getApplicationName() override       { return ProjectInfo::projectName; }
	const String getApplicationVersion() override    { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override       { return false; }
	
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
	
	void updateCallback();
	
private:
	ScopedPointer<MainWindow> mainWindow;
	ScopedPointer<Master> theSequencerMaster;
	ValueTree thePreferenceTree;
	Updater* thePackageHandler;
};

#endif  // MAIN_H_INCLUDED
