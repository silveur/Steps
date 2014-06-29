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
#include "ComboBox.h"

class ControllerView;
class SeqSlider;
class Randomiser;

class SequencerView: public Component, SliderListener, public ButtonListener, AsyncUpdater, public ValueTree::Listener, public ComboBoxListener,public KeyListener
{
public:
	SequencerView(ValueTree& sequencerTree, ControllerView* rootView);
	~SequencerView();

	void sliderValueChanged(Slider* slider);
	void resized();
	void buttonClicked(Button* button);
	void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
	void trigMidiNote(int sliderIndex);
	void refreshMidiList();
	void handleAsyncUpdate();
	void updateSelectedMidiOut(String& midiOut);
	const char * getTextForEnum(int enumVal);
	void randomiseAll();
	bool keyPressed(const KeyPress &key, Component *originatingComponent)
	{
		if (key.isKeyCode(82)) randomiseAll();
		return false;
	}
	void paint(Graphics& g)
	{
		g.setFont (Font ("Helvetica neue",12.0000f, Font::plain));
		
		g.setColour(Colour::fromRGB(198, 201, 180));
		g.fillAll();

		float heigthDiv;
		if ((int)theSequencerTree.getProperty("Length") <= 16) heigthDiv = getHeight() / 19.0f;
		else heigthDiv = getHeight() / 33.0f;
		float widthDiv = getWidth() / 130.0f;
		
		g.setColour(Colours::black);
		g.drawFittedText("Root Note", widthDiv * 2, heigthDiv * 3, widthDiv * 6, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Octave", widthDiv * 8, heigthDiv * 3, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Scale", widthDiv * 14, heigthDiv * 3, widthDiv * 10, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Division", widthDiv * 26, heigthDiv * 3, widthDiv * 5, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Shuffle", widthDiv * 33, heigthDiv * 3, widthDiv * 10, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Octave range", widthDiv * 45, heigthDiv * 3, widthDiv * 6, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Offset", widthDiv * 52, heigthDiv * 3, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Length", widthDiv * 56, heigthDiv * 3, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Midi output", widthDiv * 102, heigthDiv * 3, widthDiv * 11, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Channel", widthDiv * 113, heigthDiv * 3, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("On / Off", widthDiv * 118, heigthDiv * 3, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);		
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
	ScopedPointer<SeqComboBox> theMidiOutputList;
	ScopedPointer<SeqComboBox> theChannelList;
	ScopedPointer<SeqComboBox> theRootNoteList;
	ScopedPointer<SeqComboBox> theRootOctaveList;
	ScopedPointer<SeqComboBox> theScaleList;
	ScopedPointer<SeqComboBox> theSpeedList;
	ScopedPointer<Slider> theSequencerLength;
	ScopedPointer<Slider> theOffsetSlider;
	OwnedArray<SeqSlider> theStepSliders;
	OwnedArray<Slider> theVelocitySliders;
	OwnedArray<Slider> theDecaySliders;
	OwnedArray<TextButton> theStateButtons;
	ScopedPointer<ToggleButton> theOnOffButton;
	ScopedPointer<TextButton> theCopyButton;
	ScopedPointer<TextButton> thePasteButton;
	ScopedPointer<TextButton> theExportButton;
	ScopedPointer<TextButton> theImportButton;
	ScopedPointer<TextButton> theDeleteButton;
	OwnedArray<TextButton> theShuffleButtons;
	OwnedArray<TextButton> theRangeButtons;
	ScopedPointer<TextButton> theRandomButton;
	ScopedPointer<TextButton> theResetButton;
	UndoManager* theUndoManager;
	Scale* theCurrentScale;
	OwnedArray<Scale> theScales;
	OwnedArray<StepView> theLEDs;
	ValueTree theSequencerTree;
	ControllerView* theControllerView;
	int thePosition;
	int thePreviousStepPosition;
};

#endif  // MAINCOMPONENT_H_INCLUDED
