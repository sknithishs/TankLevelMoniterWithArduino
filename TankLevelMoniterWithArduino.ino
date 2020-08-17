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
  lcd.print("Tank Lev Moniter");
  lcd.setCursor(3,1);
  lcd.print("Welcome");
  level = getTankLevel();
  startEEPROM = EEPROM.read(0);
  len = EEPROM.read(1);
  for (int i=0; i<len; i++) {
    l[i] = EEPROM.read(startEEPROM+i);
  }
  delay(3000);
}

void loop() {
  int nl = getTankLevel();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tank 1: ");
  lcd.print(nl);
  lcd.print("%");
  if (millis()/10000>=tenMinCounter) {
    doTenMinSave(nl);
  }
  delay(10000);
}

void initializeEEPROM() {
  EEPROM.write(0,2);
  EEPROM.write(1,0);
}

void tenMinSave(int nl) {
  tenMinCounter++;
  if (len==5) {
    if (startEEPROM==248) {
      EEPROM.write(0,2);
      EEPROM.write(1,1);
    }
    EEPROM.write(startEEPROM+len,nl);
    startEEPROM++;
    EEPROM.write(0,startEEPROM);
    l[0]=l[1];
    l[1]=l[2];
    l[2]=l[3];
    l[3]=l[4];
    l[4]=nl;
  } else {
    EEPROM.write(startEEPROM+len,nl);
    len++;
    EEPROM.write(1,len);
    l[len-1]=nl;
  }
  lcd.setCursor(0,1);
  for (int i=len; i>0; i--) {
    lcd.print(l[i-1]);
    if(i>1) {
      lcd.print("-");
    }
  }
}

int temp=50;
int getTankLevel() {
  return temp++;
}
