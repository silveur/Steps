/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void copyTree(ValueTree& destinationTree,ValueTree& sourceTree);

SequencerAudioProcessor::SequencerAudioProcessor()
{
	theAudioConfig = new ValueTree("AudioConfig");
	theUndoManager = new UndoManager(1000, 1);
	theSequencerLength =NUM_CHANNELS_MAX;
	for(int i=0;i<NUM_CHANNELS_MAX;i++)
	{
		ValueTree StepTree("Step" + String(i));
		StepTree.setProperty("Pitch", 0, theUndoManager);
		StepTree.setProperty("Velocity", 127, theUndoManager);
		StepTree.setProperty("State", 1, theUndoManager);
		theAudioConfig->addChild(StepTree, i, theUndoManager);
		theSteps.add(new Step(StepTree));
	}
}

SequencerAudioProcessor::~SequencerAudioProcessor()
{
}

//==============================================================================
const String SequencerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int SequencerAudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float SequencerAudioProcessor::getParameter(int index)
{
	if(index < 16)
	{
		return theSteps[index]->thePitch;
	}
	else if(index >= 16 && index < 24)
	{
		return theSteps[index % 16]->theVelocity;
	}
	else if(index >= 24 && index < 32)
	{
		return theSteps[index % 16]->theState;
	}
	switch (index)
	{
		case SequencerLength:	return theSequencerLength;
		default:				return 0.0f;
	}
}

void SequencerAudioProcessor::setParameter(int index, float newValue)
{
	if(index < 16)
	{
		theSteps[index]->thePitch = newValue;
		theAudioConfig->getChild(index).setProperty("Pitch", newValue, theUndoManager);
	}
	else if(index >= 16 && index < 24)
	{
		theSteps[index]->theVelocity = newValue;
	}
	else if(index >= 24 && index < 32)
	{
		theSteps[index]->theState = newValue;
	}
	
	switch (index)
    {
        case SequencerLength:	theSequencerLength = newValue;  break;
        default:												break;
    }
}

const String SequencerAudioProcessor::getParameterName (int index)
{
	if(index < 16)
	{
		return "Step" + String(index % 16) + "pitch";
	}
	else if(index >= 16 && index < 24)
	{
		return "Step" + String(index % 16) + "Velocity";
	}
	else if(index >= 24 && index < 32)
	{
		return "Step" + String(index % 16) + "State";
	}
    switch (index)
    {
        case SequencerLength:     return "Length";
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

//==============================================================================
void SequencerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SequencerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void SequencerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getSampleData (channel);

        // ..do something to the data...
    }

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool SequencerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SequencerAudioProcessor::createEditor()
{
    return new SequencerAudioProcessorEditor (this);
}

//==============================================================================
void SequencerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	MemoryOutputStream rawStream(destData, false);
	theAudioConfig->writeToStream(rawStream);
}

void SequencerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	ValueTree treeToRead(ValueTree::readFromData(data, sizeInBytes));
	copyTree(*theAudioConfig, treeToRead);
}

//==============================================================================
// This creates new instances of the plugin..
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
