#ifndef List_h
#define List_h

#include "Arduino.h"
#include "radio.h"
#include "lcd.h"
#include "menu.h"

class List : public Item {
  public:
    // All Channels ordered by Mhz
    const int channelList[32] PROGMEM = {
      19, 18, 17, 16, 7, 8, 24, 6, 9, 25, 5, 10, 26, 4, 11, 27, 3, 12, 28, 2, 13, 29, 1, 14, 30, 0, 15, 31, 20, 21, 22, 23
    };
    List(Radio *r, Lcd *l, bool fr_order, char* name);
    virtual void start();
    virtual void up();
    virtual void down();
    virtual void select();
    virtual void refresh();
  private:  
    Radio *radio;
    Lcd *lcd;
    int channel = 0;
    int rssi = 0;
    int corner = 0; 
    const int ROWS = 6;
    const int SIZE = 32;
    const bool fr_order;   
    void tune(int channel);
    void print();
    int get_channel(int it);
};

#endif
