/*
 * Malaria.cpp
 *
 * Copyright 2015 Josh Bailey
 *
 * This file is part of Malaria.
 *
 * Malaria is licensed under the MIT License.
 *
 */

#include "Arduino.h"
#include "Malaria.h"

const byte clockMask = B00000001; // pin 8
const byte signMask = B00000100; // pin 10
const byte msbMask = B00011000; // pins 11 and 12

const byte ddrbOutMask = B00011101; // set 8, 10-12 to output
const byte ddrdOutMask = B11111100; // set 2-7 to output

MalariaPcmWriter::MalariaPcmWriter() {
  DDRB |= ddrbOutMask;
  PORTB = PINB & ~ddrbOutMask;
  DDRD |= ddrdOutMask; 
  PORTD = PIND & ~ddrdOutMask;
}

void MalariaPcmWriter::write(int pcm) {
  byte pos_pcm = lowByte(abs(pcm));
  byte msb = (pos_pcm & B11000000) >> 3;
  if (pcm < 0) {
    msb |= signMask;
  } 
  // Write LSBs (6 bits)
  PORTD = (PIND & B00000011) | (pos_pcm << 2);
  // Write MSBs (2 bits), sign bit, and clock
  PORTB = (((PINB & ~(msbMask | signMask)) ^ clockMask)) | msb;
}

MalariaPcmReader::MalariaPcmReader() {
  DDRB &= ~ddrbOutMask;
  PORTB = PINB & ddrbOutMask;
  DDRD &= ~ddrdOutMask;
  PORTD = PIND & ddrdOutMask;
}

int MalariaPcmReader::read() {
  int pcm = (PIND >> 2) + ((PINB & msbMask) << 3);
  if (PINB & signMask) {
    pcm = -pcm;
  }
  return pcm;
}
