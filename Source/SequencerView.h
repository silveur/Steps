/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sequencer.h"

class SequencerView: public Component, SliderListener, ButtonListener, AsyncUpdater, public ValueTree::Listener, public ComboBoxListener
{
public:
	SequencerView(Sequencer* sequencer);
	~SequencerView();

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
	ScopedPointer<ComboBox> theMidiOutputList;
	ScopedPointer<ComboBox> theRootNoteList;
	ScopedPointer<ComboBox> theRootOctaveList;
	ScopedPointer<Slider> theSequencerLength;
	ScopedPointer<Slider> theShuffleSlider;
	ScopedPointer<Slider> theRangeSlider;
	OwnedArray<Slider> theStepSliders;
	OwnedArray<Slider> theVelocitySliders;
	OwnedArray<ToggleButton> theStateButtons;
	ScopedPointer<TextButton> theRandomAllButton;
	Sequencer* theSequencer;
	ValueTree theSequencerTree;
	Image theStepImage;
	int thePosition;

};


#endif  // MAINCOMPONENT_H_INCLUDED
