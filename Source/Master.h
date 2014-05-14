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
#include "ClockGenerator.h"

extern File thePresetFolder;

class Master: public MidiInputCallback, ValueTree::Listener
{
public:
	Master(ValueTree& preferenceTree)
	{
		thePreferenceTree = preferenceTree;
		thePreferenceTree.addListener(this);
		thePresetFolder = File((File::getSpecialLocation(File::userApplicationDataDirectory)).getFullPathName()+"/Preferences/Nummer/presets/");
		theDefaultPreset = File(thePresetFolder.getFullPathName() + "/default.seq");
		if (!thePresetFolder.exists()) thePresetFolder.createDirectory();
		theMidiInput = MidiInput::createNewDevice("Sequencer", this);
		theMasterTree = ValueTree("MasterTree");
		ValueTree defaultTree = ValueTree("Sequencer");
		theSequencerArray.add(new Sequencer(defaultTree));
		theMasterTree.addChild(defaultTree, -1, nullptr);
		theClockGenerator = new ClockGenerator();
		theDefaultPreset.deleteFile();
		FileOutputStream outputStream(theDefaultPreset);
		defaultTree.writeToStream(outputStream);
		theMasterTree.addListener(this);
		theClockMode = (ClockMode)(bool)preferenceTree.getProperty("ClockMode");
		if (theClockMode == EXTERNAL)
			theMidiInput->start();
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
					theMidiInput->start();
				}
			}
			if (String(property) == "BPM")
			{
				theClockGenerator->setBPM(tree.getProperty(property));
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
	ScopedPointer<ClockGenerator> theClockGenerator;
	ScopedPointer<MidiInput> theMidiInput;
	ValueTree thePreferenceTree;
	ValueTree theMasterTree;
	File theDefaultPreset;
	ClockMode theClockMode;
};

#endif  // MASTER_H_INCLUDED
