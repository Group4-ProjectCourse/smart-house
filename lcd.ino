#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include<SoftwareSerial.h>
SoftwareSerial arduino(6,7);
int motorPin=8;
const int lightSensorPin = A0;

String input;
void setup() {
  lcd.begin(16, 2);
  pinMode(lightSensorPin,OUTPUT);
  arduino.begin(115200); 
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(arduino.available()>0){
    lcd.setCursor(0,0);
    input = arduino.readString();
    Serial.println(input);
    lcd.print(input);
  }
}
