#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/cos2048_int8.h>
#include <LowPassFilter.h>
#include <mozzi_rand.h>
#include <MIDI.h>
#include <mozzi_midi.h>
#include <Malaria.h>
#include <PinChangeInt.h>

const byte ledPin = 13;

Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kFilterMod(COS2048_DATA);

MIDI_CREATE_DEFAULT_INSTANCE();

LowPassFilter lpf;

MalariaPcmReader pcmReader;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  startMozzi(CONTROL_RATE);
  kFilterMod.setFreq(2.f);
  lpf.setResonance(200);
  attachPinChangeInterrupt(8, pcmClock, CHANGE);
  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void pcmClock() {
  audioHook();
}

void HandleNoteOn(byte channel, byte note, byte velocity) {
  int fundamentalHz = mtof(note);
  kFilterMod.setFreq_Q16n16(float_to_Q16n16(float(fundamentalHz >> 6)));
  digitalWrite(ledPin, HIGH);
}

void HandleNoteOff(byte channel, byte note, byte velocity) { 
  digitalWrite(ledPin, LOW);
}

void loop() {
  MIDI.read();
}

void updateControl() {
  byte cutoff_freq = 100 + kFilterMod.next()/2;
  lpf.setCutoffFreq(cutoff_freq);
}

int updateAudio() {
  return lpf.next(pcmReader.read() >> 2);
}
