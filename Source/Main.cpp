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

#include "../JuceLibraryCode/JuceHeader.h"
#include "Main.h"
#include "Suite.h"
#include "Updater.h"

File thePresetFolder;
File theDefaultPreset;

void SequencerApplication::initialise (const String& commandLine)
{
	thePackageHandler = new Updater();
	thePreferenceTree = ValueTree("Preferences"); Suite::initialiseSuitesArray();
	theSequencerMaster = new Master(thePreferenceTree);
	mainWindow = new MainWindow(theSequencerMaster->getMasterTree(), thePreferenceTree);
}

void SequencerApplication::updateCallback()
{
	startTimer(10);
}
void SequencerApplication::timerCallback()
{
	delete thePackageHandler;
	stopTimer();
}

START_JUCE_APPLICATION (SequencerApplication)
