/*
  ==============================================================================

	RootView.h
	Created: 23 Apr 2014 12:15:42pm
	Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef ROOTVIEW_H_INCLUDED
#define ROOTVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Master.h"
#include "SequencerView.h"
#include "MenuBar.h"

class HeaderView;

class ControllerView: public Component, public ValueTree::Listener, public ApplicationCommandTarget
{
public:
	ControllerView(ValueTree& masterTree);
	~ControllerView();
	void resized();
	void kickBack();
	void paint(Graphics& g);
	void addSequencer(ValueTree& sequencerTreeToAdd);
	void removeSequencer(int index);
	void updatePositions();
	const int getNumOfSequencer() const;
	ValueTree& getMasterTree() { return theMasterTree; }
	Rectangle<int>& getScreenSize() { return theMainScreen; }
	
	bool perform(const InvocationInfo& info)
	{
		switch (info.commandID)
		{
			case COMMAND_ID_EXPORTALL:
			{
				DBG("PERFORM");
				return true;
			}
			default: return false;
		}
	}
	
	ApplicationCommandTarget* getNextCommandTarget()
	{
		return this;
	}
	
	void getAllCommands(Array <CommandID>& commands)
	{
		Array<CommandID> commandIDs = Array<CommandID>();
		for (int i = 2; i < (int)COMMAND_NUM_IDS; i++) // Command IDs are non-zero
		{
			commandIDs.add((CommandID)i);
		}
		commands.addArray(commandIDs, 0, commandIDs.size());
	}
	
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
	{
		const String settingsCategory("Settings");
		const String viewCategory("Views");
		const String sizeCategory("Size");
		const String supportCategory("Support");
		
		switch (commandID)
		{
			case COMMAND_ID_EXPORTALL:
			{
				result.setInfo("Export master sequencer",
							   "Check for Seaboard Grand Software and Firmware updates",
							   settingsCategory, 0);
				result.addDefaultKeypress('s', ModifierKeys::commandModifier);
				break;
			}
				
			default: break;
		}
	}

private:
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property){}
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& child){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& child){}
	void valueTreeChildOrderChanged (ValueTree& parent){}
	void valueTreeParentChanged (ValueTree& tree){}
	OwnedArray<SequencerView> theSequencerViews;
	ScopedPointer<MenuBar> theMenuBar;
	Rectangle<int> theMainScreen;
	HeaderView* theHeaderView;
	ValueTree theMasterTree;
};



#endif  // ROOTVIEW_H_INCLUDED
