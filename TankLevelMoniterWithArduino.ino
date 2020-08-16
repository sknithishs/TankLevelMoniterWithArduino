#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <EEPROM.h>

int EEsize = 1024;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
int level,startEEPROM,l[5],len,tenMinCounter=0;
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Welcome, Tank");
  lcd.setCursor(3,1);
  lcd.print("Level Moniter");
  level = getTankLevel();
  startEEPROM = EEPROM.read(0);
  len = EEPROM.read(1);
  for (int i=0; i<len i++) {
    l[i] = EEPROM.read(startEEPROM+i);
  }
}

void loop() {
  int l = getTankLevel();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tank 1: ");
  lcd.print(l);
  lcd.print("%");
  if (mills%600000>tenMinCounter) {
    lcd.setCursor(0,1);
    for (int i=0; i<len i++) {
      lcd.print(l[i]);
      if(i<4) {
        lcd.print("-");
      }
    }
  }
  
}


int getTankLevel() {
  return 50;
}
