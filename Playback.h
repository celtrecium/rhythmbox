#pragma once
#include <Arduino.h>

#define SAMPLE_RATE 8000

namespace Playback {
  struct Sound {
    byte *data;
    int size;
    int position;
    byte volume;

    Sound();
    Sound(unsigned char const *data, int size, byte volume=100);

    Sound &operator=(const Sound &sample);
  };

  constexpr int maxListLength { 5 };
  void add(unsigned char const *data, int length);
  void add(const Sound &Sound);
  void start();
  void stop();
  void initialize();
  void terminate();
}
