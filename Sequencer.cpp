#include "./Sequencer.h"
#include "./BottomWidget.h"

Sequencer::Sequencer(const Hardware *hardware)
  : hardware(hardware),
    patterns({ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' }), 
    current(0) {
  memset(sequence, 0, sizeof(sequence));
};

void Sequencer::edit() {
    drawInterface();
    delay(200);

    while(hardware->keypad.readKey() != Keypad::Key::K7) {
      switch(hardware->keypad.key) {
        case Keypad::Key::K0:
          sequence[current] -= !!sequence[current];
          break;
    
        case Keypad::Key::K1:
          current += !!(current < sizeof(sequence));
          break;
    
        case Keypad::Key::K2:
          current -= !!current;
          break;
        
        case Keypad::Key::K3:
          sequence[current] += !!(sequence[current] < 8);
          break;
    
        default:
          break;
      }

      if(hardware->keypad.key != Keypad::Key::None) {
        drawInterface();
        delay(200);
      }
   }
}

void Sequencer::drawOpacityGridRow(const int y) {
  for(int i = 0; i < 4; ++i) {
    hardware->display.drawLine(0, y + i*2, DISP_WIDTH, y + i*2, WHITE);
  }
  
  hardware->display.setTextColor(BLACK);
}

void Sequencer::drawInterface() {
  hardware->display.clearDisplay();
  hardware->display.setCursor(12, 8);
  hardware->display.println("Num Pattern");
  
  for(int i = -1; i <= 1; ++i) {
    int y = 24 + 8*i;

    if(current + i < 0) {
      hardware->display.println();
      continue;
    } 
    
    hardware->display.setCursor(12, y);
    hardware->display.print(current + i);
    hardware->display.setCursor(36, y);

    if(!sequence[current + i]) {
      hardware->display.print("Empty");
    } else {
      hardware->display.print(patterns[sequence[current + i] - 1].id);
    }

    if(i) drawOpacityGridRow(y);
  }
  
  hardware->display.display();
}

void Sequencer::play(const int bpm) {
  BottomWidget(hardware).draw();
  hardware->display.print("playing");
  hardware->display.display();

  for(int i = 0; sequence[i]; ++i) {
    patterns[sequence[i] - 1].play(bpm);
  }
}