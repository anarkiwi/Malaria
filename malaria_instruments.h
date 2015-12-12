#include <MozziGuts.h>
#include <mozzi_midi.h>
#include "malaria_operator.h"

class MalariaInstrument {
 public:
  MalariaInstrument() {};
  virtual void noteOn(byte note, byte velocity);
  virtual void noteOff(byte note, byte velocity);
  virtual void updateControl();
  virtual int updateAudio();
  bool isPlaying();
 protected:
  bool gate;
};

class FMBell : public MalariaInstrument {
 public:
  FMBell();
  void noteOn(byte note, byte velocity);
  void noteOff(byte note, byte velocity);
  void updateControl();
  int updateAudio();
 private:
  MalariaOperator modulator;
  MalariaOperator carrier;
  Q16n16 deviation;
};
