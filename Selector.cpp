#include "./Selector.h"

Selector::Selector(const Hardware *hardware, const char *title, const int index, const int maxIndex) 
  : BottomWidget(hardware), title(title), index(index), maxIndex(maxIndex) {}

void Selector::printItem() {
  hardware->display.print(index);
}

void Selector::selectLeft() {
  index -= !!index;
}

void Selector::selectRight() {
  index += !!(index < maxIndex);
}

void Selector::draw() {
  BottomWidget::draw();
  hardware->display.print(title);
  hardware->display.print(": ");
  printItem();
  hardware->display.display();
}

int Selector::select() {
  draw();
  delay(200);

  while(hardware->keypad.readKey() != Keypad::Key::K4) {
    switch(hardware->keypad.key) {
      case Keypad::Key::K0:
        selectLeft();
        break;

      case Keypad::Key::K3:
        selectRight();
        break;

      case Keypad::Key::K7:
        return -1;
    }
      
    if(hardware->keypad.key != Keypad::Key::None) {
      draw();
      delay(200);
    }
  }

  return index;
}