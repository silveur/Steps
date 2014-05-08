/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "StepsView.h"
#include "Scales.h"
#include "Slider.h"

class ControllerView;
class PositionComp;

class SequencerView: public Component, SliderListener, ButtonListener, AsyncUpdater, public ValueTree::Listener, public ComboBoxListener,public KeyListener
{
public:
	SequencerView(ValueTree& sequencerTree, ControllerView* rootView);
	~SequencerView();

	void sliderValueChanged(Slider* slider);
	void resized();
	void buttonClicked(Button* button);
	void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
	void refreshMidiList();
	void handleAsyncUpdate();
	void updateSelectedMidiOut(String& midiOut);
	const char * getTextForEnum(int enumVal);
	void randomiseAll();
	bool keyPressed(const KeyPress &key, Component *originatingComponent)
	{
		if (key.isKeyCode(82)) randomiseAll();
		return true;
	}

	static ValueTree& getCopyTree()
	{
		static ValueTree theCopyTree;
		return theCopyTree;
	}

private:
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
	void updateNotesAndOctaves();
	void loadScales();
	String isOnScale(int value);
	ScopedPointer<ComboBox> theMidiOutputList;
	ScopedPointer<ComboBox> theChannelList;
	ScopedPointer<ComboBox> theRootNoteList;
	ScopedPointer<ComboBox> theRootOctaveList;
	ScopedPointer<ComboBox> theScaleList;
	ScopedPointer<Slider> theSequencerLength;
	ScopedPointer<Slider> theShuffleSlider;
	ScopedPointer<Slider> theRangeSlider;
	ScopedPointer<Slider> theOffsetSlider;
	OwnedArray<SeqSlider> theStepSliders;
	OwnedArray<Slider> theVelocitySliders;
	OwnedArray<Slider> theDecaySliders;
	OwnedArray<TextButton> theStateButtons;
	ScopedPointer<ToggleButton> theOnOffButton;
	ScopedPointer<TextButton> theRandomAllButton;
	ScopedPointer<TextButton> theResetAllButton;
	ScopedPointer<TextButton> theCopyButton;
	ScopedPointer<TextButton> thePasteButton;
	ScopedPointer<TextButton> theExportButton;
	ScopedPointer<TextButton> theImportButton;
	ScopedPointer<PositionComp> thePositionComp;
	UndoManager* theUndoManager;
	Scale* theCurrentScale;
	OwnedArray<Scale> theScales;
	StepView theStepView;
	ValueTree theSequencerTree;
	ControllerView* theControllerView;
	Image theStepImage;
	int thePosition;
	friend class PositionComp;
};

class PositionComp: public Component
{
public:
	PositionComp(SequencerView* view)
	{
		theSequencerView = view;
		addAndMakeVisible(theSlider = new Slider(""));
	}
	
	void paint(Graphics& g)
	{
		g.fillAll(Colours::red);
		g.drawRect(theSequencerView->theStateButtons[theSequencerView->thePosition]->getX(), theSequencerView->theStateButtons[theSequencerView->thePosition]->getBottom(), 20, 20);
	}

	void resized()
	{
		theSlider->setBounds(0, 0, 20, 20);
		repaint();
	}
	
private:
	ScopedPointer<Slider> theSlider;
	SequencerView* theSequencerView;
	
};


#endif  // MAINCOMPONENT_H_INCLUDED
