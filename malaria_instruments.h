#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>
#include <mozzi_midi.h>

#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/triangle2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>

#define applyGain(v, g)	((v * g) >> 8)
#define OSCIL_CELLS	2048


class MalariaInstrument {
 public:
  MalariaInstrument() {};
  virtual void noteOn(byte note, byte velocity);
  virtual void noteOff(byte note, byte velocity);
  virtual void updateControl();
  virtual int updateAudio();
  virtual bool isPlaying();
 protected:
};


class FMBell : public MalariaInstrument {
 public:
  FMBell() {
    carrier.setTable(SIN2048_DATA);
    modulator.setTable(SIN2048_DATA);
    carrier.setFreq(0);
    modulator.setFreq(0);
    modEnv.setIdleLevel(0);
    modEnv.setLevels(255, 0, 0, 0);
    modEnv.setTimes(20, 2000, 0, 0);
    carEnv.setIdleLevel(0);
    carEnv.setLevels(255, 0, 0, 0);
    carEnv.setTimes(20, 2000, 0, 0);
    gate = false;
    gain = 0;
  }
  void noteOn(byte note, byte velocity) {
    float fundamentalHz = mtof(float(note));
    Q16n16 carrierFreq = float_to_Q16n16(fundamentalHz * 5.f);
    carrier.setFreq_Q16n16(carrierFreq);
    Q16n16 modulatorFreq = float_to_Q16n16(fundamentalHz * 7.f);
    deviation = (modulatorFreq>>16) * float_to_Q8n8(1.0f);
    modulator.setFreq_Q16n16(modulatorFreq);
    modEnv.noteOn();
    carEnv.noteOn();
    gate = true;
    gain = velocity;
  }
  void noteOff(byte note, byte velocity) {
    gate = false;
  }
  void updateControl() {
    modEnv.update();
    carEnv.update();
  }
  int updateAudio() {
    if (gate) {
      Q15n16 modulation = applyGain(deviation, applyGain(modulator.next(), modEnv.next()));
      return applyGain(applyGain(carrier.phMod(modulation), carEnv.next()), gain);
    } else {
      return 0;
    }
  }
  bool isPlaying() {
    return gate;
  }
 private:
  Oscil<OSCIL_CELLS, AUDIO_RATE> carrier;
  Oscil<OSCIL_CELLS, AUDIO_RATE> modulator;
  ADSR <CONTROL_RATE, AUDIO_RATE> modEnv;
  ADSR <CONTROL_RATE, AUDIO_RATE> carEnv;
  Q16n16 deviation;
  byte gain;
  bool gate;
};
