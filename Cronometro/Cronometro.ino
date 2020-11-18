#include <stdio.h>
int intHours;
int intMinutes;
int intSeconds;

unsigned long cronometer;
unsigned long milliSecondsInSecond = 1000;
unsigned long milliSecondsInMinute = 60000;
unsigned long milliSecondsInHour = 3600000;
char timer[100];
void computeHMS(unsigned long duration) {

  float floatHours;
  float floatMinutes;
  float floatSeconds;

  intHours = 0;
  intMinutes = 0;
  intSeconds = 0;

  if (duration >= 1000) {
      floatSeconds = duration / milliSecondsInSecond % 60;
      intSeconds = floatSeconds;
      
      floatMinutes = duration / milliSecondsInMinute % 60;
      intMinutes = floatMinutes;
      
      floatHours = duration / milliSecondsInHour % 24;
      intHours = floatHours;
  
  }
  sprintf(timer, "%02d:%02d:%02d", intHours, intMinutes, intSeconds);
  delay(1000);
}
void setup(){
  Serial.begin(115200);
  
  }
void loop(){
  cronometer = millis();
  computeHMS(cronometer);
  
  }
