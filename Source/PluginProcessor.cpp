/*
  ==============================================================================

    Author:  Silvere Letellier

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void copyTree(ValueTree& destinationTree,ValueTree& sourceTree);

SequencerAudioProcessor::SequencerAudioProcessor()
{
	theAudioConfig = new ValueTree("AudioConfig");
	theUndoManager = new UndoManager(1000, 1);
    theMidiCore = new MidiCore();
    theMidiCore->openMidiOutput(0);
	theSequencer = new Sequencer(this, theMidiCore);
	theSequencerLength =NUM_CHANNELS_MAX;
	theSequencerPosition = 0;
	isPlaying = false;
	for(int i=0;i<NUM_CHANNELS_MAX;i++)
	{
		ValueTree StepTree("Step" + String(i));
		StepTree.setProperty("Pitch", 0, theUndoManager);
		StepTree.setProperty("Velocity", 127, theUndoManager);
		StepTree.setProperty("State", 0, theUndoManager);
		theAudioConfig->addChild(StepTree, i, theUndoManager);
		theSteps.add(new Step(StepTree));
	}
}

SequencerAudioProcessor::~SequencerAudioProcessor()
{
	delete theAudioConfig;
	delete theUndoManager;
	delete theSequencer;
}

const String SequencerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

void SequencerAudioProcessor::setSequencerPosition(int newPos)
{
	theSequencerPosition = newPos;
}

int SequencerAudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float SequencerAudioProcessor::getParameter(int index)
{
	if(index < NUM_CHANNELS_MAX)
	{
		return theSteps[index]->thePitch;
	}
	else if(index >= NUM_CHANNELS_MAX && index < NUM_CHANNELS_MAX*2)
	{
		return theSteps[index % NUM_CHANNELS_MAX]->theVelocity;
	}
	else if(index >= NUM_CHANNELS_MAX*2 && index < NUM_CHANNELS_MAX*3)
	{
		return theSteps[index % NUM_CHANNELS_MAX]->theState;
	}
	switch (index)
	{
		case SequencerLength:	return theSequencerLength;
        case MidiOutputIndex:	return theMidiOutputIndex;
		default:				return 0.0f;
	}
}

void SequencerAudioProcessor::setParameter(int index, float newValue)
{
	if(index < NUM_CHANNELS_MAX)
	{
		theSteps[index]->thePitch = newValue;
	}
	else if(index >= NUM_CHANNELS_MAX && index < NUM_CHANNELS_MAX*2)
	{
		theSteps[index % NUM_CHANNELS_MAX]->theVelocity = newValue;
	}
	else if(index >= NUM_CHANNELS_MAX*2 && index < NUM_CHANNELS_MAX*3)
	{
		theSteps[index % NUM_CHANNELS_MAX]->theState = newValue;
	}
	else
	{
		switch (index)
		{
			case SequencerLength:	theSequencerLength = newValue;  break;
            case MidiOutputIndex:
            {
                theMidiOutputIndex = newValue;
                theMidiCore->openMidiOutput(theMidiOutputIndex);
                break;
            }
			default:												break;
		}
	}
}

const String SequencerAudioProcessor::getParameterName (int index)
{
	if(index < NUM_CHANNELS_MAX)
	{
		return "Step" + String(index % NUM_CHANNELS_MAX) + "pitch";
	}
	else if(index >= NUM_CHANNELS_MAX && index < NUM_CHANNELS_MAX*2)
	{
		return "Step" + String(index % NUM_CHANNELS_MAX) + "Velocity";
	}
	else if(index >= NUM_CHANNELS_MAX*2 && index < NUM_CHANNELS_MAX*3)
	{
		return "Step" + String(index % NUM_CHANNELS_MAX) + "State";
	}
    switch (index)
    {
        case SequencerLength:     return "Length";
        case MidiOutputIndex:     return "MidiOutputIndex";
        default:					break;
    }
	
    return String::empty;
}

const String SequencerAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

const String SequencerAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String SequencerAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool SequencerAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool SequencerAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool SequencerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SequencerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SequencerAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double SequencerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SequencerAudioProcessor::getNumPrograms()
{
    return 0;
}

int SequencerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SequencerAudioProcessor::setCurrentProgram (int index)
{
}

const String SequencerAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void SequencerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void SequencerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	theMidiCollector.reset(sampleRate);
}

void SequencerAudioProcessor::releaseResources()
{
	theSequencer->stop();
}

void SequencerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{	
	getPlayHead()->getCurrentPosition (lastPosInfo);
	if (!isPlaying && lastPosInfo.isPlaying)
	{
		isPlaying = true;
		theSequencer->start();
		DBG("Block size:" + String(buffer.getNumSamples()));
	}
	else if(lastPosInfo.isPlaying && isPlaying)
	{
		
		isPlaying = true;
		theSequencer->setPosition(lastPosInfo);
	}
	else if(!lastPosInfo.isPlaying)
	{
		isPlaying = false;
		theSequencer->stop();
	}
    for (int i = 0; i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
}

bool SequencerAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* SequencerAudioProcessor::createEditor()
{
    return new SequencerAudioProcessorEditor (this);
}

void SequencerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	for(int i=0;i<NUM_CHANNELS_MAX;i++)
	{
		theAudioConfig->getChild(i).setProperty("Pitch", theSteps[i]->thePitch, theUndoManager);
		theAudioConfig->getChild(i).setProperty("Velocity", theSteps[i]->theVelocity, theUndoManager);
		theAudioConfig->getChild(i).setProperty("State", theSteps[i]->theState, theUndoManager);
	}
	theAudioConfig->setProperty("Length", theSequencerLength, theUndoManager);
	MemoryOutputStream rawStream(destData, false);
	theAudioConfig->writeToStream(rawStream);
}

void SequencerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	ValueTree treeToRead(ValueTree::readFromData(data, sizeInBytes));
	copyTree(*theAudioConfig, treeToRead);
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SequencerAudioProcessor();
}

inline void copyTree(ValueTree& destinationTree,ValueTree& sourceTree)
{
    destinationTree.copyPropertiesFrom(sourceTree, 0);
    const int numChildren = sourceTree.getNumChildren();
    for (int i = 0; i < numChildren; ++i)
    {
		String sourceTreeChildName = (String)sourceTree.getChild(i).getType();
		String destTreeChildName = (String)destinationTree.getChild(i).getType();
		ValueTree sourceChild = sourceTree.getChild(i);
        ValueTree destinationChild = destinationTree.getChild(i);
        if(destinationChild.isValid() && sourceChild.isValid())
		{
            destinationChild.copyPropertiesFrom(sourceChild, 0);
			if(sourceChild.getNumChildren()!=0)
				copyTree(destinationChild,sourceChild);
		}
    }
}
