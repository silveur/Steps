/*
  ==============================================================================

	MidiCore.h
	Created: 13 Mar 2013 11:57:22pm
	Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef __MIDICORE_H_8D40ACBD__
#define __MIDICORE_H_8D40ACBD__

#include "../JuceLibraryCode/JuceHeader.h"

class MidiCore
{
public:
	MidiCore();
	~MidiCore();
	
	void noteOn(int noteNumber,uint8 velocity, int channel);
	void noteOff(int noteNumber);
	void killNotes();
	void outputMidi(const MidiMessage& msg);
	void openMidiOutput(String& name);
	void outputMidi(const MidiMessage &msg, int delayMs);
	void setChannel(int channel);
	StringArray getMidiDevicesList();
	
private:

	MidiOutput* theMidiOutput;
};

#endif  // __MIDICORE_H_8D40ACBD__
