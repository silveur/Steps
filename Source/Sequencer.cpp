/*
  ==============================================================================

    Sequencer.cpp
    Created: 22 Dec 2013 11:52:14pm
    Author:  silvere letellier

  ==============================================================================
*/

#include "Sequencer.h"
#include "PluginProcessor.h"
#include "MidiCore.h"

int Sequencer::theStepTime = 0;

Sequencer::Sequencer(SequencerAudioProcessor* processor, MidiCore* midiCore)
{
	theMidiCore = midiCore;
	theProcessor = processor;
	theTempo = -1;
	thePosition = 0;
	theRootNote = 0;
}

Sequencer::~Sequencer()
{
	delete theMidiCore;
}

void Sequencer::processBlock(AudioSampleBuffer& buffer, AudioPlayHead::CurrentPositionInfo& infos, MidiBuffer& messages)
{
	theTempo = infos.bpm;
	thePPQPosition = infos.ppqPosition;
	beat = (int)thePPQPosition + 1;
	step = 1 +  (int)(thePPQPosition / 0.25) % theProcessor->theSequencerLength;
	
	theStepTime = 1.0 / (theTempo / 60.0) * 250;
	
	
//	theSyncTime = theStepTime - ((mod / 0.25) * theStepTime);
	DBG("PPQ:" << thePPQPosition);
	DBG("Step:" << step);
	
}

void Sequencer::stop()
{
}

void Sequencer::repositionSequencer()
{
	

	DBG("SyncTime: " + String(theSyncTime));
	
}



