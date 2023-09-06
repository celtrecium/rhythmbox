#include "./Keypad.h"

namespace Keypad {
  inline int isIntValueBetween(const int value, const int begin, const int end) {
    return value >= begin && value < end;
  }

  Keypad::Keypad(byte pin) 
    : _pin(pin), 
      key(Key::None) {
    pinMode(pin, INPUT);
  }

  Key Keypad::readKey()  {
    static const int keysAValueBegin[] = { 
      0,   60,  100, 180, 
      230, 280, 340, 400, 
      470, 530, 610, 690, 
      750, 800, 880, 950, 1000
    };
    static const int keysValue { 16 };

    int readValue = analogRead(_pin);

    for(int i = 0; i < keysValue; ++i) {
      if(isIntValueBetween(readValue, keysAValueBegin[i], keysAValueBegin[i + 1])) {
        return key = static_cast<Key>(i);
      }
    }

    return key = Key::None;
  }
}