/*
  ==============================================================================

	Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class SequencerAudioProcessorEditor  : public AudioProcessorEditor, public SliderListener, ValueTree::Listener, ButtonListener, Timer, ComboBoxListener
{
public:
    SequencerAudioProcessorEditor (SequencerAudioProcessor* ownerFilter);
    ~SequencerAudioProcessorEditor();

    void paint (Graphics& g);
	void sliderValueChanged(Slider* slider);
	void buttonClicked(Button* button);
	void resized();
	void timerCallback();
	void refreshMidiList();
	void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
private:
	
	SequencerAudioProcessor* getProcessor() const
    {
        return static_cast <SequencerAudioProcessor*> (getAudioProcessor());
    }
	ScopedPointer<TextButton> theCreateMidiPortButton;
	OwnedArray<Slider> theStepSliders;
	OwnedArray<Slider> theVelocitySliders;
	ScopedPointer<Slider> theSequencerLength;
	OwnedArray<ToggleButton> theStateButtons;
    ScopedPointer<ComboBox> theMidiOutputList;
	ValueTree theAudioConfig;
	int thePosition;
};


#endif  // PLUGINEDITOR_H_INCLUDED
