#include <stdio.h>

#define analog 2
#define D0 8

int raio = 1; 
float dist = 0;
volatile byte pulsos = 0;
unsigned long timeold = 0;
unsigned int ppv = 20;

String x;
char Dist[4];

void contador(){
    pulsos++;
}

void setup(){
    Serial.begin(115200);
    pinMode(D0, INPUT);
    pinMode(analog, INPUT);
    attachInterrupt(0, contador, FALLING);
    
}

void loop(){
  if(millis() - timeold >= 1000){
    
    detachInterrupt(0);
    dist = (pulsos/ppv)*(2*3.14*raio);
    Serial.print("distância:");
    Serial.println(dist);
    Serial.print("pulsos:");
    Serial.println(pulsos, DEC);
    timeold = millis();
    pulsos = 0;

    
    
    attachInterrupt(0, contador, FALLING);
    delay(100);
    }
  
}
