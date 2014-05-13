/*
 ==============================================================================
 
 MenuBar.h
 Created: 4 Apr 2013 12:17:47pm
 Author:  Christopher Fonseka
 
 ==============================================================================
 */

#ifndef __MENUBAR_H_881DA458__
#define __MENUBAR_H_881DA458__

#include "../JuceLibraryCode/JuceHeader.h"

enum MenuBarCommandIDs
{
	COMMAND_ID_NULL = 1, // Application Cammand Targets must be non-zero
	COMMAND_ID_SHOW_ABOUT,
	COMMAND_ID_EXPORTALL,
	COMMAND_ID_IMPORT_ALL,
	COMMAND_ID_WEBSITE,
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
