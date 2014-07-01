/* =====================================================================
 
 * Steps - Midi sequencer
 * Copyright (C) 2014  Silvere Letellier for Nummer Music
 * Contact: <silvere.letellier@gmail.com>
 
 -----------------------------------------------------------------------
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 ===================================================================== */

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "StepsView.h"
#include "Scales.h"
#include "LookAndFeel.h"

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
		g.setFont (Font ("Helvetica Neue",12.0000f, Font::plain));
		g.setColour(SeqLookAndFeel::getColour(COLOUR_1));

		float heigthDiv;
		if ((int)theSequencerTree.getProperty("Length") <= 16) heigthDiv = getHeight() / 19.0f;
		else heigthDiv = getHeight() / 34.0f;
		float widthDiv = getWidth() / 130.0f;
		
		g.drawFittedText("Root Note", widthDiv * 2, heigthDiv * 3, widthDiv * 6, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Octave", widthDiv * 8, heigthDiv * 3, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Scale", widthDiv * 14, heigthDiv * 3, widthDiv * 10, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Division", widthDiv * 26, heigthDiv * 3, widthDiv * 5, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Shuffle", widthDiv * 33, heigthDiv * 3, widthDiv * 10, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Octave range", widthDiv * 45, heigthDiv * 3, widthDiv * 6, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Offset", widthDiv * 53, heigthDiv * 3, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Length", widthDiv * 57, heigthDiv * 3, widthDiv * 4, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Midi output", widthDiv * 102, heigthDiv * 3, widthDiv * 13, heigthDiv * 2, Justification::centred, 1);
		g.drawFittedText("Channel", widthDiv * 115, heigthDiv * 3, widthDiv * 5, heigthDiv * 2, Justification::centred, 1);
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
	ScopedPointer<Randomiser> theRandomiser;

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
