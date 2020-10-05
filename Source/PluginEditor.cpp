/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CliffLp01filterEqAudioProcessorEditor::CliffLp01filterEqAudioProcessorEditor (CliffLp01filterEqAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (850, 500);
    // Setup your sliders and other gui components - - - -
    auto& params = processor.getParameters();

    // LowShelf Freq Slider
    juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
    mFilterLowShelfSlider.setBounds(25, 25, 100, 100); // x, y, width, height (in pixels)
    mFilterLowShelfSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterLowShelfSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25); // readOnly,w,h

    mFilterLowShelfSlider.setRange(audioParam->range.start, audioParam->range.end, 0.1f);
    mFilterLowShelfSlider.setValue(*audioParam);
    mFilterLowShelfSlider.setSkewFactorFromMidPoint(2000.0);

    mFilterLowShelfSlider.setTextValueSuffix(" Hz");
    mFilterLowShelfSlider.addListener(this);
    addAndMakeVisible(mFilterLowShelfSlider);

    mFilterLowShelfLabel.setText("Low Shelf", juce::dontSendNotification);
    mFilterLowShelfLabel.attachToComponent(&mFilterLowShelfSlider, false);
    mFilterLowShelfLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterLowShelfLabel);

    // Gain slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(1);
    mFilterLSQSlider.setBounds(25, 175, 100, 100);
    mFilterLSQSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterLSQSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    mFilterLSQSlider.setRange(audioParam->range.start, audioParam->range.end, 0.001f);
    mFilterLSQSlider.setValue(*audioParam);
    mFilterLSQSlider.setSkewFactorFromMidPoint(0.0);
    mFilterLSQSlider.addListener(this);
    addAndMakeVisible(mFilterLSQSlider);

    mFilterLSQLabel.setText("Low Gain", juce::dontSendNotification);
    mFilterLSQLabel.attachToComponent(&mFilterLSQSlider, false);
    mFilterLSQLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterLSQLabel);

    // MidRange Freq Slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(2);
    mFilterPeakSlider.setBounds(150, 25, 100, 100); // x, y, width, height (in pixels)
    mFilterPeakSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterPeakSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25); // readOnly,w,h

    mFilterPeakSlider.setRange(audioParam->range.start, audioParam->range.end, 0.1f);
    mFilterPeakSlider.setValue(*audioParam);
    mFilterPeakSlider.setSkewFactorFromMidPoint(2000.0);

    mFilterPeakSlider.setTextValueSuffix(" Hz");
    mFilterPeakSlider.addListener(this);
    addAndMakeVisible(mFilterPeakSlider);

    mFilterPeakLabel.setText("Mid Range", juce::dontSendNotification);
    mFilterPeakLabel.attachToComponent(&mFilterPeakSlider, false);
    mFilterPeakLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterPeakLabel);

    // MidRange Q Slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(3);
    mFilterPeakQSlider.setBounds(150, 325, 100, 100);
    mFilterPeakQSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterPeakQSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    mFilterPeakQSlider.setRange(audioParam->range.start, audioParam->range.end, 0.001f);
    mFilterPeakQSlider.setValue(*audioParam);
    mFilterPeakQSlider.addListener(this);
    addAndMakeVisible(mFilterPeakQSlider);

    mFilterPeakQLabel.setText("Mid Q", juce::dontSendNotification);
    mFilterPeakQLabel.attachToComponent(&mFilterPeakQSlider, false);
    mFilterPeakQLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterPeakQLabel);

    // MidRange Gain Slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(4);
    mFilterPeakGSlider.setBounds(150, 175, 100, 100);
    mFilterPeakGSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterPeakGSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    mFilterPeakGSlider.setRange(audioParam->range.start, audioParam->range.end, 0.001f);
    mFilterPeakGSlider.setValue(*audioParam);
    mFilterPeakGSlider.setSkewFactorFromMidPoint(0.0);
    mFilterPeakGSlider.addListener(this);
    addAndMakeVisible(mFilterPeakGSlider);

    mFilterPeakGLabel.setText("Mid Gain", juce::dontSendNotification);
    mFilterPeakGLabel.attachToComponent(&mFilterPeakGSlider, false);
    mFilterPeakGLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterPeakGLabel);

    // HighShelf Freq Slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(5);
    mFilterHSSlider.setBounds(275, 25, 100, 100); // x, y, width, height (in pixels)
    mFilterHSSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterHSSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25); // readOnly,w,h

    mFilterHSSlider.setRange(audioParam->range.start, audioParam->range.end, 0.1f);
    mFilterHSSlider.setValue(*audioParam);
    mFilterHSSlider.setSkewFactorFromMidPoint(2000.0);

    mFilterHSSlider.setTextValueSuffix(" Hz");
    mFilterHSSlider.addListener(this);
    addAndMakeVisible(mFilterHSSlider);

    mFilterHSLabel.setText("High Shelf", juce::dontSendNotification);
    mFilterHSLabel.attachToComponent(&mFilterHSSlider, false);
    mFilterHSLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterHSLabel);

    // High Shelf Gain Slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(6);
    mFilterHSGSlider.setBounds(275, 175, 100, 100);
    mFilterHSGSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterHSGSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    mFilterHSGSlider.setRange(audioParam->range.start, audioParam->range.end, 0.001f);
    mFilterHSGSlider.setValue(*audioParam);
    mFilterHSGSlider.setSkewFactorFromMidPoint(0.0);
    mFilterHSGSlider.addListener(this);
    addAndMakeVisible(mFilterHSGSlider);

    mFilterHSGLabel.setText("High Gain", juce::dontSendNotification);
    mFilterHSGLabel.attachToComponent(&mFilterHSGSlider, false);
    mFilterHSGLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterHSGLabel);

    // Low Pass Slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(8);
    mFilterLPSlider.setBounds(450, 25, 100, 100); // x, y, width, height (in pixels)
    mFilterLPSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterLPSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25); // readOnly,w,h

    mFilterLPSlider.setRange(audioParam->range.start, audioParam->range.end, 0.1f);
    mFilterLPSlider.setValue(*audioParam);
    mFilterLPSlider.setSkewFactorFromMidPoint(2000.0);

    mFilterLPSlider.setTextValueSuffix(" Hz");
    mFilterLPSlider.addListener(this);
    addAndMakeVisible(mFilterLPSlider);

    mFilterLPLabel.setText("Low Pass", juce::dontSendNotification);
    mFilterLPLabel.attachToComponent(&mFilterLPSlider, false);
    mFilterLPLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterLPLabel);

    // Low Pass Q
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(9);
    mFilterLPQSlider.setBounds(450, 175, 100, 100);
    mFilterLPQSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterLPQSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    mFilterLPQSlider.setRange(audioParam->range.start, audioParam->range.end, 0.001f);
    mFilterLPQSlider.setValue(*audioParam);
    mFilterLPQSlider.addListener(this);
    addAndMakeVisible(mFilterLPQSlider);

    mFilterLPQLabel.setText("Low Pass Q", juce::dontSendNotification);
    mFilterLPQLabel.attachToComponent(&mFilterLPQSlider, false);
    mFilterLPQLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterLPQLabel);

    // High Pass Slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(10);
    mFilterHPSlider.setBounds(575, 25, 100, 100); // x, y, width, height (in pixels)
    mFilterHPSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterHPSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25); // readOnly,w,h

    mFilterHPSlider.setRange(audioParam->range.start, audioParam->range.end, 0.1f);
    mFilterHPSlider.setValue(*audioParam);
    mFilterHPSlider.setSkewFactorFromMidPoint(2000.0);

    mFilterHPSlider.setTextValueSuffix(" Hz");
    mFilterHPSlider.addListener(this);
    addAndMakeVisible(mFilterHPSlider);

    mFilterHPLabel.setText("High Pass", juce::dontSendNotification);
    mFilterHPLabel.attachToComponent(&mFilterHPSlider, false);
    mFilterHPLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterHPLabel);

    // High Pass Q
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(11);
    mFilterHPQSlider.setBounds(575, 175, 100, 100);
    mFilterHPQSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mFilterHPQSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    mFilterHPQSlider.setRange(audioParam->range.start, audioParam->range.end, 0.001f);
    mFilterHPQSlider.setValue(*audioParam);
    mFilterHPQSlider.addListener(this);
    addAndMakeVisible(mFilterHPQSlider);

    mFilterHPQLabel.setText("High Pass Q", juce::dontSendNotification);
    mFilterHPQLabel.attachToComponent(&mFilterHPQSlider, false);
    mFilterHPQLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mFilterHPQLabel);

    // Volume slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(7);
    mVolumeSlider.setBounds(700, 25, 100, 400); // x, y, width, height (in pixels)
    mVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25); // readOnly,w,h

    mVolumeSlider.setRange(audioParam->range.start, audioParam->range.end, 0.01f);
    mVolumeSlider.setValue(*audioParam);
    mVolumeSlider.setSkewFactorFromMidPoint(1.0);
    mVolumeSlider.setTextValueSuffix(" dB");
    mVolumeSlider.addListener(this);
    addAndMakeVisible(mVolumeSlider);

    mVolumeLabel.setText("Master Volume", juce::dontSendNotification);
    mVolumeLabel.attachToComponent(&mVolumeSlider, false);
    mVolumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mVolumeLabel);

    // low pass switch (values 0 or 1)
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(13);
    mLPSwitch.setBounds(450, 300, 100, 50); // x, y, width, height (in pixels)
    mLPSwitch.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mLPSwitch.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25); // readOnly,w,h

    mLPSwitch.setRange(audioParam->range.start, audioParam->range.end, 1.0f);
    mLPSwitch.setValue(*audioParam);

    mLPSwitch.addListener(this);
    addAndMakeVisible(mLPSwitch);

    mLPSwitchL.setText("Low Pass Switch", juce::dontSendNotification);
    mLPSwitchL.attachToComponent(&mLPSwitch, false);
    mLPSwitchL.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mLPSwitchL);

    // high pass switch (values 0 or 1)
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(12);
    mHPSwitch.setBounds(575, 300, 100, 50); // x, y, width, height (in pixels)
    mHPSwitch.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mHPSwitch.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25); // readOnly,w,h

    mHPSwitch.setRange(audioParam->range.start, audioParam->range.end, 1.0f);
    mHPSwitch.setValue(*audioParam);

    mHPSwitch.addListener(this);
    addAndMakeVisible(mHPSwitch);

    mHPSwitchL.setText("High Pass Switch", juce::dontSendNotification);
    mHPSwitchL.attachToComponent(&mHPSwitch, false);
    mHPSwitchL.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mHPSwitchL);

    startTimer(100);
}

CliffLp01filterEqAudioProcessorEditor::~CliffLp01filterEqAudioProcessorEditor()
{
}

//==============================================================================
void CliffLp01filterEqAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
   
}

void CliffLp01filterEqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void CliffLp01filterEqAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    auto& params = processor.getParameters();

    if (slider == &mFilterLowShelfSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = mFilterLowShelfSlider.getValue(); // set the AudioParameter
        DBG("Low Shelf Slider Changed");
    }
    else if (slider == &mFilterLSQSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(1);
        *audioParam = mFilterLSQSlider.getValue(); // set the param
        DBG("LSQ Slider Changed");
    }

    else if (slider == &mFilterPeakSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(2);
        *audioParam = mFilterPeakSlider.getValue(); // set the AudioParameter
        DBG("MidRange Peak Slider Changed");
    }
    else if (slider == &mFilterPeakQSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(3);
        *audioParam = mFilterPeakQSlider.getValue(); // set the param
        DBG("PeakQ Slider Changed");
    }
    else if (slider == &mFilterPeakGSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(4);
        *audioParam = mFilterPeakGSlider.getValue(); // set the param
        DBG("Peak Gain Slider Changed");
    }
    else if (slider == &mFilterHSSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(5);
        *audioParam = mFilterHSSlider.getValue(); // set the param
        DBG("High Shelf Slider Changed");
    }
    else if (slider == &mFilterHSGSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(6);
        *audioParam = mFilterHSGSlider.getValue(); // set the param
        DBG("HS Gain Changed");
    }
    else if (slider == &mFilterLPSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(8);
        *audioParam = mFilterLPSlider.getValue(); // set the param
        DBG("LP Freq Changed ");
    }
    else if (slider == &mFilterLPQSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(9);
        *audioParam = mFilterLPQSlider.getValue(); // set the param
        DBG("LPQ Changed ");
    }
    else if (slider == &mFilterHPSlider) 
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(10);
        *audioParam = mFilterHPSlider.getValue(); // set the param
        DBG("HP Freq Changed ");
    }
    else if (slider == &mFilterHPQSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(11);
        *audioParam = mFilterHPQSlider.getValue(); // set the param
        DBG("HPQ Changed ");
    }
    else if (slider == &mHPSwitch)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(12);
        *audioParam = mHPSwitch.getValue(); // set the param
        DBG("HP Switch Changed ");
    }
    else if (slider == &mLPSwitch)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(13);
        *audioParam = mLPSwitch.getValue(); // set the param
        DBG("LP Switch Changed ");
    }
    else if (slider == &mVolumeSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(7);
        *audioParam = mVolumeSlider.getValue(); // set the param
        DBG("Volume Changed");
    };
}

void CliffLp01filterEqAudioProcessorEditor::timerCallback() {
    auto& params = processor.getParameters();

    //Update the value of each slider to match the value in the Processor
    juce::AudioParameterFloat* VolumeParam = (juce::AudioParameterFloat*)params.getUnchecked(7);
    mVolumeSlider.setValue(VolumeParam->get(), juce::dontSendNotification);
}
