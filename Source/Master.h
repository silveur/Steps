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

#ifndef MASTER_H_INCLUDED
#define MASTER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sequencer.h"
#include "ClockSource.h"

extern File thePresetFolder;
extern File theDefaultPreset;

class Master: public MidiInputCallback, ValueTree::Listener
{
public:
	Master(ValueTree& preferenceTree)
	{
		thePreferenceTree = preferenceTree;
		thePreferenceTree.addListener(this);
		
		thePresetFolder = File(thePreferenceTree.getProperty("PresetFolder", String("~/")));
		
		theDefaultPreset = File((File::getSpecialLocation(File::userApplicationDataDirectory)).getFullPathName()+"/Preferences/Steps/default.seq");
		theLastStateFile = File((File::getSpecialLocation(File::userApplicationDataDirectory)).getFullPathName()+"/Preferences/Steps/last.seq");
		
		if (!thePresetFolder.exists()) thePresetFolder.createDirectory();
		
		theMidiInput = MidiInput::createNewDevice("Steps", this);
		
		if (!theDefaultPreset.exists())
		{
			theMasterTree = ValueTree("MasterTree");
			ValueTree defaultTree = ValueTree("Steps");
			Sequencer* seq = new Sequencer(defaultTree);
			
			theDefaultPreset.deleteFile();
			FileOutputStream outputStream(theDefaultPreset);
			defaultTree.writeToStream(outputStream);
			
			delete seq;
		}
		
		if (theLastStateFile.exists())
		{
			FileInputStream inputStream(theLastStateFile);
			ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
			theMasterTree = treeToLoad.createCopy();
			for (int i=0;i<theMasterTree.getNumChildren();i++)
			{
				ValueTree sequencerTree = theMasterTree.getChild(i);
				theSequencerArray.add(new Sequencer(sequencerTree));
			}
		}
		else
		{
			theMasterTree = ValueTree("MasterTree");
			ValueTree defaultTree = ValueTree("Steps");
			theSequencerArray.add(new Sequencer(defaultTree));
			theMasterTree.addChild(defaultTree, -1, nullptr);
			
			theDefaultPreset.deleteFile();
			FileOutputStream outputStream(theDefaultPreset);
			defaultTree.writeToStream(outputStream);
		}
		
		theClockSource = new ClockSource(this);
		theClockSource->theBPM = preferenceTree.getProperty("BPM");
		
		theMasterTree.addListener(this);
		theClockMode = (ClockMode)(bool)preferenceTree.getProperty("ClockMode");
		if (theClockMode == EXTERNAL) theMidiInput->start();
		outputClock = thePreferenceTree.getProperty("MasterClock");
	}

	~Master()
	{
		for (int i=0;i<theSequencerArray.size();i++)
		{
			theSequencerArray[i]->stop();
		}
		theMidiInput->stop();
		
		if (theLastStateFile.exists()) theLastStateFile.deleteFile();
		FileOutputStream outputStream(theLastStateFile);
		theMasterTree.writeToStream(outputStream);
	}

	void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
	{
		for (int i=0;i<theSequencerArray.size();i++)
		{
			theSequencerArray[i]->handleIncomingMidiMessage(message);
		}
		if (theMasterClockOutput != nullptr && outputClock && (message.isMidiClock() || message.isMidiStart() || message.isMidiStop()))
		{
			theMasterClockOutput->sendMessageNow(message);
		}
	}

	ValueTree& getMasterTree()
	{
		return theMasterTree;
	}

private:
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
	{
		if (tree == thePreferenceTree)
		{
			if (String(property) == "ClockMode")
			{
				theClockMode = (ClockMode)(bool)tree.getProperty(property);
				if (theClockMode == INTERNAL)
				{
					theMidiInput->stop();
				}
				else if (theClockMode == EXTERNAL)
				{
					theClockSource->stopThread(200);
					theMidiInput->start();
				}
			}
			else if (String(property) == "BPM")
			{
				theClockSource->theBPM = tree.getProperty(property);
			}
			else if (String(property) == "MasterClock")
			{
				int index = tree.getProperty(property);
				if (index == 0) outputClock = false;
				else { theMasterClockOutput = MidiOutput::openDevice(index-1); outputClock = true; }
			}
			else if (String(property) == "State" && theClockMode == INTERNAL)
			{
				bool state = tree.getProperty("State");
				if (state) theClockSource->startThread();
				else theClockSource->stopThread(200);
			}
		}
	}
	
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& child)
	{
		if (parentTree == theMasterTree)
		{
			theSequencerArray.add(new Sequencer(child));
		}
	}
	
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& child)
	{
		if (parentTree == theMasterTree)
		{
			for (int i=0;i<theSequencerArray.size();i++)
			{
				if (theSequencerArray[i]->theSequencerTree == child)
				{
					theSequencerArray.remove(i);
				}
			}
		}
	}
	
	void valueTreeRedirected (ValueTree &treeWhichHasBeenChanged){}
	void valueTreeChildOrderChanged (ValueTree& parent){}
	void valueTreeParentChanged (ValueTree& tree){}

	OwnedArray<Sequencer> theSequencerArray;
	ScopedPointer<ClockSource> theClockSource;
	ScopedPointer<MidiInput> theMidiInput;
	ScopedPointer<MidiOutput> theMasterClockOutput;
	File theLastStateFile;
	ValueTree thePreferenceTree;
	ValueTree theMasterTree;
	ClockMode theClockMode;
	bool outputClock;
};

#endif  // MASTER_H_INCLUDED
