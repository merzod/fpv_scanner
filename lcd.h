#ifndef Lcd_h
#define Lcd_h

#include <LCD5110_Graph.h>
#include "Arduino.h"

class Lcd : public LCD5110 {
  public:
    Lcd(int p1, int p2, int p3, int p4, int p5);
    void clrArea(int x, int y, int dx, int dy);
    void invArea(int x, int y, int dx, int dy);
    void printStr(String str, int x, int y);
    void printBatt(int x, int y, double batt);
};

#endif
