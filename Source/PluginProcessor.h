/*
  ==============================================================================

    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Step.h"
#include "Sequencer.h"
class SequencerAudioProcessor  : public AudioProcessor
{
public:

    SequencerAudioProcessor();
    ~SequencerAudioProcessor();
	
	ValueTree& getValueTree(){return *theAudioConfig;}
	void setSequencerPosition(int newPos);
	int lastUIWidth, lastUIHeight;
	int theSequencerLength;
	int theSequencerPosition;
	AudioPlayHead::CurrentPositionInfo lastPosInfo;
	OwnedArray<Step> theSteps;
	OwnedArray<Step>& getSteps(){return theSteps;}
	enum Parameters
    {
		StepPitchs = 16,
		StepVelocities = 32,
		StepState = 48,
        SequencerLength,
		
        totalNumParams
    };
	
	ValueTree* theAudioConfig;
	UndoManager* theUndoManager;
	Sequencer* theSequencer;
	
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;
    const String getName() const;
    int getNumParameters();
    float getParameter (int index);
    void setParameter (int index, float newValue);
    const String getParameterName (int index);
    const String getParameterText (int index);
    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;
    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequencerAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
