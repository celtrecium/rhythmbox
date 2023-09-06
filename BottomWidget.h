#pragma once
#include <Arduino.h>
#include "./Hardware.h"

class BottomWidget {
  public:
    const Hardware *hardware;
    
    BottomWidget(const Hardware *hardware);
    virtual void draw();
};