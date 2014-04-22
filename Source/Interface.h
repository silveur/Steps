/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sequencer.h"

class Interface: public Component, Timer, SliderListener, ButtonListener
{
public:
    Interface(Sequencer* sequencer);
    ~Interface();

    void paint (Graphics&);
	void sliderValueChanged(Slider* slider);
    void resized();
	void buttonClicked(Button* button);
	void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
	void refreshMidiList();
	void timerCallback();
	
private:
	Rectangle<int> theMainScreen;
	ScopedPointer<ComboBox> theMidiOutputList;
	OwnedArray<Slider> theStepSliders;
	OwnedArray<Slider> theVelocitySliders;
	OwnedArray<ToggleButton> theStateButtons;
	Sequencer* theSequencer;
	int* thePosition;

};


#endif  // MAINCOMPONENT_H_INCLUDED
