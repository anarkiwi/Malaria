#include <MozziGuts.h>
#include <mozzi_midi.h>
#include "malaria_operator.h"

class MalariaInstrument {
 public:
  virtual void noteOn(byte note, byte velocity);
  void noteOff(byte note, byte velocity);
  virtual void updateControl();
  virtual int updateAudio();
  bool isPlaying();
 protected:
  bool gate;
};

class SimpleFMPairInstrument : public MalariaInstrument {
 public:
  void noteOn(byte note, byte velocity);
  void updateControl();
  int updateAudio();
 protected:
  SimpleFMPair pair;
};

class FMBell : public SimpleFMPairInstrument {
 public:
  FMBell();
};

class FMBass : public SimpleFMPairInstrument {
 public:
  FMBass();
};
