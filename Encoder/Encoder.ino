#include <PinChangeInterrupt.h>


#include <stdio.h>


#define analog1 3
#define D01 2
#define analog2 8
#define D02 9


int raio = 1; 
float dist = 0;
volatile byte pulsos_r = 0;
volatile byte pulsos_l = 0;
unsigned long timeold = 0;
unsigned int ppv = 20;



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
  if(millis() - timeold >= 1000){
    
    detachInterrupt(0);
    detachInterrupt(1);
    dist = (pulsos_r/ppv)*(2*3.14*raio);
    Serial.print("distância:");
    Serial.println(dist);
    Serial.print("pulsos:");
    Serial.println(pulsos_r, DEC);
    Serial.println(pulsos_l, DEC);
    timeold = millis();
    pulsos_r = 0;
    pulsos_l = 0;
    
    
    attachInterrupt(0, contador_r, FALLING);
    attachInterrupt(1, contador_l, FALLING);
    delay(100);
    }
  
}
