#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <EEPROM.h>

int EEsize = 1024;
const int trigPin = 3;
const int echoPin = 6;
int openDistance = 5, totalTankLevel = 50;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
int startEEPROM,l[5],len,tenMinCounter=0,nl=0,ll=0,distance,oneMinCounter=0,alarmStatus=1,MotorStatus;
long d;
int melody[] = {
  262, 196, 196, 220, 196, 0, 247, 262
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Tank Lev Moniter");
  lcd.setCursor(3,1);
  lcd.print("Welcome");
  startEEPROM = EEPROM.read(0);
  len = EEPROM.read(1);
  for (int i=0; i<len; i++) {
    l[i] = EEPROM.read(startEEPROM+i);
  }
  delay(3000);
}

void loop() {
  Serial.println("Start");
  if (alarmStatus == 1) {
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(8, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(8);
    }
  }
  if (millis()/10000>oneMinCounter) {
    oneMinCounter++;
    ll=nl;
    nl = getTankLevel();
    Serial.println(nl);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tank 1: ");
    lcd.print(nl);
    lcd.print("%");
    if (millis()/10000>=tenMinCounter) {
      doTenMinSave(nl);
    }
    analyze();
    Serial.println("Done");
  }
}

void initializeEEPROM() {
  EEPROM.write(0,2);
  EEPROM.write(1,0);
}

void doTenMinSave(int nl) {
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
    Serial.print(l[i-1]);
    lcd.print(l[i-1]);
    if(i>1) {
      Serial.print("-");
      lcd.print("-");
    }
  }
}


int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  d = pulseIn(echoPin, HIGH);
  distance = d*0.034/2;
  Serial.print(distance);
  Serial.print(",");
  return distance;
}

int getAvgDistance() {
  int avg = 0;
  for(int i=0;i<10;i++) {
    avg+=getDistance();
    delay(100);
  }
  return avg/=10;
}

void analyze() {
  switch(MotorStatus) {
    case 0: if(nl-ll>0.5) {
      MotorStatus++;
    }
    break;
    case 1: if(nl-ll<=0.5) {
      MotorStatus--;
    }
    break;
  }
  if (nl<20 || nl>70) {
    alarmStatus = 1;
  } else {
    alarmStatus = 0;
  }
}

int getTankLevel() {
  int v = (int)(((getAvgDistance()-openDistance)*100)/totalTankLevel);
  if(v>100 || v<0) {
    v = getTankLevel();
  }
  return v;
}
