#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>
#include <mozzi_midi.h>

#include <tables/sin2048_int8.h>

#define applyGain(v, g)  ((v * g) >> 8)


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
    aCarrier.setTable(SIN2048_DATA);
    aModulator.setTable(SIN2048_DATA);
    aCarrier.setFreq(0);
    aModulator.setFreq(0);
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
    carrierFreq = float_to_Q16n16(fundamentalHz * 5.f);
    modulatorFreq = float_to_Q16n16(fundamentalHz * 7.f);
    deviation = (modulatorFreq>>16) * mod_index;
    aModulator.setFreq_Q16n16(modulatorFreq);
    aCarrier.setFreq_Q16n16(carrierFreq);
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
      Q15n16 modulation = applyGain(deviation, applyGain(aModulator.next(), modEnv.next()));
      return applyGain(applyGain(aCarrier.phMod(modulation), carEnv.next()), gain);
    } else {
      return 0;
    }
  }
  bool isPlaying() {
    return gate;
  }
 private:
  const Q8n8 mod_index = float_to_Q8n8(1.0f);
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aCarrier;
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aModulator;
  ADSR <CONTROL_RATE, AUDIO_RATE> modEnv;
  ADSR <CONTROL_RATE, AUDIO_RATE> carEnv;
  Q16n16 carrierFreq, modulatorFreq, deviation;
  byte gain;
  bool gate;
};

