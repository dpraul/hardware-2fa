#include "TimeLib.h"
#include "LedControl.h"
#include "DS3231.h"
#include "TOTP.h"
#include <Wire.h>

// Google Authenticator uses 10-character private key and 30 second refresh
uint8_t hmacKey[] = {0x6b, 0x33, 0x30, 0x61, 0x73, 0x76, 0x62, 0x36, 0x79, 0x64};
TOTP totp_gen = TOTP(hmacKey, 10, 30);
char code[7];

/* Pin 12 is DIN
   Pin 11 is CLK
   Pin 10 is CS/LOAD */
LedControl lc = LedControl(12, 11, 10, 1);
DS3231 Clock;
bool Century = false, h12, PM; // flags needed to use DS3231 library

void setup() {
  Wire.begin();
  Serial.begin(9600);

  //Wake-up MAX72XX
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
}

void loop() {
  long ts = getTimestamp();
  char* newCode = totp_gen.getCode(ts);
  if (strcmp(code, newCode) != 0) {
    strcpy(code, newCode);
    displayCode(code);
  }
  int byte = 0;
  if (Serial.available() > 0) {
    byte = Serial.read();
    Serial.write(code);
  }
  
  delay(100);
}

void displayCode(char* code) {
  lc.setChar(0, 7, ' ', false);
  lc.setChar(0, 6, code[0], false);
  lc.setChar(0, 5, code[1], false);
  lc.setChar(0, 4, code[2], false);
  lc.setChar(0, 3, code[3], false);
  lc.setChar(0, 2, code[4], false);
  lc.setChar(0, 1, code[5], false);
  lc.setChar(0, 0, ' ', false);
}

long getTimestamp() {
  // Uses Time.maketime() to convert RTC time to a timestamp
  tmElements_t tm;
  tm.Second = Clock.getSecond();
  tm.Minute = Clock.getMinute();
  tm.Hour = Clock.getHour(h12, PM);
  tm.Day = Clock.getDate();
  tm.Month = Clock.getMonth(Century);
  // for makeTime, Year is the offset since 1970
  tm.Year = (2000 + Clock.getYear()) - 1970;

  return (long)makeTime(tm);
}
