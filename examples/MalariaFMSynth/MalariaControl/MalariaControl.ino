#include <SoftwareSerial.h>
#include <MIDI.h>

const byte txPin = 2;
const byte rxPin = 3;
const byte ledPin = 13;

SoftwareSerial midiTxSerial(rxPin, txPin);
MIDI_CREATE_INSTANCE(SoftwareSerial, midiTxSerial, midiTx);
MIDI_CREATE_DEFAULT_INSTANCE();

void HandleNoteOn(byte channel, byte note, byte velocity) {
  midiTx.sendNoteOn(note, velocity, channel);
  digitalWrite(ledPin, HIGH);
}

void HandleNoteOff(byte channel, byte note, byte velocity) {
  midiTx.sendNoteOff(note, velocity, channel);
  digitalWrite(ledPin, LOW);
}

void setup() {
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(ledPin, OUTPUT);
  midiTx.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  MIDI.read();
}
