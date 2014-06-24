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
		g.setColour(Colour::fromRGB(198, 201, 180));
		g.fillAll();
		g.setColour(Colours::black);
		g.drawRect(getLocalBounds());
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
	ScopedPointer<ComboBox> theSpeedList;
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
	ScopedPointer<TextButton> theDeleteButton;
	ScopedPointer<StepView> thePositionComp;
	ScopedPointer<StepView> thePositionComp2;
	ScopedPointer<Randomiser> theRandomiser;
	UndoManager* theUndoManager;
	Scale* theCurrentScale;
	OwnedArray<Scale> theScales;
	ValueTree theSequencerTree;
	ControllerView* theControllerView;
	int thePosition;
};

#endif  // MAINCOMPONENT_H_INCLUDED
