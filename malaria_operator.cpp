#include "malaria_operator.h"

#define applyGain(v, g) ((v * g) >> 8)


MalariaOperator::MalariaOperator() {
  osc.setTable(SIN2048_DATA);
  osc.setFreq(0);
  env.setIdleLevel(0);
  setGain(0);
};

void MalariaOperator::updateControl() {
  env.update();
}

int MalariaOperator::next() {
  return applyGain(applyGain(osc.next(), env.next()), gain);
}

int MalariaOperator::phMod(Q15n16 modulation) {
  return applyGain(applyGain(osc.phMod(modulation), env.next()), gain);
}

Q15n16 MalariaOperator::getPhMod(byte intensity) {
  return applyGain(applyGain((Q15n16) intensity * osc.next(), env.next()), gain);
}

void MalariaOperator::setEnvLevels(byte a, byte d, byte s, byte r) {
  env.setLevels(a, d, s, r);
}

void MalariaOperator::setEnvTimes(int a, int d, int s, int r) {
  env.setTimes(a, d, s, r);
}

void MalariaOperator::setGain(byte g) {
  gain = g;
} 

void MalariaOperator::setFreq(int f) {
  osc.setFreq(f);
}

void MalariaOperator::setFreq_Q16n16(Q16n16 f) {
  osc.setFreq_Q16n16(f);
}

void MalariaOperator::noteOn() {
  env.noteOn();
}

void SimpleFMPair::setParameters(float c_ratio, float m_ratio, int a_ms, int d_ms, byte mod_gain) {
  carrier_ratio = c_ratio;
  modulator_ratio = m_ratio;
  carrier.setEnvLevels(255, 0, 0, 0);
  carrier.setEnvTimes(a_ms, d_ms, 0, 0);
  carrier.setGain(0);
  modulator.setEnvLevels(255, 20, 0, 0);
  modulator.setEnvTimes(a_ms, d_ms, 0, 0);
  modulator.setGain(mod_gain);
}

void SimpleFMPair::noteOn(float fundamentalHz, byte car_gain) {
  Q16n16 carrierFreq = float_to_Q16n16(fundamentalHz * carrier_ratio);
  carrier.setFreq_Q16n16(carrierFreq);
  Q16n16 modulatorFreq = float_to_Q16n16(fundamentalHz * modulator_ratio);
  modulator.setFreq_Q16n16(modulatorFreq);
  carrier.setGain(car_gain);
  carrier.noteOn();
  modulator.noteOn();
}

void SimpleFMPair::updateControl() {
  modulator.updateControl();
  carrier.updateControl();
}

int SimpleFMPair::updateAudio() {
  return carrier.phMod(modulator.getPhMod(255));
}
