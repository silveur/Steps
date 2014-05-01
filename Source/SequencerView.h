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

class ControllerView;

class SequencerView: public Component, SliderListener, ButtonListener, AsyncUpdater, public ValueTree::Listener, public ComboBoxListener, public Timer, public KeyListener
{
public:
	SequencerView(ValueTree& sequencerTree, ControllerView* rootView);
	~SequencerView();

	void sliderValueChanged(Slider* slider);
	void resized();
	void paint(Graphics& g);
	void buttonClicked(Button* button);
	void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
	void refreshMidiList();
	void handleAsyncUpdate();
	void updateSelectedMidiOut(String& midiOut);
	void showBubbleMessage(Component* targetComponent, const String& textToShow);
	const char * getTextForEnum(int enumVal);
	void randomiseAll();
	bool keyPressed(const KeyPress &key, Component *originatingComponent)
	{
		if (key.isKeyCode(82))
		{
			showPopUp = false;
			randomiseAll();
			startTimer(100);
		}
		else if (key.isKeyCode(90) && key.getModifiers() == ModifierKeys::commandModifier)
		{
			showPopUp = false;
			theUndoManager->undo();
			startTimer(100);
		}
		else if (key.isKeyCode(90) && key.getModifiers() == (ModifierKeys::commandModifier | ModifierKeys::shiftModifier))
		{
			showPopUp = false;
			theUndoManager->redo();
			startTimer(100);
		}
		
		return true;
	}

	static ValueTree& getCopyTree()
	{
		static ValueTree theCopyTree;
		return theCopyTree;
	}
	void timerCallback()
	{
		showPopUp = true;
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
	OwnedArray<Slider> theStepSliders;
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
	ScopedPointer<BubbleMessageComponent> theCurrentBubbleMessage;
	UndoManager* theUndoManager;
	Scale* theCurrentScale;
	OwnedArray<Scale> theScales;
	StepView theStepView;
	ValueTree theSequencerTree;
	ControllerView* theControllerView;
	Image theStepImage;
	int thePosition;
	bool showPopUp;
};


#endif  // MAINCOMPONENT_H_INCLUDED
