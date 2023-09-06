#pragma once
#include <Arduino.h>

class Pattern {
  public:
    const char id;
    static const int tracksValue { 7 };
    static const int trackLength { 16 };
    int16_t tracks[tracksValue];

    Pattern(const char id);
    void play(const int bpm);
    void playSingleBeat(const int i);
};