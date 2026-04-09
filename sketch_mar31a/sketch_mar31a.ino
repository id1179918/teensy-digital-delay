#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <algorithm>
#include <iterator>


#define fisrtUnusedPin 2
#define lastUnusedPin 41

#define inputVolumePotPin   A2 // 16
#define outputVolumePotPin  A3 // 17
#define delayFeedbackPotPin A5 // 19
#define dryWetPotPin        A7 // 21
#define delayTimePotPin     A6 // 20
#define filterDryWetPotPin  A8 // 22

// INPUTS
AudioInputUSB            usb1;           //xy=59,104
//AudioInputAnalog         adc1;           //xy=61,64
//AudioPlayMemory          playMem1;       //xy=66,20

// INPUT MIXER
AudioMixer4              inputMixer;         //xy=276,66

// PRE-FILTER MIXER
AudioMixer4              preFilterMixer;         //xy=584,108

// FILTER (HIGH PASS)
AudioFilterStateVariable filter2;        //xy=614,219

// POST-FILTER MIXER
AudioMixer4              postFilterMixer;         //xy=833,251

// DELAY UNIT
AudioEffectDelay         delayModule;         //xy=969,138

// OUTPUT & FEEDBACK MIXER
AudioMixer4              outputMixer; //xy=1120,35

// OUTPUTS
//AudioOutputAnalog        dac1;           //xy=1325,95
AudioOutputUSB           usb2;           //xy=1326,28

// UNIT CONNECTIONS
AudioConnection          patchCord1(usb1, 0, inputMixer, 2);
AudioConnection          patchCord2(usb1, 1, inputMixer, 3);
//AudioConnection          patchCord3(adc1, 0, inputMixer, 1);
//AudioConnection          patchCord4(playMem1, 0, inputMixer, 0);

AudioConnection          patchCord5(inputMixer, 0, preFilterMixer, 0);
AudioConnection          patchCord7(inputMixer, 0, outputMixer, 1);

AudioConnection          patchCord10(preFilterMixer, 0, filter2, 0);
AudioConnection          patchCord11(preFilterMixer, 0, postFilterMixer, 0);
AudioConnection          patchCord12(filter2, 2, postFilterMixer, 1);
AudioConnection          patchCord13(postFilterMixer, delayModule);

AudioConnection          patchCord14(delayModule, 0, outputMixer, 0);
AudioConnection          patchCord15(outputMixer, 0, usb2, 0);
AudioConnection          patchCord16(outputMixer, 0, usb2, 1);
//AudioConnection          patchCord17(outputMixer, dac1);
AudioConnection          patchCord18(outputMixer, 0, preFilterMixer, 2);

int usedPins[7] = {
  inputVolumePotPin,
  outputVolumePotPin,
  dryWetPotPin,
  delayFeedbackPotPin,
  delayTimePotPin,
  filterDryWetPotPin
};

void setup() {
  Serial.begin(140000);
  AudioMemory(400);

  for(int i = fisrtUnusedPin; i <= lastUnusedPin; i++) {
    if (std::find(std::begin(usedPins), std::end(usedPins), i) == std::end(usedPins)) {
      pinMode(i, INPUT_DISABLE);
    }
  }

  inputMixer.gain(0, 0.0); // PlayBackMem input
  inputMixer.gain(1, 0.0); // ADC input
  inputMixer.gain(2, 0.0); // USB-Left input
  inputMixer.gain(3, 0.0); // USB-Right

  outputMixer.gain(0, 0.0); // delayModule input
  outputMixer.gain(1, 0.0); // direct signal input to output

  preFilterMixer.gain(0, 0.0); // direct signal input
  preFilterMixer.gain(1, 0.0); // input from Hold
  preFilterMixer.gain(2, 0.0); // return signal from outputMixer

  postFilterMixer.gain(0, 0.0); // direct signal input
  postFilterMixer.gain(1, 0.0); // filtered signal input

  delayModule.delay(0, 20.0); // 20 mS delay
  delayModule.disable(1);
  delayModule.disable(2);
  delayModule.disable(3);
  delayModule.disable(4);
  delayModule.disable(5);
  delayModule.disable(6);
  delayModule.disable(7);

  AudioInterrupts();

}

void loop() {
  float inputLevel = analogRead(inputVolumePotPin);
  float outputLevel = analogRead(outputVolumePotPin);
  float dryWetLevel = analogRead(dryWetPotPin);
  float delayFeedbackLevel = analogRead(delayFeedbackPotPin);
  float delayTime = map(analogRead(delayTimePotPin), 0, 1023, 20, 2413);
  float filterLevel = analogRead(filterDryWetPotPin);

  Serial.printf("inputLevel: %f/1023\n", inputLevel);
  Serial.printf("outputLevel: %f/1023\n", outputLevel);
  Serial.printf("dryWetLevel: %f/1023\n", dryWetLevel);
  Serial.printf("delayFeedbackLevel: %f/1023\n", delayFeedbackLevel);
  Serial.printf("delayTime: %f mS\n", delayTime);
  Serial.printf("filterLevel: %f/1023\n\n", filterLevel);


  delayModule.delay(0, delayTime);


  delay(1000);
}
