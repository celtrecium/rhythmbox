#pragma once
#include <Arduino.h>
#include "./Selector.h"
#include "./Sequencer.h"
#include "./Hardware.h"

class BPMSelector : public Selector {
  public:
    BPMSelector(const Hardware *hardware, const int currentBPM);
};

class PatternSelector : public Selector {
  private:
    const Pattern *patterns;

  public: 
    PatternSelector(const Hardware *hardware, const Pattern *patterns, const int patternsValue);
    void printItem() final override;
};

class MainScreenSelector : public Selector {
  public:
    MainScreenSelector(const Hardware *hardware);
    void printItem() final override;
};

class Rhythmbox {
  public:
    int bpm;
    Hardware hardware;
    Sequencer sequencer;
    BPMSelector bpmSelector;
    PatternSelector patternSelector;
    MainScreenSelector mainScreenSelector;

    Rhythmbox();

    void drawMainScreen();

    void initialize();
    void setBPM();
    void editSequence();
    void editPattern();
};