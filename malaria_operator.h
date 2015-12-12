#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>

#include <tables/sin2048_int8.h>
#include <tables/cos2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/triangle2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>

#define applyGain(v, g)	((v * g) >> 8)
#define OSCIL_CELLS	2048


class MalariaOperator {
 public:
  MalariaOperator();
  void updateControl();
  int next();
  int phMod(Q15n16 modulation);
  Q15n16 getPhMod(Q16n16 deviation);
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
