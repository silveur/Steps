/* =====================================================================
 
 * Steps - Midi sequencer>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ===================================================================== */

#ifndef ROOTVIEW_H_INCLUDED
#define ROOTVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Master.h"
#include "SequencerView.h"
#include "MenuBar.h"

class HeaderView;

class ControllerView: public Component, public ValueTree::Listener
{
public:
	ControllerView(ValueTree& masterTree, ValueTree& preferenceTree);
	~ControllerView();
	void resized();
	void exportAll();
	void importAll();
	void refreshView();
	void addSequencer(ValueTree& sequencerTreeToAdd);
	void removeSequencer(int index);
	const int getNumOfSequencer() const;
	ValueTree& getMasterTree() { return theMasterTree; }

private:
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property){}
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& child){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& child){}
	void valueTreeChildOrderChanged (ValueTree& parent){}
	void valueTreeParentChanged (ValueTree& tree){}
	OwnedArray<SequencerView> theSequencerViews;
	HeaderView* theHeaderView;
	ValueTree theMasterTree;
	ValueTree thePreferenceTree;
};



#endif  // ROOTVIEW_H_INCLUDED
