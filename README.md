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

