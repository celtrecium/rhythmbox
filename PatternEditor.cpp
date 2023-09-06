#include "./PatternEditor.h"
#include "./images.h"

PatternEditor::PatternEditor(const Hardware *hardware, Pattern *pattern, const int bpm) 
  : hardware(hardware),
    bpm(bpm),
    pattern(pattern), 
    cursor({ 0, 0 }) {
  drawInterface();
  delay(200);

  while(hardware->keypad.readKey() != Keypad::Key::K12) {
    switch(hardware->keypad.key) {
      case Keypad::Key::K0:
        moveCursor({ cursor.x - 1, cursor.y });
        break;

      case Keypad::Key::K1:
        moveCursor({ cursor.x, cursor.y + 1 });
        break;
        
      case Keypad::Key::K2:
        moveCursor({ cursor.x, cursor.y - 1 });
        break;
        
      case Keypad::Key::K3:
        moveCursor({ cursor.x + 1, cursor.y });
        break;
        
      case Keypad::Key::K4:
        invertCell();
        break;

      case Keypad::Key::K7:
        playPattern();

      default:
        break;
    }
    
    if(hardware->keypad.key != Keypad::Key::None) {
      drawInterface();
      delay(200);
    }
  }
}

void PatternEditor::playPattern() {
  for(int i = 0; i < Pattern::trackLength; ++i) {
    hardware->display.drawBitmap(grid.pos.x + PE_CELL_WIDTH*i, grid.pos.y - 3, ARROW_DOWN, 5, 3, BLACK);
    hardware->display.display();

    pattern->playSingleBeat(i);
    delay(15000 / bpm);

    hardware->display.drawBitmap(grid.pos.x + PE_CELL_WIDTH*i, grid.pos.y - 3,
      ARROW_DOWN, 5, 3, WHITE);
  }
}

void PatternEditor::invertCell() {
  pattern->tracks[cursor.y] ^= (1 << cursor.x);
}

void PatternEditor::drawFilledCell(const Point pos) {
  hardware->display.drawBitmap(
    grid.pos.x + pos.x*PE_CELL_WIDTH,
    grid.pos.y + pos.y*PE_CELL_HEIGHT,
    FILLED_CELL, 4, PE_CELL_HEIGHT, BLACK
  );
}

void PatternEditor::moveCursor(const Point point) {
  cursor = Point { 
    point.x < Pattern::trackLength 
      ? point.x < 0 
        ? 0 
        : point.x 
      : Pattern::trackLength-1,
    point.y < Pattern::tracksValue
      ? point.y < 0 
        ? 0 
        : point.y 
      : Pattern::tracksValue-1,
  };
}

void PatternEditor::drawCursor(const Point pos) {
  hardware->display.fillRect(
    grid.pos.x + pos.x*PE_CELL_WIDTH,
    grid.pos.y + pos.y*PE_CELL_HEIGHT + 3,
    PE_CELL_WIDTH, 3,
    BLACK
  );
}

void PatternEditor::fillTrack(const int trackIndex) {
  for(int i = 0; i < Pattern::trackLength; ++i) {
    if(pattern->tracks[trackIndex] & (1 << i)) {
      drawFilledCell({ i, trackIndex });
    }
  }
}

void PatternEditor::fillTracks() {
  for(int i = 0; i < Pattern::tracksValue; ++i) {
    fillTrack(i);
  }
}

void PatternEditor::drawGrid() {
  for(int i = 0; i <= Pattern::tracksValue; ++i) {
    hardware->display.drawLine(
      grid.pos.x, grid.pos.y + i*PE_CELL_HEIGHT, 
      grid.pos.x + grid.size.width, grid.pos.y + i*PE_CELL_HEIGHT, 
      BLACK);
  }

  for(int i = 0; i <= Pattern::trackLength; ++i) {
    hardware->display.drawLine(
      grid.pos.x + i*PE_CELL_WIDTH, grid.pos.y - !(i % 4)*2, 
      grid.pos.x + i*PE_CELL_WIDTH, grid.pos.y + grid.size.height, 
      BLACK);
  }
}

void PatternEditor::drawInterface() {
  hardware->display.clearDisplay();
  hardware->display.drawBitmap(0, grid.pos.y + 1, INSTR_LAYOUT, 16, 41, BLACK);

  drawGrid();
  fillTracks();
  drawCursor(cursor);

  hardware->display.display();
} 