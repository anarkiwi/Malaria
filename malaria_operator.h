#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>

#include <tables/sin2048_int8.h>
#include <tables/cos2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/triangle2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>

#define	CONTROL_RATE	256
#define MIN_ADRS_MS	(int(1e3 / CONTROL_RATE) + 1)

#define applyGain(v, g)	((v * g) >> 8)
#define OSCIL_CELLS	2048


class MalariaOperator {
 public:
  MalariaOperator();
  void updateControl();
  int next();
  int phMod(Q15n16 modulation);
  Q15n16 getPhMod(byte intensity);
  void setEnvLevels(byte a, byte d, byte s, byte r);
  void setEnvTimes(int a, int d, int s, int r);
  void setGain(byte g);
  void setFreq(int f);
  void setFreq_Q16n16(Q16n16 f);
  void noteOn();
 protected:
  Oscil<OSCIL_CELLS, AUDIO_RATE> osc;
  ADSR <CONTROL_RATE, AUDIO_RATE> env;
  byte gain;
};

class SimpleFMPair {
 public:
  SimpleFMPair(float c_ratio, float m_ratio, int a_ms, int d_ms, byte mod_gain) {
    carrier_ratio = c_ratio;
    modulator_ratio = m_ratio;
    carrier.setEnvLevels(255, 0, 0, 0);
    carrier.setEnvTimes(a_ms, d_ms, 0, 0);
    carrier.setGain(0);
    modulator.setEnvLevels(255, 20, 0, 0);
    modulator.setEnvTimes(a_ms, d_ms, 0, 0);
    modulator.setGain(mod_gain);
  }
  void noteOn(float fundamentalHz, byte car_gain) {
    Q16n16 carrierFreq = float_to_Q16n16(fundamentalHz * carrier_ratio);
    carrier.setFreq_Q16n16(carrierFreq);
    Q16n16 modulatorFreq = float_to_Q16n16(fundamentalHz * modulator_ratio);
    modulator.setFreq_Q16n16(modulatorFreq);
    carrier.setGain(car_gain);
    carrier.noteOn();
    modulator.noteOn();
  }
  void updateControl() {
    modulator.updateControl();
    carrier.updateControl();
  }
  int updateAudio() {
    return carrier.phMod(modulator.getPhMod(255));
  }
 protected:
   MalariaOperator carrier;
   MalariaOperator modulator;
   float carrier_ratio;
   float modulator_ratio;
};
