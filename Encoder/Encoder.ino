#define D0 9
#define analog 10
int raio = 1; 
float dist = 0;
volatile byte pulsos = 0;
unsigned long timeold = 0;
unsigned int ppv = 20;

void contador(){
    pulsos++;
}

void setup(){
    Serial.begin(115200);
    pinMode(D0, INPUT);
    pinMode(analog, INPUT);
    attachInterrupt(1, contador, FALLING);
    
}

void loop(){
  if(millis() - timeold >= 1000){
    detachInterrupt(1);
    dist = (2*3,14*pulsos)/2*ppv;
    timeold = millis();
    pulsos = 0;

    Serial.print("distância:");
    Serial.println(dist, DEC);
    attachInterrupt(1, contador, FALLING);
    }
  
}
