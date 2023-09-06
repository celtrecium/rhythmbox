#include "./Pattern.h"
#include "./samples.h"
#include "./Playback.h"

Pattern::Pattern(const char id)
  : id(id),
    tracks { 0, 0, 0 } {}

static const Playback::Sound instruments[] = {
  { KICK, sizeof(KICK), 100 },
  { SNARE, sizeof(SNARE), 100 },
  { HIHAT, sizeof(HIHAT), 100 },
  { SHAKER, sizeof(SHAKER), 100 },
  { RIM, sizeof(RIM), 100 },
  { CLAP, sizeof(CLAP), 100 },
  { COWBELL, sizeof(COWBELL), 100 },
};

void Pattern::playSingleBeat(const int i) {
  for(int j = 0; j < Pattern::tracksValue; ++j) {
    if(tracks[j] & (1 << i)) {
      Playback::add(instruments[j]);
    }
  }

  Playback::start();
}

void Pattern::play(const int bpm) {
  for(int i = 0; i < Pattern::trackLength; ++i) {
    playSingleBeat(i);
    delay(15000 / bpm);
  }
}