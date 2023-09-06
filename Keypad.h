#pragma once
#include <Arduino.h>

namespace Keypad {
  enum class Key {
    K0,  K1,  K2,  K3,
    K4,  K5,  K6,  K7,
    K8,  K9,  K10, K11,
    K12, K13, K14, K15,
    None
  };

  class Keypad {
    private:
      byte _pin;

    public:
      Key key;

      Keypad(byte pin);
      Key readKey();
  };
}