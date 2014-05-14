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

ControllerView::ControllerView(ValueTree& masterTree, ValueTree& preferenceTree): theMasterTree(masterTree)
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
	updatePositions();
	theMasterTree.addListener(this);
	setInterceptsMouseClicks(false, true);
	theMenuBar = new MenuBar(this);
	theMenuBar->addCommandTarget(this, this);
}

ControllerView::~ControllerView()
{
	delete theHeaderView;
}

void ControllerView::paint(Graphics& g)
{
	g.setColour(Colours::white);
	g.fillAll();
}

void ControllerView::updatePositions()
{
	int sequencer16Height = theMainScreen.getHeight() / 4.6;
	int sequencer32Height = theMainScreen.getHeight() / 2.6;
	int totalHeigth = 0;
	int sequencerWidth = theMainScreen.getWidth() / 1.5;
	theHeaderView->setBounds(0, 0, sequencerWidth, theMainScreen.getHeight() / 24);
	totalHeigth += theHeaderView->getHeight();
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		if ((int)theMasterTree.getChild(i).getProperty("Length") > 16)
		{
			theSequencerViews[i]->setBounds(0, totalHeigth, sequencerWidth, sequencer32Height);
		}
		else if ((int)theMasterTree.getChild(i).getProperty("Length") < 17)
		{
			theSequencerViews[i]->setBounds(0, totalHeigth, sequencerWidth, sequencer16Height);
		}
		totalHeigth += theSequencerViews[i]->getHeight();
	}
	if (theMasterTree.getNumChildren() != 0)
		setSize(sequencerWidth, totalHeigth);
}

void ControllerView::resized()
{
	updatePositions();
	theAboutView->setBounds(getBounds());
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
		updatePositions();
	}
	else
	{
		ValueTree sequencerTree("Sequencer" + String(theMasterTree.getNumChildren()));
		theMasterTree.addChild(sequencerTree, -1, nullptr);
		theSequencerViews.add(new SequencerView(sequencerTree, this));
		addAndMakeVisible(theSequencerViews.getLast());
		updatePositions();
	}
}

void ControllerView::removeSequencer(int i)
{
	if (i == -1)
	{
		int index = theMasterTree.getNumChildren()-1;
		theMasterTree.removeChild(index, nullptr);
		theSequencerViews.remove(index);
		updatePositions();
	}
	else
	{
		theMasterTree.removeChild(i, nullptr);
		theSequencerViews.remove(i);
		updatePositions();
	}
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

