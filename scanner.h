#ifndef Scanner_h
#define Scanner_h

#include "Arduino.h"
#include "radio.h"
#include "lcd.h"
#include "menu.h"

class Scanner : public Item {
  public:
    // All Channels ordered by Mhz
    const int channelList[32] PROGMEM = {
      19, 18, 17, 16, 7, 8, 24, 6, 9, 25, 5, 10, 26, 4, 11, 27, 3, 12, 28, 2, 13, 29, 1, 14, 30, 0, 15, 31, 20, 21, 22, 23
    };
    Scanner(Radio *r, Lcd *l);
    int* scan();
    void set_threshold(int th);
    void show_results();
    void switch_show_results();
    virtual void start();
    virtual void up();
    virtual void down();
    virtual void select();
    virtual void refresh();
  private:
    const int RSSI_GRID_MAX = 38;
    const int DEF_RES_P = -1;
    int rssi_min = 50;
    const int rssi_max = 300;
    int RSSI_THRESHOLD = 200;
    Radio *radio;
    Lcd *lcd;
    int channel = 0;
    int rssi = 0;
    int scan_result[32];
    int result_pointer = DEF_RES_P;
    
    int scale(int rssi);
    String get_channel_line();
};

#endif
