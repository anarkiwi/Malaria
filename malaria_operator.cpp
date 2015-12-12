#include "malaria_operator.h"

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
