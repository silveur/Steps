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

class Master: public MidiInputCallback
{
public:
	Master()
	{
        thePreferenceFile = File((File::getSpecialLocation(File::userApplicationDataDirectory)).getFullPathName()+"/Preferences/Nummer/default");
		theMidiInput = MidiInput::createNewDevice("Sequencer", this);
        theMasterTree = ValueTree("MasterTree");
        if (thePreferenceFile.exists())
        {
            FileInputStream fileInputStream(thePreferenceFile);
            ValueTree treeToLoad = ValueTree::readFromStream(fileInputStream);
            theMasterTree.copyPropertiesFrom(treeToLoad, nullptr);
            for (int i=0;i<treeToLoad.getNumChildren();i++)
            {
                ValueTree tree = treeToLoad.getChild(i).createCopy();
                theMasterTree.addChild(tree, -1, nullptr);
                theSequencerArray.add(new Sequencer(tree));
            }

        }
        else
        {
            thePreferenceFile.create();
            for (int i=0;i<3;i++)
            {
                ValueTree tree = ValueTree("Sequencer" + String(i));
                initTree(tree);
                theSequencerArray.add(new Sequencer(tree));
                theMasterTree.addChild(tree, -1, nullptr);
            }
        }
        
		theMidiInput->start();
	}
	~Master()
	{
        thePreferenceFile.deleteFile();
        FileOutputStream presetToSave(thePreferenceFile);
        theMasterTree.writeToStream(presetToSave);
		theMidiInput->stop();
	}
	
	void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
	{
		for (int i=0;i<theSequencerArray.size();i++)
		{
			theSequencerArray[i]->handleIncomingMidiMessage(message);
		}
	}
    
    Sequencer* addSequencer()
    {
        
        ValueTree existingSequencerTree = theMasterTree.getChild(theMasterTree.getNumChildren());
        if (existingSequencerTree.isValid())
        {
             return theSequencerArray.add(new Sequencer(existingSequencerTree));
        }
        else
        {            
            existingSequencerTree = ValueTree("Sequencer" + String(theMasterTree.getNumChildren()));
            theMasterTree.addChild(existingSequencerTree, -1, nullptr);
            return theSequencerArray.add(new Sequencer(existingSequencerTree));
        }
    }
	
	OwnedArray<Sequencer>& getSequencerArray()
	{
		return theSequencerArray;
	}
    
    void initTree(ValueTree& tree)
    {
        tree.setProperty("Length", 16, nullptr);
		tree.setProperty("RootNote", 0, nullptr);
		tree.setProperty("RootOctave", 3, nullptr);
		tree.setProperty("Shuffle", 0, nullptr);
		tree.setProperty("Range", 1, nullptr);
    }
	
private:
	OwnedArray<Sequencer> theSequencerArray;
	ScopedPointer<MidiInput> theMidiInput;
    ValueTree theMasterTree;
    File thePreferenceFile;
};



#endif  // MASTER_H_INCLUDED
