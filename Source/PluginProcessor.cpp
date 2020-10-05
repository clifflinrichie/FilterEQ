/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CliffLp01filterEqAudioProcessor::CliffLp01filterEqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // LowShelf filter
    addParameter(mFilterLowShelfParam = new juce::AudioParameterFloat("LowShelf Freq (Hz)", // parameterID,
        "LSFiltFc", // parameterName,
        40.0f, // minValue,
        22000.0f, // maxValue,
        500.0f)); // defaultValue

    // Low Shelf Gain
    addParameter(mFilterLSQParam = new juce::AudioParameterFloat("LowShelf Gain", // parameterID,
        "LSGain", // parameterName,
        -60.0f, // minValue,
        12.0f, // maxValue,
        0.0f)); // defaultValue

    // Midrange peak filter
    addParameter(mFilterPeakParam = new juce::AudioParameterFloat("Midrange", //param id
        "MidPeak", // param name
        20.0f, // min val
        22000.0f, // max
        2000.0f)); // default

    // Midrange peak Q
    addParameter(mFilterPQParam = new juce::AudioParameterFloat("MidPeak Q", //param id
        "PeakQ", // param name
        1.0f, // min val
        15.0f, // max
        3.0f)); // default

    // Midrange gain
    addParameter(mFilterPGainParam = new juce::AudioParameterFloat("MidPeak Gain",
        "PeakGain",
        -60.0f, // min
        12.0f, // max
        0.0f)); // default
    
    // HiShelf filter
    addParameter(mFilterHighShelfParam = new juce::AudioParameterFloat("HighShelf Freq. (Hz)", // parameterID,
        "HighFc", // parameterName,
        40.0f, // minValue,
        22000.0f, // maxValue,
        10000.0f)); // defaultValue

    // High Shelf Gain
    addParameter(mFilterHSGParam = new juce::AudioParameterFloat("HighShelf Gain", // parameterID,
        "HSGain", // parameterName,
        -60.0f, // minValue,
        12.0f, // maxValue,
        0.0f)); // defaultValue

    // Master Volume
    addParameter(mVolumeParam = new juce::AudioParameterFloat("Volume",
        "Gain",
        -60.0f,
        10.0f,
        1.0f));

    // LowPass filter
    addParameter(mFilterLPParam = new juce::AudioParameterFloat("LowPass", //param id
        "LP", // param name
        20.0f, // min val
        22000.0f, // max
        500.0f)); // default

    // LowPass Q
    addParameter(mFilterLPQParam = new juce::AudioParameterFloat("LowPass Q", //param id
        "LPQ", // param name
        1.0f, // min val
        15.0f, // max
        3.0f)); // default

    // HighPass filter
    addParameter(mFilterHPParam = new juce::AudioParameterFloat("Highpass", //param id
        "HP", // param name
        20.0f, // min val
        22000.0f, // max
        500.0f)); // default

    // HighPass Q
    addParameter(mFilterHPQParam = new juce::AudioParameterFloat("HighPass Q", //param id
        "HPQ", // param name
        1.0f, // min val
        15.0f, // max
        3.0f)); // default

    // onSwitch High Pass
    addParameter(mOnSwitch = new juce::AudioParameterFloat("OnS",
        "HP Switch",
        0.0f,
        1.0f,
        0.0f));

    // onSwitch LowPAss
    addParameter(mLPOnSwitch = new juce::AudioParameterFloat("LP On",
        "LP Switch",
        0.0f,
        1.0f,
        0.0f));
}

CliffLp01filterEqAudioProcessor::~CliffLp01filterEqAudioProcessor()
{
}

//==============================================================================
const juce::String CliffLp01filterEqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CliffLp01filterEqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CliffLp01filterEqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CliffLp01filterEqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CliffLp01filterEqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CliffLp01filterEqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CliffLp01filterEqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CliffLp01filterEqAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CliffLp01filterEqAudioProcessor::getProgramName (int index)
{
    return {};
}

void CliffLp01filterEqAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CliffLp01filterEqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mFs = sampleRate;
    mGainLinear = 1.0f;
    LPisOn = false;
    HPisOn = false;
}

void CliffLp01filterEqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CliffLp01filterEqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CliffLp01filterEqAudioProcessor::calcAlgorithmParams() {
    // 1.
    // Get user parameters for LowShelf
    float LSfc = mFilterLowShelfParam->get();
    float LSQ = mFilterLSQParam->get();

    // calculating lowshelf
    float LSCoeffs[5]; // an array of 5 floats for filter coeffs: [b0, b1, b2, a1, a2]
    Mu45FilterCalc::calcCoeffsLowShelf(LSCoeffs, LSfc, LSQ, mFs); // coeffs, fc, gain, fs

    // Set the coefficients for each filter
    mLSFilterL.setCoefficients(LSCoeffs[0], LSCoeffs[1], LSCoeffs[2], LSCoeffs[3], LSCoeffs[4]);
    mLSFilterR.setCoefficients(LSCoeffs[0], LSCoeffs[1], LSCoeffs[2], LSCoeffs[3], LSCoeffs[4]);

    // 2. 
    // setting up a gain value first
    //mPeakGainLinear = pow(10, (mFilterPGainParam->get()) / 20);
    float peakGain = mFilterPGainParam->get();

    // user parameters for midrange 
    float MidFc = mFilterPeakParam->get();
    float MidQ = mFilterPQParam->get();

    float MidCoeffs[5]; 
    Mu45FilterCalc::calcCoeffsPeak(MidCoeffs, MidFc, peakGain, MidQ, mFs); // coeffs, hz, gain, q, samplerate

    mPeakFilterL.setCoefficients(MidCoeffs[0], MidCoeffs[1], MidCoeffs[2], MidCoeffs[3], MidCoeffs[4]);
    mPeakFilterR.setCoefficients(MidCoeffs[0], MidCoeffs[1], MidCoeffs[2], MidCoeffs[3], MidCoeffs[4]);

    // 3. Get user parameters for HighShelf
    float HSfc = mFilterHighShelfParam->get();
    float HSG = mFilterHSGParam->get();

    // calculating lowshelf
    float HSCoeffs[5]; // an array of 5 floats for filter coeffs: [b0, b1, b2, a1, a2]
    Mu45FilterCalc::calcCoeffsHighShelf(HSCoeffs, HSfc, HSG, mFs); // coeffs, fc, gain, fs

    // Set the coefficients for each filter
    mHSFilterL.setCoefficients(HSCoeffs[0], HSCoeffs[1], HSCoeffs[2], HSCoeffs[3], HSCoeffs[4]);
    mHSFilterR.setCoefficients(HSCoeffs[0], HSCoeffs[1], HSCoeffs[2], HSCoeffs[3], HSCoeffs[4]);

    // 4. Setting up a master volume
    mGainLinear = pow(10, (mVolumeParam->get()) / 20);

    // 5. Creating LowPass
    float LPfc = mFilterLPParam->get();
    float LPQ = mFilterLPQParam->get();

    // calculating lowshelf
    float LPFCoeffs[5]; // an array of 5 floats for filter coeffs: [b0, b1, b2, a1, a2]
    Mu45FilterCalc::calcCoeffsLPF(LPFCoeffs, LPfc, LPQ, mFs); // coeffs, fc, gain, fs

    // Set the coefficients for each filter
    mLPFilterL.setCoefficients(LPFCoeffs[0], LPFCoeffs[1], LPFCoeffs[2], LPFCoeffs[3], LPFCoeffs[4]);
    mLPFilterR.setCoefficients(LPFCoeffs[0], LPFCoeffs[1], LPFCoeffs[2], LPFCoeffs[3], LPFCoeffs[4]);

    // 6. Creating HighPass
    float HPfc = mFilterHPParam->get();
    float HPQ = mFilterHPQParam->get();

    // calculating lowshelf
    float HPFCoeffs[5]; // an array of 5 floats for filter coeffs: [b0, b1, b2, a1, a2]
    Mu45FilterCalc::calcCoeffsHPF(HPFCoeffs, HPfc, HPQ, mFs); // coeffs, fc, gain, fs

    // Set the coefficients for each filter
    mHPFilterL.setCoefficients(HPFCoeffs[0], HPFCoeffs[1], HPFCoeffs[2], HPFCoeffs[3], HPFCoeffs[4]);
    mHPFilterR.setCoefficients(HPFCoeffs[0], HPFCoeffs[1], HPFCoeffs[2], HPFCoeffs[3], HPFCoeffs[4]);

    

    //DBG("Switch = "<< HPisOn); // For debugging
}

void CliffLp01filterEqAudioProcessor::SwitchCheck() {
    // switch test
    float HPCheck = mOnSwitch->get();
    if (HPCheck > 0.0) { HPisOn = true; }
    else  { HPisOn = false; }
}

void CliffLp01filterEqAudioProcessor::LPSwitchCheck() {
    // switch test
    float LPCheck = mLPOnSwitch->get();
    if (LPCheck > 0.0) { LPisOn = true; }
    else { LPisOn = false; }
}

void CliffLp01filterEqAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Update the algorithm params
    calcAlgorithmParams();
    SwitchCheck();
    LPSwitchCheck();

    // Get the left and right audio buffers
    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);

    
    for (int samp = 0; samp < buffer.getNumSamples(); samp++)
    {
        // Process each audio sample
        if (HPisOn && LPisOn) {
            channelDataLeft[samp] = mHPFilterL.tick(channelDataLeft[samp]);
            channelDataRight[samp] = mHPFilterR.tick(channelDataRight[samp]);
            
            channelDataLeft[samp] = mLPFilterL.tick(channelDataLeft[samp]);
            channelDataRight[samp] = mLPFilterR.tick(channelDataRight[samp]);
        }
        else if (HPisOn) {
            channelDataLeft[samp] = mHPFilterL.tick(channelDataLeft[samp]);
            channelDataRight[samp] = mHPFilterR.tick(channelDataRight[samp]);
        }
        else if (LPisOn) {
            channelDataLeft[samp] = mLPFilterL.tick(channelDataLeft[samp]);
            channelDataRight[samp] = mLPFilterR.tick(channelDataRight[samp]);
        }

        else {
            channelDataLeft[samp] = mLSFilterL.tick(channelDataLeft[samp]);
            channelDataRight[samp] = mLSFilterR.tick(channelDataRight[samp]);

            channelDataLeft[samp] = mPeakFilterL.tick(channelDataLeft[samp]);
            channelDataRight[samp] = mPeakFilterR.tick(channelDataRight[samp]);

            channelDataLeft[samp] = mHSFilterL.tick(channelDataLeft[samp]);
            channelDataRight[samp] = mHSFilterR.tick(channelDataRight[samp]);
            
        }
        
        SwitchCheck();
        LPSwitchCheck();

        channelDataLeft[samp] = mGainLinear * channelDataLeft[samp];
        channelDataRight[samp] = mGainLinear * channelDataRight[samp];
    }
}

//==============================================================================
bool CliffLp01filterEqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CliffLp01filterEqAudioProcessor::createEditor()
{
    return new CliffLp01filterEqAudioProcessorEditor (*this);
}

//==============================================================================
void CliffLp01filterEqAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CliffLp01filterEqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CliffLp01filterEqAudioProcessor();
}
