/*
  ==============================================================================

	RootView.cpp
	Created: 23 Apr 2014 12:15:42pm
	Author:  Silvere Letellier

  ==============================================================================
*/

#include "ControllerView.h"
#include "HeaderView.h"
#include "AboutView.h"

ColourTheme theColourTheme = DARK;
Colour textButtonTextColour;

ControllerView::ControllerView(ValueTree& masterTree, ValueTree& preferenceTree): theMasterTree(masterTree), thePreferenceTree(preferenceTree)
{
	textButtonTextColour = SeqLookAndFeel::getColour(COLOUR_1);
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		ValueTree sequenceTree = theMasterTree.getChild(i);
		theSequencerViews.add(new SequencerView(sequenceTree, this));
		addAndMakeVisible(theSequencerViews[i]);
	}
	theMainScreen = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
	addAndMakeVisible(theHeaderView = new HeaderView(this, preferenceTree));
	addAndMakeVisible(theAboutView = new AboutView(this));
	theAboutView->setVisible(false);
	theMasterTree.addListener(this);
	setInterceptsMouseClicks(false, true);
	theMenuBar = new MenuBar(this);
	theMenuBar->addCommandTarget(this, this);
	
	refreshView();
}

ControllerView::~ControllerView()
{
	delete theHeaderView;
}

void ControllerView::resized()
{
	refreshView();
}

void ControllerView::refreshView()
{
	float headerHeight = 4.0f; float totalDiv = 0.0f; float pixelsPerDiv = 9.0f;
	theHeaderView->setBounds(0, 0, getWidth(), headerHeight * pixelsPerDiv);
	theHeaderView->repaint();
	totalDiv += theHeaderView->getHeight();
	
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		float sequencerHeigth;
		if ((int)theMasterTree.getChild(i).getProperty("Length") <= 16) sequencerHeigth = 19.0f * pixelsPerDiv;
		else sequencerHeigth = 34.0f * pixelsPerDiv;
		
		theSequencerViews[i]->setBounds(0, totalDiv, getWidth(), sequencerHeigth);
		theSequencerViews[i]->repaint();
		totalDiv += theSequencerViews[i]->getHeight();
	}
	
	theAboutView->setBounds(getBounds());
	setSize(1200, totalDiv);
}

void ControllerView::kickBack()
{
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		theMasterTree.getChild(i).setProperty("KickBack", 1, nullptr);
	}
}

const int ControllerView::getNumOfSequencer() const
{
	return theMasterTree.getNumChildren();
}

void ControllerView::addSequencer(ValueTree& sequencerTreeToAdd)
{
	if(sequencerTreeToAdd.isValid())
	{
		ValueTree copiedTree = sequencerTreeToAdd.createCopy();
		theMasterTree.addChild(copiedTree, -1, nullptr);
		theSequencerViews.add(new SequencerView(copiedTree, this));
		addAndMakeVisible(theSequencerViews.getLast());
	}
	else
	{
		ValueTree sequencerTree("Sequencer" + String(theMasterTree.getNumChildren()));
		theMasterTree.addChild(sequencerTree, -1, nullptr);
		theSequencerViews.add(new SequencerView(sequencerTree, this));
		addAndMakeVisible(theSequencerViews.getLast());
	}
	refreshView();
}

void ControllerView::removeSequencer(int i)
{
	if (i == -1)
	{
		int index = theMasterTree.getNumChildren()-1;
		theMasterTree.removeChild(index, nullptr);
		theSequencerViews.remove(index);
	}
	else
	{
		theMasterTree.removeChild(i, nullptr);
		theSequencerViews.remove(i);
	}
	refreshView();
}

bool ControllerView::perform(const InvocationInfo& info)
{
	switch (info.commandID)
	{
		case COMMAND_ID_SHOW_ABOUT:
		{
			theSequencerViews.clear();
			theHeaderView->setVisible(false);
			theAboutView->setVisible(true);
			return true;
		}
		case COMMAND_ID_EXPORTALL:
		{
			theHeaderView->exportAll();
			return true;
		}
		case COMMAND_ID_IMPORT_ALL:
		{
			theHeaderView->importAll();
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

void ControllerView::aboutViewClicked()
{
	if (theAboutView->isVisible())
	{
		theAboutView->setVisible(false);
		for (int i=0; i<theMasterTree.getNumChildren(); i++)
		{
			ValueTree sequenceTree = theMasterTree.getChild(i);
			theSequencerViews.add(new SequencerView(sequenceTree, this));
			addAndMakeVisible(theSequencerViews[i]);
		}
		theHeaderView->setVisible(true);
		resized();
	}
}

ApplicationCommandTarget* ControllerView::getNextCommandTarget()
{
	return this;
}

void ControllerView::getAllCommands(Array <CommandID>& commands)
{
	Array<CommandID> commandIDs = Array<CommandID>();
	for (int i = 2; i < (int)COMMAND_NUM_IDS; i++) // Command IDs are non-zero
	{
		commandIDs.add((CommandID)i);
	}
	commands.addArray(commandIDs, 0, commandIDs.size());
}

void ControllerView::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
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

