#include <PinChangeInterrupt.h>


#include <stdio.h>


#define analog1 2
#define D01 3
#define analog2 8
#define D02 9


float raio = 1; 
float dist = 0;
float dist_t = 0;
float pace = 0;
volatile byte pulsos_r = 0;
volatile byte pulsos_l = 0;
unsigned long timeold = 0;
float ppv = 20;
unsigned long timer = 0;
unsigned long milliSecondsInMinute = 60000;
float minutes = 0;

void contador_r(){
    pulsos_r++;
}
void contador_l(){
    pulsos_l++;
}

void setup(){
    Serial.begin(115200);
    pinMode(D01, INPUT);
    pinMode(analog1, INPUT);
    pinMode(D02, INPUT);
    pinMode(analog2, INPUT);
    attachInterrupt(1, contador_r, FALLING);
    attachPinChangeInterrupt(analog2, contador_l, FALLING);
    
}

void loop(){
  timer = millis();
  if(millis() - timeold >= 1000){
    
    detachInterrupt(1);
    detachInterrupt(analog2);
    dist = (pulsos_r/ppv)*(3.14*raio);
    dist_t += dist;
    Serial.print("distância:");
    Serial.println(dist);
    Serial.print("distância total:");
    Serial.println(dist_t);
    Serial.print("pulsos:");
    Serial.println(pulsos_r, DEC);
    Serial.println(pulsos_l, DEC);
    timeold = millis();
    pulsos_r = 0;
    pulsos_l = 0;
    
    
    attachInterrupt(1, contador_r, FALLING);
    attachInterrupt(analog2, contador_l, FALLING);
    delay(100);
    }
    if(dist_t >=100){
      Serial.print(timer);
      Serial.print("tempo corrido: ");
      minutes = float(timer) / float(milliSecondsInMinute);
      Serial.println(minutes);
      
      pace = (minutes)/(dist_t);
      
      Serial.print("pace:");
      Serial.println(pace);
      timer = 0;
      dist_t = 0; 
      };
     delay(1000);
}
