/*
  ==============================================================================

    MidiCore.cpp
    Created: 13 Mar 2013 11:57:22pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#include "MidiCore.h"


MidiCore::MidiCore():theOutput(0)
{
    theOutput=MidiOutput::openDevice(0);
    if(theOutput==nullptr)
    {
        AlertWindow::AlertIconType icon = AlertWindow::NoIcon;
        icon = AlertWindow::InfoIcon;
        AlertWindow::showMessageBoxAsync (icon,
                                          "Midi failed opening a port!",
                                          "Please create some midi buses before using the sequencer! It can be done via the Audio Midi Setup of your Mac...",
                                          "Got it!");
    }
}
void MidiCore::changeMidiOutput(int index)
{
    theOutput =MidiOutput::openDevice(index);
}

MidiCore::~MidiCore()
{
    killNotes();
}

int MidiCore::getNumberOfOutput()
{
    theOutput =MidiOutput::openDevice(0);
    if(theOutput==nullptr)
    {return 0;}
    return 1;
}
void MidiCore::midiNoteOn(int noteNumber,int velocity)
{
    Time theTime;
    const MidiMessage midiMessage(0x90,noteNumber,velocity,0);
    theOutput->sendMessageNow(midiMessage);
}
void MidiCore::midiNoteOff(int noteNumber)
{
    Time theTime;
    const MidiMessage midiMessage(0x80,noteNumber,127,0);
    theOutput->sendMessageNow(midiMessage);
}

void MidiCore::killNotes()
{
    theOutput->sendMessageNow(MidiMessage::allNotesOff(1));
    theOutput->sendMessageNow(MidiMessage::allSoundOff(1));
}