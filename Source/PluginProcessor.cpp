/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic startup code for a Juce application.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SequencerAuAudioProcessor::SequencerAuAudioProcessor():delayBuffer (2, 12000)
{
    // Set up some default values..
    gain = 1.0f;
    delay = 0.5f;
    sampleCount=0;
    lastUIWidth = 400;
    lastUIHeight = 200;
    theSampleRate=0;
    lastPosInfo.resetToDefault();
    previousStep =0;
    numSteps=16;
    rootKey = 0;
    transpose=0;
    theMidiCore = new MidiCore();
    isRunning=false;
    currentArray.clear();
    shuffle=0.2;
    newStep=0;
    midiOutputIndex=0;
    for(int i=0;i<16;i++)
    {
        noteValues[i]=0;
        velocities[i]=127;
        theOnOffArray[i]=true;
    }
    setMidiOutputDevice();
}

SequencerAuAudioProcessor::~SequencerAuAudioProcessor()
{
    flush();
    deleteAndZero(theMidiCore);
}

//==============================================================================
const String SequencerAuAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int SequencerAuAudioProcessor::getNumParameters()
{
    return 0;
}

float SequencerAuAudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:     return gain;
        case delayParam:    return delay;
        default:            return 0.0f;
    }
}

void SequencerAuAudioProcessor::setParameter (int index, float newValue)
{
    switch (index)
    {
        case gainParam:     gain = newValue;  break;
        case delayParam:    delay = newValue;  break;
        default:            break;
    }
}

const String SequencerAuAudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case gainParam:     return "gain";
        case delayParam:    return "delay";
        default:            break;
    }
    
    return String::empty;
}

const String SequencerAuAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

const String SequencerAuAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String SequencerAuAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool SequencerAuAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool SequencerAuAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool SequencerAuAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SequencerAuAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SequencerAuAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

int SequencerAuAudioProcessor::getNumPrograms()
{
    return 0;
}

int SequencerAuAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SequencerAuAudioProcessor::setCurrentProgram (int index)
{
}

const String SequencerAuAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void SequencerAuAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SequencerAuAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    keyboardState.reset();
    delayBuffer.clear();
    theSampleRate=sampleRate;
}

void SequencerAuAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}
void SequencerAuAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    delayBuffer.clear();
}

void SequencerAuAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    AudioPlayHead::CurrentPositionInfo newTime;
    getPlayHead()->getCurrentPosition (newTime);
    
    const int numSamples = buffer.getNumSamples();
//    MidiBuffer::Iterator midiIterator (midiMessages);
//    midiIterator.setNextSamplePosition (0);
//    MidiMessage m (0xf4, 0.0);
//    
//    while (numSamples > 0)
//    {
//        int midiEventPos;
//        //const bool useEvent = midiIterator.getNextEvent (m, midiEventPos) && midiEventPos < 0 + numSamples;
//        
//        //if (useEvent)
//            //handleMidiEvent (m);
//    }
//    
    
    
    double modulo=0.25;
    
    if(newTime.isPlaying==false)
    {
        flush();
        previousStep = -1;
        inc=0;
        sequencerPosition=0;
        sequencerCount=0;
    }
    else if(newTime.isPlaying==true)
    {
             getPlayHead()->getCurrentPosition (newTime);
             double quarterNotes = newTime.ppqPosition;
             int step = (int)4*modf(quarterNotes, &modulo);
             //DBG(String(step));
             DBG(String(quarterNotes));
             newStep = step;

             if(newStep!=previousStep)
             {
                 int noteValue = (60+(12*transpose)+rootKey+(noteValues[sequencerPosition]));
                 flush();
                 if(theOnOffArray[sequencerPosition]==true)
                 {
                     theMidiCore->midiNoteOn(noteValue,velocities[sequencerPosition]);
                     currentArray.add(noteValue);
                 }
                 previousStep=newStep;
                 sequencerCount++;
                 sequencerPosition = sequencerCount%numSteps;
             }
    }
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
}
//void SequencerAuAudioProcessor::handleMidiEvent(const juce::MidiMessage &m)
//{
//    if (m.isNoteOn())
//    {
//        this->rootKey=m.getNoteNumber();
//    }
//}
int SequencerAuAudioProcessor::getStep()
{
    return sequencerPosition;
}
void SequencerAuAudioProcessor::setMidiOutputDevice()
{
    theMidiCore->changeMidiOutput(midiOutputIndex);
}
void SequencerAuAudioProcessor::init()
{
    previousStep=0;
}
void SequencerAuAudioProcessor::rootKeyChanged(int newRootKey)
{
    rootKey=newRootKey;
}
void SequencerAuAudioProcessor::transposeChanged(int newTranspose)
{
    transpose=newTranspose;
}
void SequencerAuAudioProcessor::killNotes()
{
    theMidiCore->killNotes();
}
void SequencerAuAudioProcessor::stopSequence()
{
    // stopTimer(MAIN_TIMER);
    ////theSequencer->isRunning=false;
    this->flush();
}
void SequencerAuAudioProcessor::setBPM(double newBPM)
{
    //  computedBPM = (1.0/(((newBPM/60.0)*4.0)))*1000.0;
}
void SequencerAuAudioProcessor::setShuffle(float value)
{
    //shuffle = theSequencer->getShuffle();
}
void SequencerAuAudioProcessor::newChord(Array<int> newArray)
{
    currentArray.clear();
    currentArray.swapWithArray(newArray);
}
void SequencerAuAudioProcessor::flush()
{
    for(int i=0;i<currentArray.size();i++)
    {
        theMidiCore->midiNoteOff(currentArray.getUnchecked(i));
        currentArray.remove(i);
    }
}


//==============================================================================
bool SequencerAuAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SequencerAuAudioProcessor::createEditor()
{
    return new PluginEditor (this);
}

//==============================================================================
void SequencerAuAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    XmlElement xml ("MYPLUGINSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);
    xml.setAttribute ("gain", gain);
    xml.setAttribute ("delay", delay);
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void SequencerAuAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            lastUIWidth  = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);
            
            gain  = (float) xmlState->getDoubleAttribute ("gain", gain);
            delay = (float) xmlState->getDoubleAttribute ("delay", delay);
        }
    }
}
double SequencerAuAudioProcessor::getTailLengthSeconds() const
{
    return 1;
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SequencerAuAudioProcessor();
}
