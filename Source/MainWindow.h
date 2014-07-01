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

#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ControllerView.h"
#include "LookAndFeel.h"
#include "AboutView.h"

extern File thePresetFolder;

class MainWindow: public ResizableWindow, public KeyListener, public ValueTree::Listener, public ApplicationCommandTarget
{
public:
	MainWindow(ValueTree& masterTree, ValueTree& preferenceTree): ResizableWindow("MainWindow", true)
	{
		LookAndFeel::setDefaultLookAndFeel(theLookAndFeel = new SeqLookAndFeel());
		thePreferenceFile = File((File::getSpecialLocation(File::userApplicationDataDirectory)).getFullPathName()+"/Preferences/Steps/pref");
		if (!thePreferenceFile.exists()) thePreferenceFile.create();
		thePreferenceTree = preferenceTree;
		theMasterTree = masterTree;
		FileInputStream inputStream(thePreferenceFile);
		ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
		if (treeToLoad.isValid())
		{
			thePreferenceTree.copyPropertiesFrom(treeToLoad, nullptr);
			int x = thePreferenceTree.getProperty("X", 50); int y = thePreferenceTree.getProperty("Y", 50);
			setBounds(x, y, 0, 0);
			theColourTheme = (ColourTheme)(int)preferenceTree.getProperty("ColourTheme");
		}
		else
			setBounds(100, 100, 100, 100);
		theMainScreen = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
		thePresetFolder = thePreferenceTree.getProperty("PresetFolder");
		theControllerView = new ControllerView(masterTree, thePreferenceTree);
		theAboutWindow = new AboutWindow();
		theAboutWindow->setVisible(false);
		setContentOwned(theControllerView, true);
		theState = false;
		thePreferenceTree.addListener(this);
		setVisible(true);
		addKeyListener(this);
		theMenuBar = new MenuBar(this);
		theMenuBar->addCommandTarget(this, this);
	}
	
	~MainWindow()
	{
		thePreferenceTree.setProperty("X", getX(), nullptr);
		thePreferenceTree.setProperty("Y", getY(), nullptr);
		thePreferenceTree.setProperty("W", getWidth(), nullptr);
		thePreferenceTree.setProperty("H", getHeight(), nullptr);
		thePreferenceTree.setProperty("State", false, nullptr);
		thePreferenceTree.setProperty("ColourTheme", (int)theColourTheme, nullptr);
		if (thePreferenceFile.exists()) thePreferenceFile.deleteFile();
		FileOutputStream outputStream(thePreferenceFile);
		thePreferenceTree.writeToStream(outputStream);
	}
	
	void paint(Graphics& g)
	{
		g.setColour(SeqLookAndFeel::getColour(COLOUR_BACKGROUND));
		g.fillAll();
	}
		
	void closeButtonPressed()
	{		
		JUCEApplication::getInstance()->systemRequestedQuit();
	}
	
	bool keyPressed(const KeyPress &key, Component *originatingComponent)
	{
		if (key.isKeyCode(78))
		{
			ModifierKeys mod = ModifierKeys::getCurrentModifiersRealtime();
			if (mod == ModifierKeys::commandModifier)
			{
				ValueTree tree;
				theControllerView->addSequencer(tree);
			}
		}
		else if(key.isKeyCode(32))
		{
			theState = !theState;
			thePreferenceTree.setProperty("State", theState, nullptr);
		}
		return true;
	}

	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
	{
		if (String(property) == "ColourTheme")
		{
			theColourTheme = (ColourTheme)(int)tree.getProperty(property, false);
			clearContentComponent();
			theControllerView = new ControllerView(theMasterTree, thePreferenceTree);
			setContentOwned(theControllerView, true);
		}
	}
	
	bool perform(const InvocationInfo& info)
	{
		switch (info.commandID)
		{
			case COMMAND_ID_SHOW_ABOUT:
			{
				if (!theAboutWindow->isVisible())
				{
					int aboutViewWidth = theMainScreen.getWidth()/4; int aboutViewHeight = theMainScreen.getHeight()/5;
					theAboutWindow->setBounds(aboutViewWidth * 2 - (aboutViewWidth/2), aboutViewHeight, aboutViewWidth, aboutViewHeight);
					theAboutWindow->setVisible(true);
				}
				return true;
			}
			case COMMAND_ID_EXPORTALL:
			{
				theControllerView->exportAll();
				return true;
			}
			case COMMAND_ID_IMPORT_ALL:
			{
				theControllerView->importAll();
				return true;
			}
			case COMMAND_ID_CHANGE_PRESET_FOLDER:
			{
				FileChooser fileChooser ("Select New Preset Folder",
										 thePresetFolder,
										 "*");
				if (fileChooser.browseForDirectory())
				{
					thePresetFolder = fileChooser.getResult();
					thePreferenceTree.setProperty("PresetFolder", thePresetFolder.getFullPathName(), nullptr);
				}
				return true;
			}
			case COMMAND_ID_WEBSITE:
			{
				system("open http://www.nummermusic.com");
				return true;
			}
			case COMMAND_ID_SKIN1:
			{
				thePreferenceTree.setProperty("ColourTheme", 0, nullptr);
				return true;
			}
			case COMMAND_ID_SKIN2:
			{
				thePreferenceTree.setProperty("ColourTheme", 1, nullptr);
				return true;
			}
			case COMMAND_ID_SKIN3:
			{
				thePreferenceTree.setProperty("ColourTheme", 2, nullptr);
				return true;
			}
			case COMMAND_ID_SKIN4:
			{
				thePreferenceTree.setProperty("ColourTheme", 3, nullptr);
				return true;
			}
			default: return false;
		}
	}
	
	ApplicationCommandTarget* getNextCommandTarget()
	{
		return this;
	}
	
	void getAllCommands(Array <CommandID>& commands)
	{
		Array<CommandID> commandIDs = Array<CommandID>();
		for (int i = 2; i < (int)COMMAND_NUM_IDS; i++) // Command IDs are non-zero
		{
			commandIDs.add((CommandID)i);
		}
		commands.addArray(commandIDs, 0, commandIDs.size());
	}
	
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
	{
		const String settingsCategory("Settings");
		const String viewCategory("Views");
		const String sizeCategory("Size");
		const String supportCategory("Support");
		
		switch (commandID)
		{
			case COMMAND_ID_SHOW_ABOUT:
			{
				result.setInfo("About Sequencer",
							   "About this software",
							   settingsCategory, 0);
				break;
			}
			case COMMAND_ID_EXPORTALL:
			{
				result.setInfo("Export All",
							   "Export master sequencer",
							   settingsCategory, 0);
				result.addDefaultKeypress('s', ModifierKeys::commandModifier);
				break;
			}
			case COMMAND_ID_IMPORT_ALL:
			{
				result.setInfo("Import All",
							   "Import master sequencer",
							   settingsCategory, 0);
				result.addDefaultKeypress('s', ModifierKeys::commandModifier + ModifierKeys::altModifier);
				break;
			}
			case COMMAND_ID_CHANGE_PRESET_FOLDER:
			{
				result.setInfo("Set Preset Folder",
							   "Select new preset folder",
							   settingsCategory, 0);
				break;
			}
			case COMMAND_ID_WEBSITE:
			{
				result.setInfo("Visit our website",
							   "Nummer website",
							   settingsCategory, 0);
				break;
			}
			case COMMAND_ID_SKIN1:
			{
				result.setInfo("Dark",
							   "Dark view",
							   viewCategory, 0);
				result.addDefaultKeypress('1', ModifierKeys::commandModifier);
				break;
			}
			case COMMAND_ID_SKIN2:
			{
				result.setInfo("Bright",
							   "Bright view",
							   viewCategory, 0);
				result.addDefaultKeypress('2', ModifierKeys::commandModifier);
				break;
			}
			case COMMAND_ID_SKIN3:
			{
				result.setInfo("Wine",
							   "Wine view",
							   viewCategory, 0);
				result.addDefaultKeypress('3', ModifierKeys::commandModifier);
				break;
			}
			case COMMAND_ID_SKIN4:
			{
				result.setInfo("Etsy",
							   "Etsy view",
							   viewCategory, 0);
				result.addDefaultKeypress('4', ModifierKeys::commandModifier);
				break;
			}
			default: break;
		}
	}
	
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
	
private:
	File thePreferenceFile;
	ValueTree thePreferenceTree;
	ValueTree theMasterTree;
	Rectangle<int> theMainScreen;
	ScopedPointer<SeqLookAndFeel> theLookAndFeel;
	ScopedPointer<AboutWindow> theAboutWindow;
	ControllerView* theControllerView;
	ScopedPointer<MenuBar> theMenuBar;
	bool theState;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

#endif  // MAINWINDOW_H_INCLUDED
