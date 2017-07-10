#ifndef List_h
#define List_h

#include "Arduino.h"
#include "radio.h"
#include "lcd.h"
#include "menu.h"

class List : public Item {
  public:
    // All Channels ordered by Mhz
    const int channelList[40] PROGMEM = {
      19, 18, 32, 17, 33, 16, 7, 34, 8, 24, 6, 9, 25, 5, 35, 10, 26, 4, 11, 27, 3, 36, 12, 28, 2, 13, 29, 37, 1, 14, 30, 0, 15, 31, 38, 20, 21, 39, 22, 23
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
    const int SIZE = 40;
    const bool fr_order;   
    void tune(int channel);
    void print();
    int get_channel(int it);
};

#endif
