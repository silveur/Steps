/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SequencerAudioProcessor::SequencerAudioProcessor()
{
	for(int i=0;i<NUM_CHANNELS_MAX;i++)
	{
		theSteps.add(new Step());
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

float SequencerAudioProcessor::getParameter (int index)
{
	switch (index)
	{
		case SequencerLength:	return theSequencerLength;
		default:				return 0.0f;
	}
}

void SequencerAudioProcessor::setParameter (int index, float newValue)
{
	switch (index)
    {
        case SequencerLength:	theSequencerLength = newValue;  break;
        default:												break;
    }
}

const String SequencerAudioProcessor::getParameterName (int index)
{
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SequencerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	
    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            lastUIWidth  = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);
			
            theSequencerLength  = (float) xmlState->getDoubleAttribute ("Lengh", theSequencerLength);

        }
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SequencerAudioProcessor();
}
