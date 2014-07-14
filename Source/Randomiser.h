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

#ifndef RANDOMISER_H_INCLUDED
#define RANDOMISER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "SequencerView.h"

class Randomiser: public Component, public ButtonListener, public ValueTree::Listener
{
public:
	Randomiser(SequencerView* sequencerView, ValueTree& sequencerTree);
	~Randomiser() {}
	void resized();
	void buttonClicked(Button* button);
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}

private:
	SequencerView* theSequencerView;
	ValueTree theSequencerTree;
	ScopedPointer<TextButton> theRandomAllButton;
	ScopedPointer<TextButton> theResetAllButton;
	ScopedPointer<ToggleButton> theVelocityButton;
	ScopedPointer<ToggleButton> theDecayButton;
	ScopedPointer<ToggleButton> thePitchButton;
	ScopedPointer<ToggleButton> theStateButton;
};

#endif  // RANDOMISER_H_INCLUDED
