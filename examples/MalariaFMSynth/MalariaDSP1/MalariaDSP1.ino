#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include <ADSR.h>
#include <mozzi_midi.h>
#include <MIDI.h>
#include <Malaria.h>

const byte ledPin = 13;
const Q8n8 mod_index = float_to_Q8n8(1.0f);

Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aCarrier(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aModulator(SIN2048_DATA);
ADSR <CONTROL_RATE, AUDIO_RATE> modEnv;
ADSR <CONTROL_RATE, AUDIO_RATE> carEnv;

MIDI_CREATE_DEFAULT_INSTANCE();

Q16n16 carrierFreq, modulatorFreq, deviation;
MalariaPcmWriter pcmWriter;

bool gate = false;
byte gain = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  startMozzi(CONTROL_RATE);
  aCarrier.setFreq(0);
  aModulator.setFreq(0);
  modEnv.setIdleLevel(0);
  modEnv.setLevels(255, 0, 0, 0);
  modEnv.setTimes(20, 2000, 0, 0);
  carEnv.setIdleLevel(0);
  carEnv.setLevels(255, 0, 0, 0);
  carEnv.setTimes(20, 2000, 0, 0);
  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  audioHook();
}

void HandleNoteOn(byte channel, byte note, byte velocity) {
  float fundamentalHz = mtof(float(note));
  carrierFreq = float_to_Q16n16(fundamentalHz * 5.f);
  modulatorFreq = float_to_Q16n16(fundamentalHz * 7.f);
  deviation = (modulatorFreq>>16) * mod_index;
  aModulator.setFreq_Q16n16(modulatorFreq);
  aCarrier.setFreq_Q16n16(carrierFreq);
  modEnv.noteOn();
  carEnv.noteOn();
  gain = velocity;
  gate = true;
  digitalWrite(ledPin, HIGH);
}

void HandleNoteOff(byte channel, byte note, byte velocity) { 
  gate = false;
  digitalWrite(ledPin, LOW);
}

void updateControl() {
  MIDI.read();
  modEnv.update();
  carEnv.update();
}

#define APPLY_GAIN(v, g)  ((v * g) >> 8)

int updateAudio() {
  if (gate) {
    Q15n16 modulation = APPLY_GAIN(deviation, APPLY_GAIN(aModulator.next(), modEnv.next()));
    pcmWriter.write(APPLY_GAIN(APPLY_GAIN(aCarrier.phMod(modulation), carEnv.next()), gain));
  } else {
    pcmWriter.write(0);
  }
  return 0;
}

