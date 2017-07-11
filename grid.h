#ifndef Grid_h
#define Grid_h

#include "Arduino.h"
#include "radio.h"
#include "lcd.h"
#include "menu.h"

class Grid : public Item {
  public:
    Grid(Radio *r, Lcd *l);
    virtual void start();
    virtual void up();
    virtual void down();
    virtual void select();
    virtual void refresh();
  private:
    Radio *radio;
    Lcd *lcd;
    int channel = 0;
    int prev_channel = 0;
    int rssi = 0;
};

#endif
