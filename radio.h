#ifndef Radio_h
#define Radio_h

#include "Arduino.h"

class Radio {
  public:
    // Channels to sent to the SPI registers
    const int chTable[40] PROGMEM = {
      // Channel 1 - 8
      0x2A05,    0x299B,    0x2991,    0x2987,    0x291D,    0x2913,    0x2909,    0x289F,    // Band A
      0x2903,    0x290C,    0x2916,    0x291F,    0x2989,    0x2992,    0x299C,    0x2A05,    // Band B
      0x2895,    0x288B,    0x2881,    0x2817,    0x2A0F,    0x2A19,    0x2A83,    0x2A8D,    // Band E
      0x2906,    0x2910,    0x291A,    0x2984,    0x298E,    0x2998,    0x2A02,    0x2A0C,    // Band F / Airwave
      0x281D,    0x288F,    0x2902,    0x2914,    0x2987,    0x2999,    0x2A0C,    0x2A1E     // Band R / RaceBand
      
    };
    // Channels with their Mhz Values
    const int channelFreqTable[40] PROGMEM = {
      // Channel 1 - 8
      5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725, // Band A
      5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866, // Band B
      5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945, // Band E
      5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880, // Band F / Airwave
      5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917  // Band R / RaceBand
    };

    Radio(int spi_data, int spi_slave_select, int spi_clock, int rssi);
    int tune(int channel);
    int read_rssi();
    int get_channel(); // number of the channel in the table (e.g. 0-31)
    String get_channel_name(); // band number (e.g. 11-18, 21-28, ... 41-48)
    String get_channel_letter(int ch_bit_1); // band letter
    int get_channel_fr(); // channel frequency in MHz (e.g. 5885, 5905)
    String get_channel_name(int channel); // band number (e.g. 11-18, 21-28, ... 41-48)
    int get_channel_fr(int channel); // channel frequency in MHz (e.g. 5885, 5905)
  private:
    const int RSSI_DELAY = 50;
    const int NO_CHANNEL = -1;
    const int spi_data_pin;
    const int spi_slave_select_pin;
    const int spi_clock_pin;
    const int rssi_pin;
    int current_channel = NO_CHANNEL;

    void set_channel(int channel);
    void wait_rssi();
    bool is_channel_set();
    void SERIAL_SENDBIT1();
    void SERIAL_SENDBIT0();
    void SERIAL_ENABLE_LOW();
    void SERIAL_ENABLE_HIGH();
};

#endif
