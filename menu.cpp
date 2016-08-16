#include "menu.h"

Menu::Menu(Lcd *lcd, int capacity) : capacity(capacity), lcd(lcd) {
  items = new Item *[capacity];
}

void Menu::add_item(Item* item) {
  if(size < capacity) {
    items[size] = item;
    size++;
  }
}

void Menu::print() {
  lcd->clrScr();
  lcd->drawRect(0, 0, 83, 47); // common border
  for(int i=0; i<size; i++) {
    lcd->print(items[i]->get_name(), 3, 3 + i*7);
    if(i == selection) {
      lcd->invArea(2, 2+i*7, 80, 7);
    }
  }
  lcd->update();
}

Item* Menu::get_selected() {
  return items[selection];
}
void Menu::selection_down() {
  if(selection < size-1) {
    selection++;
    print();
  }
}
void Menu::selection_up() {
  if(selection > 0) {
    selection--;
    print();
  }
}


