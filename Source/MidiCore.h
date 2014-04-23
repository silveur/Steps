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
	MidiCore(String& midiOut);
	MidiCore();
	~MidiCore();
	
	void noteOn(int noteNumber,int velocity);
	void noteOff(int noteNumber);
	void killNotes();
	void outputMidi(const MidiMessage& msg);
	void openMidiOutput(String& name);
	void outputMidi(const MidiMessage &msg, int delayMs);
	StringArray getMidiDevicesList();
private:
	
	MidiOutput* theMidiOutput;
};




#endif  // __MIDICORE_H_8D40ACBD__
