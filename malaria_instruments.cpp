#include "malaria_instruments.h"

bool MalariaInstrument::isPlaying() {
  return gate;
}

void MalariaInstrument::noteOff(byte note, byte velocity) {
  gate = false;
}

FMBell::FMBell() {
  carrier.setEnvLevels(255, 0, 0, 0);
  carrier.setEnvTimes(20, 2000, 0, 0);
  carrier.setGain(0);
  modulator.setEnvLevels(255, 0, 0, 0);
  modulator.setEnvTimes(20, 2000, 0, 0);
  modulator.setGain(255);
  gate = false;
}

void FMBell::noteOn(byte note, byte velocity) {
  float fundamentalHz = mtof(float(note));
  Q16n16 carrierFreq = float_to_Q16n16(fundamentalHz * 5.f);
  carrier.setFreq_Q16n16(carrierFreq);
  Q16n16 modulatorFreq = float_to_Q16n16(fundamentalHz * 7.f);
  modulator.setFreq_Q16n16(modulatorFreq);
  carrier.setGain(velocity);
  carrier.noteOn();
  modulator.noteOn();
  gate = true;
}

void FMBell::updateControl() {
  modulator.updateControl();
  carrier.updateControl();
}

int FMBell::updateAudio() {
  if (gate) {
    return carrier.phMod(modulator.getPhMod(255));
  } else {
    return 0;
  }
}
