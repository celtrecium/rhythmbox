#include "./Hardware.h"
#include "./Playback.h"
#include "./Config.h"
#define PINS Config::Display::pins

Hardware::Hardware() 
  : keypad(A0), 
    display(PINS[0], PINS[1], PINS[2], PINS[3], PINS[4]) {
  Playback::initialize();

  display.begin();
  display.setContrast(Config::Display::contrast);
  display.clearDisplay();
}
