#include "scanner.h"

Scanner::Scanner(Radio *r, Lcd *l) : Item("Range Scan"), radio(r), lcd(l) {
}

int* Scanner::scan() {
  // LCD stuff: grid and base data
  lcd->clrScr();
  lcd->drawRect(0, 0, 81, 47); // common border
  lcd->drawLine(0, 39, 81, 39); // status line
  lcd->print("Scanning...", CENTER, 41);
  int th_sc = 38 - scale(RSSI_THRESHOLD);
  lcd->drawLine(1, th_sc, 81, th_sc);
  lcd->update();
  // scan logic
  int local_max = 0; // max/min for further grid scale
  int local_min = rssi_max;
  int local_max_pt = DEF_RES_P;
  result_pointer = DEF_RES_P; // reset result pointer before scan 
  for(int i=0; i<=39; i++) {
    channel = channelList[i];
    rssi = radio->tune(channel);
    int fr = radio->get_channel_fr();
    String cn = radio->get_channel_name();
    if (rssi < local_min) { // find minimum
      local_min = rssi;
    }
    if(rssi > local_max) {
      local_max = rssi;
      local_max_pt = i;
    }
    int rssi_scaled = scale(rssi);
    scan_result[i] = rssi;
    int x = 1+i*2;
    //lcd->clrArea(lcd, x, 1, 2, 38);
    lcd->drawRect(x, 38, x+1, 38-rssi_scaled);
    lcd->update();

    Serial.print(i);
    Serial.print("\t:");
    Serial.print(cn);
    Serial.print("\t:");
    Serial.print(fr);
    Serial.print("\t:");
    Serial.print(rssi);
    Serial.print("\t:");
    Serial.println(rssi_scaled);
  }
  if(local_max > RSSI_THRESHOLD) { // if something found - update result pointer with max rssi position
    result_pointer = local_max_pt;
  }
  rssi_min = local_min;
  return scan_result;
}

void Scanner::switch_show_results() {
  if(result_pointer != DEF_RES_P) {
    channel = channelList[result_pointer];
    radio->tune(channel);
    show_results();
  }
}

void Scanner::show_results() {
  lcd->clrArea(1, 40, 80, 7); // clean status line
  bool found = false;
  if(result_pointer != DEF_RES_P) {
    channel = channelList[result_pointer];
    rssi = radio->read_rssi();
    int rssi_scaled = scale(rssi);
    int fr = radio->get_channel_fr();
    // format string
    lcd->printStr(get_channel_line(), CENTER, 41);
    // mark bottom line
    int x = 1 + result_pointer*2;
    lcd->clrArea(x, 1, 2, 38);
    lcd->drawRect(x, 38, x+1, 38-rssi_scaled);
    if(rssi < RSSI_THRESHOLD) {
      int th_sc = 38 - scale(RSSI_THRESHOLD);
      lcd->drawLine(x, th_sc, x+2, th_sc);
    }
    lcd->drawLine(0, 39, 83, 39); // status line
    lcd->invArea(x, 39, 2, 1); 
  } else {
    lcd->print("Nothing found", CENTER, 41);
  }
  lcd->update();
}

String Scanner::get_channel_line() {
  String str = "";
  str+=radio->get_channel_name();
  str+=":";
  str+=radio->get_channel_fr();
  str+="MHz ";
  str+=((double)rssi/100.0);
  str+="V";
  return str;
}


int Scanner::scale(int rssi) {
  int rssi_scaled = rssi - rssi_min;
  int rssi_delta = rssi_max - rssi_min;
  int result = ((rssi_scaled * RSSI_GRID_MAX)/rssi_delta);
  return result < 0 ? 0 : result > RSSI_GRID_MAX ? RSSI_GRID_MAX : result;
}

void Scanner::set_threshold(int th) {
  RSSI_THRESHOLD = th;
}

void Scanner::start() {
  scan();
  switch_show_results();
}

void Scanner::select() {
  start();
}

void Scanner::refresh() {
  show_results();
}

void Scanner::up() {
  if(result_pointer != DEF_RES_P) { // if something found
    for(int i=result_pointer+1; i<=31; i++) {
      if(scan_result[i] > RSSI_THRESHOLD) { // found next spike, switch to it and exit
        result_pointer = i;
        switch_show_results();
        break;
      }
    }
  }
}

void Scanner::down() {
  if(result_pointer != DEF_RES_P) { // if something found
    for(int i=result_pointer-1; i>=0; i--) {
      if(scan_result[i] > RSSI_THRESHOLD) { // found next spike, switch to it and exit
        result_pointer = i;
        switch_show_results();
        break;
      }
    }
  }
}

