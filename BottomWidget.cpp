#include "./BottomWidget.h"

BottomWidget::BottomWidget(const Hardware *hardware) 
  : hardware(hardware) {}

void BottomWidget::draw() {
  hardware->display.setCursor(0, 40);
  hardware->display.fillRect(0, 40, DISP_WIDTH, 8, WHITE);
  hardware->display.display();
}