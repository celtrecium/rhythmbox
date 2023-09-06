#pragma once
#include <Arduino.h>
#include "./Pattern.h"
#include "./Geometry.h"
#include "./Hardware.h"

const int PE_CELL_WIDTH { 4 };
const int PE_CELL_HEIGHT { 6 };

class PatternEditor {
  private:
  
    const Hardware *hardware;
    const int bpm;
    Pattern *pattern;
    Point cursor;
    static const struct {
      Point pos;
      Size size;
    } grid { { 18, 5 }, {
          Pattern::trackLength * PE_CELL_WIDTH,
          Pattern::tracksValue * PE_CELL_HEIGHT } };

  public:
    PatternEditor(const Hardware *hardware, Pattern *pattern, const int bpm);
    
    void drawFilledCell(const Point pos);
    void drawCursor(const Point pos);
    void fillTrack(const int trackIndex);
    void fillTracks();
    void drawGrid();
    void drawInterface();
    void moveCursor(const Point point);
    void invertCell();
    void playPattern();
};