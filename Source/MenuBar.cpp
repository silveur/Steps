/*
 ==============================================================================
 
 MenuBar.cpp
 Created: 4 Apr 2013 12:17:47pm
 Author:  Christopher Fonseka
 
 ==============================================================================
 */

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
