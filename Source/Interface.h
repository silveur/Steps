/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sequencer.h"

class Interface: public Component, SliderListener, ButtonListener, AsyncUpdater, public ValueTree::Listener, public ComboBoxListener
{
public:
    Interface(Sequencer* sequencer);
    ~Interface();

    void paint (Graphics&);
	void sliderValueChanged(Slider* slider);
    void resized();
	void buttonClicked(Button* button);
	void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
	void refreshMidiList();
	void handleAsyncUpdate();
	void updateSelectedMidiOut(String& midiOut);
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
	
private:
	void updateNotesAndOctaves();
	Rectangle<int> theMainScreen;
	ScopedPointer<ComboBox> theMidiOutputList;
	ScopedPointer<ComboBox> theRootNoteList;
	ScopedPointer<ComboBox> theRootOctaveList;
	ScopedPointer<Slider> theSequencerLength;
	OwnedArray<Slider> theStepSliders;
	OwnedArray<Slider> theVelocitySliders;
	OwnedArray<ToggleButton> theStateButtons;
	ScopedPointer<TextButton> theRandomAllButton;
	Sequencer* theSequencer;
	ValueTree theSequencerTree;
	int thePosition;

};


#endif  // MAINCOMPONENT_H_INCLUDED
