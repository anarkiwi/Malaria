#include "malaria_instruments.h"

bool MalariaInstrument::isPlaying() {
  return gate;
}

void MalariaInstrument::noteOff(byte note, byte velocity) {
  gate = false;
}

FMBell::FMBell() : pair(5, 7, 4, 2000, 255) {
  gate = false;
}

void FMBell::noteOn(byte note, byte velocity) {
  float fundamentalHz = mtof(float(note));
  pair.noteOn(fundamentalHz, velocity);
  gate = true;
}

void FMBell::updateControl() {
  pair.updateControl();
}

int FMBell::updateAudio() {
  if (gate) {
    return pair.updateAudio();
  } else {
    return 0;
  }
}
