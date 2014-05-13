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

class AboutView;
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
	bool perform(const InvocationInfo& info);
	ApplicationCommandTarget* getNextCommandTarget();
	void getAllCommands(Array <CommandID>& commands);
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);
	void aboutViewClicked();
	
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
	ScopedPointer<AboutView> theAboutView;
	ValueTree theMasterTree;
};



#endif  // ROOTVIEW_H_INCLUDED
