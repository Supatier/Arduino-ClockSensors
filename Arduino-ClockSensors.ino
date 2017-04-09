/*
Copyright 2017 Supatier

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**/

#include "DHT.h"
#include "LiquidCrystal.h"
#include "RTClib.h"
#include <LCDKeypad.h>
#include <Wire.h>

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

int DD, MM, YY, H, M, S;

String sDD;
String sMM;
String sYY;
String sH;
String sM;
String sS;

DHT dht(DHTPIN, DHTTYPE);
LCDKeypad lcd;
RTC_DS1307 RTC;
DateTime now;

byte term[8] = //icon for termometer
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};

byte pic[8] = //icon for water droplet
{
    B00100,
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B01110,
};

void setup() {
  Wire.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
  lcd.begin(16, 2);
  lcd.createChar(1,term);
  lcd.createChar(2,pic);
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop() {
  hb();
  get_time();
  get_sens();
}

void hb() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(400);
}

void get_time()
{
  now = RTC.now();
  lcd.setCursor(0, 0);
  lcd.print(sH);
  lcd.print(":");
  lcd.print(sM);
  lcd.print(" ");
  lcd.print(sDD);
  lcd.print("/");
  lcd.print(sMM);
  lcd.print("/");
  lcd.print(sYY);
  DateTime now = RTC.now();
  DD = now.day();
  MM = now.month();
  YY = now.year();
  H = now.hour();
  M = now.minute();
  S = now.second();

  //Make some fixes...
  if (DD < 10) {
    sDD = '0' + String(DD);
  } else {
    sDD = DD;
  }
  if (MM < 10) {
    sMM = '0' + String(MM);
  } else {
    sMM = MM;
  }
  sYY = YY;
  if (H < 10) {
    sH = '0' + String(H);
  } else {
    sH = H;
  }
  if (M < 10) {
    sM = '0' + String(M);
  } else {
    sM = M;
  }
  if (S < 10) {
    sS = '0' + String(S);
  } else {
    sS = S;
  }
}

void get_sens()
  {
  int h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.setCursor(0, 1);
  lcd.write(1);
  lcd.print(" ");
  lcd.print(t,1);
  lcd.print((char)223); //degree sign
  lcd.print("C   ");
  lcd.write(2);
  lcd.print(" ");
  lcd.print(h);
  lcd.print("%");
}
