#include <LCD5110_Graph.h>
#include "radio.h"
#include "scanner.h"
#include "lcd.h"
#include "menu.h"
#include "list.h"
#include "grid.h"

// Define pins
#define SPI_DATA_PIN         10
#define SPI_SLAVE_SELECT_PIN 11
#define SPI_CLOCK_PIN        12
#define RSSI_PIN             A0

#define PIN_LCD_CLK 2
#define PIN_LCD_DIN 3
#define PIN_LCD_DC  4
#define PIN_LCD_RST 5
#define PIN_LCD_CE  6

#define PIN_UP     8
#define PIN_DOWN   13
#define PIN_SELECT 7
#define PIN_MENU   9
#define PIN_BATT   A1

// Define modes
#define MODE_MENU   0
#define MODE_EDITOR 1

#define BUTTON_DELAY 200

int mode = MODE_MENU;
int old_mode = -1;
int menu_selection = 2;

extern uint8_t SmallFont[];
extern uint8_t TinyFont[];

Lcd lcd(PIN_LCD_CLK, PIN_LCD_DIN, PIN_LCD_DC, PIN_LCD_RST, PIN_LCD_CE);
Radio radio(SPI_DATA_PIN, SPI_SLAVE_SELECT_PIN, SPI_CLOCK_PIN, RSSI_PIN);
Scanner scanner(&radio, &lcd);
Grid grid(&radio, &lcd);
List ch_select(&radio, &lcd, false, "Channel Sel");
List fr_select(&radio, &lcd, true, "Frequency Sel");
Menu menu(&lcd, 4);
  
void setup() {
  pinMode(SPI_SLAVE_SELECT_PIN, OUTPUT);
  pinMode(SPI_DATA_PIN, OUTPUT);
  pinMode(SPI_CLOCK_PIN, OUTPUT);
  pinMode(PIN_UP, INPUT);
  pinMode(PIN_DOWN, INPUT);
  pinMode(PIN_SELECT, INPUT);
  pinMode(PIN_MENU, INPUT);
  lcd.InitLCD();
  lcd.setFont(TinyFont);
  Serial.begin(9600);
  init_menu();
}

void init_menu() {
  menu.add_item((Item*) &grid);
  menu.add_item((Item*) &scanner);
  menu.add_item((Item*) &ch_select);
  menu.add_item((Item*) &fr_select);
}

void loop() {
  if(old_mode != mode) {
    switch(mode) {
      case MODE_MENU:
        menu.print();
        break;
      case MODE_EDITOR:
        menu.get_selected()->start();
        break;
    }    
    old_mode = mode;
  } else {
    if(mode == MODE_EDITOR) {
      menu.get_selected()->refresh();
    } else {
      int batt = analogRead(PIN_BATT);
      double b = ((double)batt * 4.47 )/1023;
      lcd.printBatt(60, 41, b);
      lcd.update();    
    }
    if(digitalRead(PIN_MENU) == HIGH) {
      mode = MODE_MENU;
    } else if(digitalRead(PIN_UP) == HIGH) {
      if(mode == MODE_MENU) {
        menu.selection_up();
      } else {
        menu.get_selected()->up();
      }
    } else if(digitalRead(PIN_DOWN) == HIGH) {
      if(mode == MODE_MENU) {
        menu.selection_down();
      } else {
        menu.get_selected()->down();
      }
    } else if(digitalRead(PIN_SELECT) == HIGH) {
      if(mode == MODE_MENU) {
        mode = MODE_EDITOR; // enter editor
      } else {
        menu.get_selected()->select();
      }
    }
  }
  delay(BUTTON_DELAY);
}
