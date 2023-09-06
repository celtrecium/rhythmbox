#include "Adafruit_PCD8544.h"
#include "./Rhythmbox.h"
#include "./PatternEditor.h"
#include "./images.h"

// BPMSelector class
BPMSelector::BPMSelector(const Hardware *hardware, const int currentBPM)
  : Selector(hardware, "bpm", currentBPM, 500) {}

// PatternSelector class
PatternSelector::PatternSelector(const Hardware *hardware, const Pattern *patterns, const int patternsValue) 
  : Selector(hardware, "ptn edit", 0, patternsValue), 
    patterns(patterns) {}

void PatternSelector::printItem() {
  hardware->display.print(patterns[index].id);
}

// MainScreenSelector class
MainScreenSelector::MainScreenSelector(const Hardware *hardware) 
  : Selector(hardware, "", 0, 6) {}

void MainScreenSelector::printItem() {
  static const char *items[] { "seq edit", "ptrn edit", "set bpm", "play", "save", "load" };
  hardware->display.print(items[index]);
}

// Rhythmbox class
void Rhythmbox::initialize() {
  // hardware.display.begin();

  // hardware.display.setContrast(60);
  // hardware.display.setTextSize(1);

  hardware.display.clearDisplay();
  hardware.display.display();

  pinMode(A0, INPUT);
  pinMode(11, OUTPUT);
}

void Rhythmbox::drawMainScreen() {
  hardware.display.clearDisplay();
  hardware.display.drawBitmap(9, 6, RHYTHMBOX_LOGO, 63, 26, BLACK);
}

Rhythmbox::Rhythmbox() 
  : bpm(120), 
    hardware(),
    sequencer(&this->hardware),
    bpmSelector(&this->hardware, bpm), 
    patternSelector(&this->hardware, sequencer.patterns, 8),
    mainScreenSelector(&this->hardware) { 
  initialize();

  while(true) {
    drawMainScreen();

    switch(mainScreenSelector.select()) {
      case 0:
        sequencer.edit();
        break;

      case 1:
        PatternEditor(&hardware, &sequencer.patterns[patternSelector.select()], bpm);
        break;

      case 2:
        bpm = bpmSelector.select();
        break;

      case 3:
        sequencer.play(bpm);
        break;

      default:
        break;
    } 
  }
}