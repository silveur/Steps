/*
  ==============================================================================

	Master.h
	Created: 23 Apr 2014 12:36:28pm
	Author:  Silvere Letellier

  ==============================================================================
*/

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
		theDefaultPreset = File((File::getSpecialLocation(File::userApplicationDataDirectory)).getFullPathName()+"/Preferences/Nummer/default.seq");
		if (!thePresetFolder.exists()) thePresetFolder.createDirectory();
		theMidiInput = MidiInput::createNewDevice("Sequencer", this);
		theMasterTree = ValueTree("MasterTree");
		ValueTree defaultTree = ValueTree("Sequencer");
		theSequencerArray.add(new Sequencer(defaultTree));
		theMasterTree.addChild(defaultTree, -1, nullptr);
		theClockSource = new ClockSource(this);
		theClockSource->theBPM = preferenceTree.getProperty("BPM");
		theDefaultPreset.deleteFile();
		FileOutputStream outputStream(theDefaultPreset);
		defaultTree.writeToStream(outputStream);
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
	ValueTree thePreferenceTree;
	ValueTree theMasterTree;
	ClockMode theClockMode;
	bool outputClock;
};

#endif  // MASTER_H_INCLUDED
