# Malaria
Multiprocessing with Mozzi

Malaria connects two Arduino Unos (running Mozzi) together with a PCM bus,
distributing DSP work between them.  A third Arduino (not shown) receives
MIDI messages and controls parameters on the first two over a private MIDI bus.
Typically the first processor generates waveforms while the second executes filters
and other effects.

The PCM bus uses the following pins, to transmit 9 bit integers (8 + sign) from
processor 1 to 2:

  pins 2-7: least significant bits
     pin 8: clock
    pin 10: sign
pins 11-12: most significant bits

If hard synchronization is not required, the clock pin could be used for other
purposes (Eg Mozzi HiFi mode).


                     Audio output
                           +
                           |
                           |
                           |
                 +------------------------------+
                 |   1 1 1 |                    |
                 |   2 1 0 | 8  7 6 5 4 3 2   0 |
                 |   + + + + +  + + + + + +   +---------------+
                 |   | | | 9 |  | | | | | |     |             |
Arduino Mozzi    |   | | |   |  | | | | | |     |             |
processor #2     |   | | |   |  | | | | | |     |             |
                 |   | | |   |  | | | | | |     |             |
                 |   | | |   |  | | | | | |     |             +-------+ MIDI TX from
                 |   | | |   |  | | | | | |     |             |         Arduino controller
                 |   | | |   |  | | | | | |     |             |
                 +------------------------------+             |
                     | | |   |  | | | | | |                   |
                 +------------------------------+             |
                 |   + + +   +  + + + + + +   +---------------+
                 |   1 1 1   8  7 6 5 4 3 2   0 |
                 |   2 1 0                      |
                 |                              |
Arduino Mozzi    |                              |
processor #1     |                              |
                 |                              |
                 |                              |
                 |                              |
                 |                              |
                 +------------------------------+


The first Arduino runs Mozzi code like this (sending PCM samples
to the second):

  [...Mozzi headers...]

  #include <Malaria.h>
  #include <PinChangeInt.h>

  MalariaPcmWriter pcmWriter;

  [...]

  int updateAudio() {
    // write computed PCM sample to second Arduino
    pcmWriter.write(aNote.next());
    return 0;
  }

The second Arduino runs Mozzi code like this (samples arrive via
the PCM reader - note that audioHook() runs in the clock's ISR,
not loop(), where hard synchronization is required):

  [...Mozzi headers...]

  #include <Malaria.h>

  MalariaPcmReader pcmReader;

  [...]

  void setup() {
    startMozzi(CONTROL_RATE);
    [...]
    // clock ISR
    attachPinChangeInterrupt(8, pcmClock, CHANGE);
  }

  [...]

  void pcmClock() {
    // audioHook() now runs in clock's ISR, not loop().
    audioHook();
  }

  void loop() {
  }

  int updateAudio() {
    // samples arrive from first Arduino and can be
    // combined with locally computed samples.
    return (pcmReader.read() >> 1) + (aNote.next() >> 1);
  }
