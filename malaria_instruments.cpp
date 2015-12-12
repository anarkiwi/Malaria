#include "malaria_instruments.h"

bool MalariaInstrument::isPlaying() {
  return gate;
}

void MalariaInstrument::noteOff(byte note, byte velocity) {
  gate = false;
}

void SimpleFMPairInstrument::noteOn(byte note, byte velocity) {
  float fundamentalHz = mtof(float(note));
  pair.noteOn(fundamentalHz, velocity);
  gate = true;
}

void SimpleFMPairInstrument::updateControl() {
  pair.updateControl();
}

int SimpleFMPairInstrument::updateAudio() {
  if (gate) {
    return pair.updateAudio();
  } else {
    return 0;
  }
}

FMBell::FMBell() {
  pair.setParameters(5.f, 7.f, MIN_ADRS_MS, 2000, 255);
}

FMBass::FMBass() {
  pair.setParameters(1.f, 2.f, MIN_ADRS_MS, 100, 255);
}
