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

extern File thePresetFolder;

class Master: public MidiInputCallback, ValueTree::Listener
{
public:
	Master()
	{
		thePresetFolder = File((File::getSpecialLocation(File::userApplicationDataDirectory)).getFullPathName()+"/Preferences/Nummer/presets/");
		theDefaultPreset = File(thePresetFolder.getFullPathName() + "/default");
		theMidiInput = MidiInput::createNewDevice("Sequencer", this);
		theMasterTree = ValueTree("MasterTree");
		ValueTree tree = ValueTree("Sequencer1");
		theSequencerArray.add(new Sequencer(tree));
		theMasterTree.addChild(tree, -1, nullptr);
		if (!theDefaultPreset.existsAsFile())
		{
			theDefaultPreset.create();
			FileOutputStream outputStream(theDefaultPreset);
			theMasterTree.writeToStream(outputStream);
		}
		theMasterTree.addListener(this);
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
			int childIndex = parentTree.indexOf(child);
			theSequencerArray.remove(childIndex);
		}
	}
	
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property){}
	void valueTreeChildOrderChanged (ValueTree& parent){}
	void valueTreeParentChanged (ValueTree& tree){}

private:
	OwnedArray<Sequencer> theSequencerArray;
	ScopedPointer<MidiInput> theMidiInput;
	ValueTree theMasterTree;
	File theDefaultPreset;
};

#endif  // MASTER_H_INCLUDED
