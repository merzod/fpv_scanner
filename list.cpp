#import "list.h"

List::List(Radio *r, Lcd *l, bool fr, char* name) : Item(name), radio(r), lcd(l), fr_order(fr) {
}

int List::get_channel(int it) {
  if(fr_order) {
    return channelList[it];
  } else {
    return it;
  }
}

void List::start() {
  tune(channel);
  print();
}

void List::print() {
  // LCD stuff: grid and base data
  lcd->clrScr();
  lcd->drawRect(0, 0, 83, 47); // common border
  
  for(int i=corner; i<corner+ROWS; i++) {
    String s = "";
    s+=radio->get_channel_name(get_channel(i));
    s+=":";
    s+=radio->get_channel_fr(get_channel(i));
    s+="MHz ";
    if(i == channel) {
      s+=((double)rssi/100.0);
      s+="V";      
      lcd->printStr(s, 3, 3 + (i-corner)*7);
      lcd->invArea(2, 2+(i-corner)*7, 80, 7);
    } else {
      lcd->printStr(s, 3, 3 + (i-corner)*7);  
    }
  }
  lcd->update();  
}

void List::up() {
  if(channel > 0) {
    channel--;
    if(channel == corner && corner > 0) {
      corner--;
    }
    start();
  }
}

void List::down() {
  if(channel < SIZE-1) {
    channel++;
    if(channel != SIZE-1 && channel - corner > 4) {
      corner++;
    }
    start();
  }
}

void List::select() { 
  refresh();
}

void List::refresh() { // refresh rssi from the selected channel
  rssi = radio->read_rssi();
  print();
}

void List::tune(int channel) {
  rssi = radio->tune(get_channel(channel));
}

