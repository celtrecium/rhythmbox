#pragma once
#include <Arduino.h>
#include "./Hardware.h"
#include "./Pattern.h"
#include "./Keypad.h"

class Sequencer {
  private:
    const Hardware *hardware;
    byte sequence[250];
    int current;

  public:
    Pattern patterns[8];

    Sequencer(const Hardware *hardware);
    void doKeyAction(const Keypad::Key key);
    void drawOpacityGridRow(const int y);
    void drawInterface();
    void play(const int bpm);
    void edit();
};