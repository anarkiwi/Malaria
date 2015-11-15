#include <MozziGuts.h>
#include <MIDI.h>
#include <Malaria.h>
#include <malaria_instruments.h>

const byte ledPin = 13;

MIDI_CREATE_DEFAULT_INSTANCE();
FMBell instrument;
MalariaPcmWriter pcmWriter;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  startMozzi(CONTROL_RATE);
  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  audioHook();
}

void HandleNoteOn(byte channel, byte note, byte velocity) {
  instrument.noteOn(note, velocity);
  digitalWrite(ledPin, HIGH);
}

void HandleNoteOff(byte channel, byte note, byte velocity) { 
  instrument.noteOff(note, velocity);
  digitalWrite(ledPin, LOW);
}

void updateControl() {
  MIDI.read();
  instrument.updateControl();
}

int updateAudio() {
  pcmWriter.write(instrument.updateAudio());
  return 0;
}

