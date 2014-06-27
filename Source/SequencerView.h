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
#include "DeleteButton.h"
#include "ComboBox.h"

class ControllerView;
class SeqSlider;
class Randomiser;

class SequencerView: public Component, SliderListener, ButtonListener, AsyncUpdater, public ValueTree::Listener, public ComboBoxListener,public KeyListener
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
		g.setFont(11);
		g.setColour(Colour::fromRGB(198, 201, 180));
		g.fillAll();

		float heigthDiv = getHeight() / 40.0f;
		float widthDiv = getWidth() / 132.0f;
		
		g.setColour(Colours::black);
		g.drawFittedText("Root Note", widthDiv * 2, heigthDiv * 5, widthDiv * 6, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Octave", widthDiv * 8, heigthDiv * 5, widthDiv * 6, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Scale", widthDiv * 16, heigthDiv * 5, widthDiv * 10, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Division", widthDiv * 28, heigthDiv * 5, widthDiv * 6, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Shuffle", widthDiv * 59, heigthDiv * 5, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Range", widthDiv * 65, heigthDiv * 5, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Offset", widthDiv * 71, heigthDiv * 5, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Length", widthDiv * 77, heigthDiv * 5, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Midi output", widthDiv * 95, heigthDiv * 5, widthDiv * 16, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Channel", widthDiv * 113, heigthDiv * 5, widthDiv * 6, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("On / Off", widthDiv * 121, heigthDiv * 5, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Delete", widthDiv * 126, heigthDiv * 5, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
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
	ScopedPointer<Slider> theShuffleSlider;
	ScopedPointer<Slider> theRangeSlider;
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
	ScopedPointer<DeleteButton> theDeleteButton;
	ScopedPointer<Randomiser> theRandomiser;
	UndoManager* theUndoManager;
	Scale* theCurrentScale;
	OwnedArray<Scale> theScales;
	OwnedArray<StepView> theLEDs;
	ValueTree theSequencerTree;
	ControllerView* theControllerView;
	int thePosition;
};

#endif  // MAINCOMPONENT_H_INCLUDED
