/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CliffLp01filterEqAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener,
    public juce::Timer
{
public:
    CliffLp01filterEqAudioProcessorEditor (CliffLp01filterEqAudioProcessor&);
    ~CliffLp01filterEqAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

    void timerCallback() override;


private:
    juce::Label mFilterLowShelfLabel;
    juce::Slider mFilterLowShelfSlider;

    juce::Label mFilterLSQLabel;
    juce::Slider mFilterLSQSlider;

    juce::Label mFilterPeakLabel;
    juce::Slider mFilterPeakSlider;

    juce::Label mFilterPeakQLabel;
    juce::Slider mFilterPeakQSlider;

    juce::Label mFilterPeakGLabel;
    juce::Slider mFilterPeakGSlider;

    juce::Label mFilterHSLabel;
    juce::Slider mFilterHSSlider;

    juce::Label mFilterHSGLabel;
    juce::Slider mFilterHSGSlider;

    juce::Label mFilterLPLabel;
    juce::Slider mFilterLPSlider;

    juce::Label mFilterLPQLabel;
    juce::Slider mFilterLPQSlider;

    juce::Label mFilterHPLabel;
    juce::Slider mFilterHPSlider;

    juce::Label mFilterHPQLabel;
    juce::Slider mFilterHPQSlider;

    juce::Slider mHPSwitch;
    juce::Label mHPSwitchL;

    juce::Slider mLPSwitch;
    juce::Label mLPSwitchL;

    juce::Label mVolumeLabel;
    juce::Slider mVolumeSlider;

    //juce::TextButton LowPass{ "On" };

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CliffLp01filterEqAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CliffLp01filterEqAudioProcessorEditor)
};
