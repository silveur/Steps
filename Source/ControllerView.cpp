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

ControllerView::ControllerView(ValueTree& masterTree, ValueTree& preferenceTree): theMasterTree(masterTree), thePreferenceTree(preferenceTree)
{
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
	adjustSize();
}

ControllerView::~ControllerView()
{
	delete theHeaderView;
}

void ControllerView::paint(Graphics& g)
{
	g.setColour(Colour::fromRGB(198, 201, 180));
	g.fillAll();
}

void ControllerView::resized()
{
	refreshView();
}

void ControllerView::refreshView()
{
	float headerHeight = 4.0f; float totalDiv; float heightInc = 0;
	totalDiv += headerHeight;
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		if ((int)theMasterTree.getChild(i).getProperty("Length") <= 16) totalDiv += 20.0f;
		else totalDiv += 34.0f;
	}
	
	float heightDiv = (float)getHeight() / totalDiv;
		
	theHeaderView->setBounds(0, 0, getWidth(), heightDiv * headerHeight);
	heightInc += theHeaderView->getHeight();
	
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		float sequencerHeigth;
		if ((int)theMasterTree.getChild(i).getProperty("Length") <= 16) sequencerHeigth = 20.0f;
		else sequencerHeigth = 34.0f;
		
		theSequencerViews[i]->setBounds(0, heightInc, getWidth(), heightDiv * sequencerHeigth);
		heightInc += theSequencerViews[i]->getHeight();
	}
	
	theAboutView->setBounds(getBounds());
}

void ControllerView::adjustSize()
{
	float headerHeight = 4.0f; float totalDiv;
	totalDiv += headerHeight;
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		if ((int)theMasterTree.getChild(i).getProperty("Length") <= 16) totalDiv += 20.0f;
		else totalDiv += 34.0f;
	}
	setSize(theMainScreen.getWidth()*0.7f, totalDiv*10);
	refreshView();
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
	adjustSize();
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
	adjustSize();
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

		default: break;
	}
}

