#include "grid.h"

Grid::Grid(Radio *r, Lcd *l) : Item("Grid View"), radio(r), lcd(l) {
}

void Grid::start() {
    lcd->clrScr();
    // x axi
    for(int i=0; i<8; i++) {
      int x = 12+i*9;
      lcd->printNumI(i+1, x, 1);
    }
    // y axi
    for (int j=0; j<5; j++) {
      int y = 8+j*7;
      //lcd->printNumI(j+1, 3, y);
      lcd->printStr(radio->get_channel_letter(j+1), 3, y);  
    }
    refresh();
}

void Grid::up() {
  prev_channel = channel;
  if(channel < 39) {
    channel++;
  } else {
    channel = 0;
  }
  refresh();
}

void Grid::down() {
  prev_channel = channel;
  if(channel > 0) {
    channel--;
  } else {
    channel = 39;  
  }
  refresh();
}

void Grid::select() {
  for(int i=0; i<40; i++) {
    up();  
  }
}

void Grid::refresh() {
    // clean prev selection window
    int y = prev_channel / 8 + 1;
    int x = prev_channel % 8 + 1;
    lcd->clrRect(x*9, y*7, 9, 7);
    // selection window
    y = channel / 8 + 1;
    x = channel % 8 + 1;
    lcd->clrArea(x*9, y*7, 9, 7);
    lcd->drawRect(x*9, y*7, 9+x*9, 7+y*7);
    // tune
    rssi = radio->tune(channel);
    Serial.println(rssi);
    int rssi_scale = (rssi-100)*5/180;
    if(rssi_scale>5) rssi_scale=5;
    Serial.println(rssi_scale);
    lcd->drawRect(2+x*9, 6+y*7, 7+x*9, 6-rssi_scale+y*7);    
    lcd->update();  
}

