#pragma once
#include <Arduino.h>
#include "./Hardware.h"
#include "./BottomWidget.h"

class Selector : public BottomWidget {
  public:
    const char *title;
    int index;
    const int maxIndex;
  
    Selector(const Hardware *hardware, const char *title, const int index, const int maxIndex);

    virtual void selectLeft();
    virtual void selectRight();
    virtual void printItem();
    void draw() final override;
    int select();
};

