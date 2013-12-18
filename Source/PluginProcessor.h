/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_71B73F6C__
#define __PLUGINPROCESSOR_H_71B73F6C__

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiCore.h"
//==============================================================================
/**
 */
class SequencerAuAudioProcessor  : public AudioProcessor//,Synthesiser
{
public:
    //==============================================================================
    SequencerAuAudioProcessor();
    ~SequencerAuAudioProcessor();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    void reset();
    //void handleMidiEvent(const MidiMessage& m);
    
    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;
    
    //==============================================================================
    const String getName() const;
    
    int getNumParameters();
    void timerCallback();
    float getParameter (int index);
    void setParameter (int index, float newValue);
    double getTailLengthSeconds() const;
    
    const String getParameterName (int index);
    const String getParameterText (int index);
    
    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;
    
    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    
    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    
    AudioPlayHead::CurrentPositionInfo lastPosInfo;
    MidiKeyboardState keyboardState;
    int lastUIWidth, lastUIHeight;
    int sampleCount;
    
    void init();
    void setBPM(double newBPM);
    void setShuffle(float value);
    void killNotes();
    void rootKeyChanged(int newRootKey);
    void transposeChanged(int newTranspose);
    void stopSequence();
    void setNumberStep(int value);
    void setMidiOutputDevice();
    void flush();
    void newChord(Array<int> newArray);
    int getStep();
    double shuffle;
    
    int rootKey;
    bool isRunning;
    int sequencerCount;
    
    int inc;
    int previousStep;
    int newStep;
    
    int sequencerPosition;
    
    int numSteps;
    int midiOutputIndex;
    
    //==============================================================================
    enum Parameters
    {
        gainParam = 0,
        delayParam,
        
        totalNumParams
    };
    
    float gain, delay;
    AudioSampleBuffer delayBuffer;
    Synthesiser synth;
    double theSampleRate;
    
    MidiCore* theMidiCore;
    int noteValues[16];
    int velocities[16];
    bool theOnOffArray[16];
    Array<int> currentArray;
    int transpose;
    
    
private:
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequencerAuAudioProcessor)
};

#endif  // __PLUGINPROCESSOR_H_71B73F6C__
