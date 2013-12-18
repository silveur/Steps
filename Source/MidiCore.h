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
    MidiCore(int index);
    ~MidiCore();
    
    void midiNoteOn(int noteNumber,int velocity);
    void midiNoteOff(int noteNumber);
    
    int getNumberOfOutput();
    
    void changeMidiOutput(int index);
    
    void killNotes();
private:
    
    ScopedPointer<MidiOutput> theOutput;
};




#endif  // __MIDICORE_H_8D40ACBD__
