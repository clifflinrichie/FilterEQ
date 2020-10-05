/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "StkLite-4.6.1/BiQuad.h"


//==============================================================================
/**
*/
class CliffLp01filterEqAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    CliffLp01filterEqAudioProcessor();
    ~CliffLp01filterEqAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::AudioParameterFloat* mFilterLowShelfParam;
    juce::AudioParameterFloat* mFilterLSQParam; // is actually gain, Low shelf has no Q parameter

    juce::AudioParameterFloat* mFilterPeakParam;
    juce::AudioParameterFloat* mFilterPQParam;
    juce::AudioParameterFloat* mFilterPGainParam;

    juce::AudioParameterFloat* mFilterHighShelfParam;
    juce::AudioParameterFloat* mFilterHSGParam;

    juce::AudioParameterFloat* mFilterLPParam;
    juce::AudioParameterFloat* mFilterLPQParam;

    juce::AudioParameterFloat* mFilterHPParam;
    juce::AudioParameterFloat* mFilterHPQParam;

    juce::AudioParameterFloat* mVolumeParam;

    juce::AudioParameterFloat* mOnSwitch;
    juce::AudioParameterFloat* mLPOnSwitch;

    // Mu45: Objects we need
    stk::BiQuad mLSFilterL, mLSFilterR; // The filters
    stk::BiQuad mPeakFilterL, mPeakFilterR;
    stk::BiQuad mHSFilterL, mHSFilterR; 
    stk::BiQuad mLPFilterL, mLPFilterR;
    stk::BiQuad mHPFilterL, mHPFilterR;
    

    float mFs; // Sampling rate
    float mGainLinear; // midrange gain

    bool LPisOn;
    bool HPisOn;

    void SwitchCheck();
    void LPSwitchCheck();
    void calcAlgorithmParams();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CliffLp01filterEqAudioProcessor)
};
