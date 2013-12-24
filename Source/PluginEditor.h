/*
  ==============================================================================

	Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class SequencerAudioProcessorEditor  : public AudioProcessorEditor, public SliderListener, ValueTree::Listener, ButtonListener, Timer
{
public:
    SequencerAudioProcessorEditor (SequencerAudioProcessor* ownerFilter);
    ~SequencerAudioProcessorEditor();

    void paint (Graphics& g);
	void sliderValueChanged(Slider* slider);
	void buttonClicked(Button* button);
	void resized();
	void timerCallback();
	void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
private:
	
	SequencerAudioProcessor* getProcessor() const
    {
        return static_cast <SequencerAudioProcessor*> (getAudioProcessor());
    }
	OwnedArray<Slider> theStepSliders;
	OwnedArray<Slider> theVelocitySliders;
	OwnedArray<ToggleButton> theStateButtons;
	ValueTree theAudioConfig;
	int thePosition;
};


#endif  // PLUGINEDITOR_H_INCLUDED
