/*
 * Malaria.h
 *
 * Copyright 2015 Josh Bailey
 *
 * This file is part of Malaria.
 *
 * Mozzi is licensed under the MIT License.
 *
 */

class MalariaPcmWriter {
 public:
  MalariaPcmWriter();
  void write(int pcm);
};

class MalariaPcmReader {
 public:
  MalariaPcmReader();
  int read();
};
