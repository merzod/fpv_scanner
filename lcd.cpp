#include "lcd.h"

Lcd::Lcd(int p1, int p2, int p3, int p4, int p5) : LCD5110(p1, p2, p3, p4, p5) {
}
void Lcd::clrArea(int x, int y, int dx, int dy) {
  for(int i=0; i<dx; i++) {
    for(int j=0; j<dy; j++) {
      this->clrPixel(x+i, y+j);
    }
  }
}

void Lcd::invArea(int x, int y, int dx, int dy) {
  for(int i=0; i<dx; i++) {
    for(int j=0; j<dy; j++) {
      this->invPixel(x+i, y+j);
    }
  }
}

void Lcd::clrRect(int x, int y, int dx, int dy) {
  for(int i=0; i<=dx; i++) {
      this->clrPixel(x+i, y);
  } 
  for(int i=0; i<=dx; i++) {
      this->clrPixel(x+i, y+dy);
  } 
  for(int j=0; j<dy; j++) {
    this->clrPixel(x, y+j);
  }
  for(int j=0; j<dy; j++) {
    this->clrPixel(x+dx, y+j);
  }
}

void Lcd::printStr(String str, int x, int y) {
  char buf[str.length()+1];
  str.toCharArray(buf, str.length()+1);
  this->print(buf, x, y);
}

void Lcd::printBatt(int x, int y, double batt) {
  this->drawRect(x, y+1, x+4, y+4);
  this->drawLine(x+5, y+2, x+5, y+4);
  this->printNumF(batt, 1, x+7, y);
  this->print("V", x+19, y);
}


