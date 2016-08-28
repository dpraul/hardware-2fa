#include <TimeLib.h>

#include <DS3231.h>
#include <Wire.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

DS3231 Clock;
bool Century=false, h12, PM;


void setup() {
  Wire.begin();
  Serial.begin(9600);
  setSyncProvider(requestSync);
}

void loop() {
  if (timeStatus()== timeNotSet) {
    if (Serial.available()) {
      processSyncMessage();
    }
  }
  else {
    ReadDS3231();
  }
  
  delay(1000);
}

void ReadDS3231() {
  Serial.print("20");
  Serial.print(Clock.getYear(),DEC);
  Serial.print('-');
  Serial.print(Clock.getMonth(Century),DEC);
  Serial.print('-');
  Serial.print(Clock.getDate(),DEC);
  Serial.print(' ');
  Serial.print(Clock.getHour(h12, PM),DEC);
  Serial.print(':');
  Serial.print(Clock.getMinute(),DEC);
  Serial.print(':');
  Serial.print(Clock.getSecond(),DEC);
  Serial.println();
  //Serial.print("Temperature=");
  //Serial.print(Clock.getTemperature()); 
  //Serial.print('\n');
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if (pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
       syncDS3231();
     }
  }
}

void syncDS3231() {
  Clock.setSecond(second());//Set the second 
  Clock.setMinute(minute());//Set the minute 
  Clock.setHour(hour());  //Set the hour 
  //Clock.setDoW(5);    //Set the day of the week
  Clock.setDate(day());  //Set the date of the month
  Clock.setMonth(month());  //Set the month of the year
  Clock.setYear(year() - 2000);  //Set the year (Last two digits of the year)
}

time_t requestSync() {
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}
