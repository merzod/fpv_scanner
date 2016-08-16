#ifndef Menu_h
#define Menu_h

#include "Arduino.h"
#include "lcd.h"

class Item {
  public:
    Item() {}    
    Item(char *name) : name(name) {
    }
    char* get_name() {
      return name;
    }
    virtual void start() {} // called on switch to the item from the menu mode
    virtual void up() {} // called if user press up button
    virtual void down() {} // called if user press down button
    virtual void select() {} // called if user press select button
    virtual void refresh() {} // called priodically to refresh data
  private:
    char* name;
};

class Menu {
  public:
    Menu(Lcd* lcd, int capacity);
    void add_item(Item* item);
    void print();
    Item* get_selected();
    void selection_up();
    void selection_down();
  private:  
    const int capacity;
    Item** items;
    int size = 0;
    int selection = 0;
    Lcd* lcd;
};
#endif

