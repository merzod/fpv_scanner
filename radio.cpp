#include "radio.h"

Radio::Radio(int spi_data, int spi_slave_select, int spi_clock, int rssi) : spi_data_pin(spi_data), spi_slave_select_pin(spi_slave_select), spi_clock_pin(spi_clock), rssi_pin(rssi) {
}

int Radio::tune(int channel) {
  current_channel = channel;
  set_channel(current_channel);
  wait_rssi();
  return (read_rssi());
}

int Radio::get_channel() {
  return (current_channel);
}

bool Radio::is_channel_set() {
  return (current_channel != NO_CHANNEL);
}

String Radio::get_channel_name() {
  return get_channel_name(current_channel);
}

String Radio::get_channel_name(int current_channel) {
  String s;
  int c1 = current_channel / 8 + 1;
  int c2 = current_channel % 8 + 1;
  switch(c1) {
    case 1:
      s = "A";break;
    case 2:
      s = "B";break;
    case 3:
      s = "E";break;
    case 4:
      s = "F";break;
    case 5:
      s = "R";break;
    default:
      s = "X";
  }
  s += c2;
  return s;
}

int Radio::get_channel_fr() {
  if(is_channel_set()) {
    return get_channel_fr(current_channel);
  } else {
    return 0;
  }
}

int Radio::get_channel_fr(int current_channel) {
    return (channelFreqTable[current_channel]);
}

void Radio::wait_rssi() {
  delay(RSSI_DELAY);
}

int Radio::read_rssi() {
    int rssi = 0;
    for (int i = 0; i < 10; i++) {
        rssi += analogRead(rssi_pin);
    }
    rssi=rssi/10; // average
    return (rssi);
}

void Radio::set_channel(int channel) {
  int i;
  int channelData;

  //channelData = pgm_read_word(&channelTable[channel]);
  channelData = chTable[channel];
  //channelData = pgm_read_word_near(chTable + channel);

  // bit bash out 25 bits of data
  // Order: A0-3, !R/W, D0-D19
  // A0=0, A1=0, A2=0, A3=1, RW=0, D0-19=0
  SERIAL_ENABLE_HIGH();
  delayMicroseconds(1);  
  //delay(2);
  SERIAL_ENABLE_LOW();

  SERIAL_SENDBIT0();
  SERIAL_SENDBIT0();
  SERIAL_SENDBIT0();
  SERIAL_SENDBIT1();

  SERIAL_SENDBIT0();

  // remaining zeros
  for (i = 20; i > 0; i--)
    SERIAL_SENDBIT0();

  // Clock the data in
  SERIAL_ENABLE_HIGH();
  //delay(2);
  delayMicroseconds(1);  
  SERIAL_ENABLE_LOW();

  // Second is the channel data from the lookup table
  // 20 bytes of register data are sent, but the MSB 4 bits are zeros
  // register address = 0x1, write, data0-15=channelData data15-19=0x0
  SERIAL_ENABLE_HIGH();
  SERIAL_ENABLE_LOW();

  // Register 0x1
  SERIAL_SENDBIT1();
  SERIAL_SENDBIT0();
  SERIAL_SENDBIT0();
  SERIAL_SENDBIT0();

  // Write to register
  SERIAL_SENDBIT1();

  // D0-D15
  //   note: loop runs backwards as more efficent on AVR
  for (i = 16; i > 0; i--)
  {
    // Is bit high or low?
    if (channelData & 0x1)
    {
      SERIAL_SENDBIT1();
    }
    else
    {
      SERIAL_SENDBIT0();
    }

    // Shift bits along to check the next one
    channelData >>= 1;
  }

  // Remaining D16-D19
  for (i = 4; i > 0; i--)
    SERIAL_SENDBIT0();

  // Finished clocking data in
  SERIAL_ENABLE_HIGH();
  delayMicroseconds(1);
  //delay(2);

  digitalWrite(spi_slave_select_pin, LOW);
  digitalWrite(spi_clock_pin, LOW);
  digitalWrite(spi_data_pin, LOW);
}

void Radio::SERIAL_SENDBIT1()
{
  digitalWrite(spi_clock_pin, LOW);
  delayMicroseconds(1);

  digitalWrite(spi_data_pin, HIGH);
  delayMicroseconds(1);
  digitalWrite(spi_clock_pin, HIGH);
  delayMicroseconds(1);

  digitalWrite(spi_clock_pin, LOW);
  delayMicroseconds(1);
}

void Radio::SERIAL_SENDBIT0()
{
  digitalWrite(spi_clock_pin, LOW);
  delayMicroseconds(1);

  digitalWrite(spi_data_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(spi_clock_pin, HIGH);
  delayMicroseconds(1);

  digitalWrite(spi_clock_pin, LOW);
  delayMicroseconds(1);
}

void Radio::SERIAL_ENABLE_LOW()
{
  delayMicroseconds(1);
  digitalWrite(spi_slave_select_pin, LOW);
  delayMicroseconds(1);
}

void Radio::SERIAL_ENABLE_HIGH()
{
  delayMicroseconds(1);
  digitalWrite(spi_slave_select_pin, HIGH);
  delayMicroseconds(1);
}



