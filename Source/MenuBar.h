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

#ifndef __MENUBAR_H_881DA458__
#define __MENUBAR_H_881DA458__

#include "../JuceLibraryCode/JuceHeader.h"

enum MenuBarCommandIDs
{
	COMMAND_ID_NULL = 1,
	COMMAND_ID_SHOW_ABOUT,
	COMMAND_ID_EXPORTALL,
	COMMAND_ID_IMPORT_ALL,
	COMMAND_ID_CHANGE_PRESET_FOLDER,
	COMMAND_ID_WEBSITE,
	COMMAND_ID_TOOLTIP,
	COMMAND_ID_SKIN1,
	COMMAND_ID_SKIN2,
	COMMAND_ID_SKIN3,
	COMMAND_ID_SKIN4,
	COMMAND_NUM_IDS
};

class MenuBar : public MenuBarModel, public Component
{

public:
	MenuBar(ApplicationCommandTarget *mainTarget);
	~MenuBar();

	void addCommandTarget(ApplicationCommandTarget *target, Component *targetOwner);
	
private:
	ScopedPointer<ApplicationCommandManager> theCommandManager;
	ScopedPointer<PopupMenu> appDropDown;
	
	StringArray	getMenuBarNames();
	void		constructApplicationMenuPopup();
	PopupMenu	getMenuForIndex(int topLevelMenuIndex, const String& menuName);
	void		menuItemSelected(int menuItemID, int topLevelMenuIndex);

};


#endif  // __MENUBAR_H_881DA458__
