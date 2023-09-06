#pragma once
#include <Arduino.h>
#include "./Keypad.h"
#include <Adafruit_PCD8544.h>

constexpr int DISP_WIDTH { 84 };
constexpr int DISP_HEIGHT { 48 };

class Hardware {
  public:
    Keypad::Keypad keypad;
    Adafruit_PCD8544 display;
    
    Hardware();
};
