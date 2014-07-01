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

#include "MenuBar.h"

void MenuBar::constructApplicationMenuPopup()
{
	appDropDown = new PopupMenu();
	appDropDown->addCommandItem(theCommandManager, COMMAND_ID_SHOW_ABOUT);
	appDropDown->addSeparator();
}

MenuBar::MenuBar(ApplicationCommandTarget *mainTarget)
{
	theCommandManager = new ApplicationCommandManager();
	setApplicationCommandManagerToWatch(theCommandManager);
	theCommandManager->setFirstCommandTarget(mainTarget);
	theCommandManager->registerAllCommandsForTarget(JUCEApplication::getInstance());
}

MenuBar::~MenuBar()
{
#if JUCE_MAC
	MenuBarModel::setMacMainMenu(nullptr);
#endif
}

void MenuBar::addCommandTarget(juce::ApplicationCommandTarget *target, juce::Component *targetOwner)
{
	theCommandManager->registerAllCommandsForTarget(target);
	targetOwner->addKeyListener(theCommandManager->getKeyMappings());
	
	constructApplicationMenuPopup();
	
#if JUCE_MAC
	MenuBarModel::setMacMainMenu(this, appDropDown);
#endif
	addKeyListener(theCommandManager->getKeyMappings());
}

StringArray MenuBar::getMenuBarNames()
{
	StringArray names;
	names.add("Edit");
	names.add("View");
	names.add("Help");
	return names;
}

PopupMenu MenuBar::getMenuForIndex(int topLevelMenuIndex, const juce::String &menuName)
{
	PopupMenu menu = PopupMenu();
	switch (topLevelMenuIndex)
	{
		case 0:
		{
			menu.addCommandItem(theCommandManager, COMMAND_ID_EXPORTALL);
			menu.addCommandItem(theCommandManager, COMMAND_ID_IMPORT_ALL);
			menu.addCommandItem(theCommandManager, COMMAND_ID_CHANGE_PRESET_FOLDER);
			break;
		}
		case 1:
		{
			menu.addCommandItem(theCommandManager, COMMAND_ID_SKIN1);
			menu.addCommandItem(theCommandManager, COMMAND_ID_SKIN2);
			menu.addCommandItem(theCommandManager, COMMAND_ID_SKIN3);
			menu.addCommandItem(theCommandManager, COMMAND_ID_SKIN4);
			break;
		}
		case 2:
		{
			menu.addCommandItem(theCommandManager, COMMAND_ID_WEBSITE);
			break;
		}
		default: break;
	}
	return menu;
}

void MenuBar::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
	// Do Nothing
	// All commands handled in MainWindow
}
