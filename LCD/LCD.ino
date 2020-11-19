#include <U8glib.h>
#include <HX711_ADC.h>
#include <stdio.h>
#include <PinChangeInterrupt.h>

#define analog1 2
#define D01 3
#define analog2 8
#define D02 9

#define buttom1 A0
U8GLIB_ST7920_128X64_1X u8g( 6, 5, 4, 7);
                           
uint8_t draw_state1 = 0;
float BPM; //Beats per minutes
float br; // irá armazenar o numero de braçaas a cada 100m
// char de mensagens para o LCD
char timer[100]; 
char distR[10];
char distL[10];
char distT[10];
char bpm[10];
float pace;
char potT[10];
char potM[10];
char potR[10];
char potL[10];
char newtonR[10];
char newtonL[10];

char chardistR[10];
char chardistL[10];
String strdistR;
String strdistL;

char charbpm[10];
String strbpm;

char charpace[10];
String strpace;

int intpotT;
int intpotM;

int intload1;
int intload2;

char charpotR[10];
String strpotR;

char charpotL[10];
String strpotL;

// end
byte b1 = LOW; // variavel para o nivel lógico
int cb1 = 0; 

float raio = 0.5; // em metros
float dist_r = 0; //distância a cada braçada direita
float dist_l = 0; //distância a cada braçada esquerda
float dist_t = 0; // distância total 
int intdist_t;
float load1;
float load2;

char char_distR[10];
char char_distL[10];

float pot_t; //potência total
float pot_m; //potência média
float pot_r;
float pot_l;
int count; // contador de braçadas
int count_t; //contaddor total

float minutes;
unsigned long minutes2;
float dist_p;

volatile byte pulsos_r = 0; //pulsos momentaneos direito
unsigned long pulsos_r2 = 0; //pulsos totais direitos
volatile byte pulsos_l = 0; //pulsos momentaneos esquerdo
unsigned long pulsos_l2 = 0; //pulsos totais esquerdos
unsigned long timeold = 0;
float ppv = 20; // quantidade de furos em uma volta

const int HX711_dout_1 = 13; //mcu > HX711 no 1 dout pin
const int HX711_sck_1 = 12; //mcu > HX711 no 1 sck pin
const int HX711_dout_2 = 11; //mcu > HX711 no 2 dout pin
const int HX711_sck_2 = 10; //mcu > HX711 no 2 sck pin
HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1); //HX711 1
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); //HX711 2
float calibrationValue_1; // calibration value load cell 1
float calibrationValue_2; // calibration value load cell 2

void contador_r(){
    pulsos_r++;
    pulsos_r2++;
}
void contador_l(){
    pulsos_l++;
    pulsos_l2++;
}

void loadcell_start(){
 
  calibrationValue_1 = 22542.00; 
  calibrationValue_2 = 2254.00; 

  LoadCell_1.begin();
  LoadCell_2.begin();
  long stabilizingtime = 5000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step

  
  LoadCell_1.start(stabilizingtime, _tare);
  LoadCell_2.start(stabilizingtime, _tare);
 
  LoadCell_1.setCalFactor(calibrationValue_1); // user set calibration value (float)
  LoadCell_2.setCalFactor(calibrationValue_2); // user set calibration value (float)
  Serial.println("Startup is complete");
}

int intHours;
int intMinutes;
int intSeconds;

unsigned long cronometer;
unsigned long milliSecondsInSecond = 1000;
unsigned long milliSecondsInMinute = 60000;
unsigned long milliSecondsInHour = 3600000;


void computeHMS(unsigned long duration) { // função do cronometro

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
  delay(500);
}

void disp_graph_init();             //função de inicialização do display
void u8g_frame();
void draw();

void disp_graph_init()
{
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     //branco
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         //máxima intensidade
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}
void u8g_frame() {
  u8g.drawRFrame(0, 0,65,32, 0);
  u8g.setFont(u8g_font_courB08);
  u8g.drawStr(10,19, timer);
  u8g.drawRFrame(64, 0,64,32, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(85,19, distT);
  u8g.drawRFrame(0, 31,43,33,  0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(6,50, chardistL);
  u8g.drawRFrame(42, 31,43,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(46,50, charbpm);
  u8g.drawRFrame(84, 31,44,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(91,50, chardistR);
  
  
}
void u8g_frame2(){
  u8g.drawRFrame(0, 0,128,32, 0);
  u8g.drawStr(43,19, timer);
  u8g.drawRFrame(0, 31,43,33,  0);
  u8g.drawStr(8,50, charpace);
  u8g.drawRFrame(42, 31,43,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(46,50, potT);
  u8g.drawRFrame(84, 31,44,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(88,50, potM);
  
  }
void u8g_frame3() {
  u8g.drawRFrame(0, 0,65,32, 0);
  u8g.setFont(u8g_font_courB08);
  u8g.drawStr(10,19, timer);
  u8g.drawRFrame(64, 0,64,32, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(85,19, distT);
  u8g.drawRFrame(0, 31,43,33,  0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(6,50, newtonL);
  u8g.drawRFrame(42, 31,43,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(46,50, charbpm);
  u8g.drawRFrame(84, 31,44,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(91,50, newtonR);
  
  
}
void u8g_frame4() {
  u8g.drawRFrame(0, 0,128,32, 0);
  u8g.drawStr(43,19, timer);
  u8g.drawRFrame(0, 31,43,33,  0);
  u8g.drawStr(5,50, charpotL);
  u8g.drawRFrame(42, 31,43,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(46,50, potT);
  u8g.drawRFrame(84, 31,44,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(88,50, charpotR);  
}

void draw1(int a){
  switch(a){
    case 0: u8g_frame(); break;
    case 1: u8g_frame2(); break;
    case 2: u8g_frame3(); break;
    case 3: u8g_frame4(); break;
    }
  }
void next_screen(){
  b1 = digitalRead(buttom1);
  if (b1 == HIGH) {
      draw_state1 = draw_state1 + 1;
      if(draw_state1 >3)
      draw_state1 = 0;      
    }
        u8g.firstPage();  
        do {
        draw1(draw_state1);
         } 
        while( u8g.nextPage() ); 
  }  

  
void setup() 
{ 
    Serial.begin(115200);
    pinMode(buttom1, INPUT);
    disp_graph_init(); 
    loadcell_start();
    pinMode(D01, INPUT);
    pinMode(analog1, INPUT);
    pinMode(D02, INPUT);
    pinMode(analog2, INPUT);
    attachInterrupt(digitalPinToInterrupt(analog1), contador_r, FALLING);
    attachPinChangeInterrupt(analog2, contador_l, FALLING);   
} 

void loop() 
{
  cronometer = millis();
  computeHMS(cronometer);
  minutes2 = millis();
 
  
  if(millis() - timeold >= 1000){
    //Começo da contagem de pulsos
    detachInterrupt(digitalPinToInterrupt(analog1));
    detachInterrupt(analog2);
    //final da contagem de pulso
     LoadCell_1.update();
     LoadCell_2.update();
    //começo da load cell
    
    
    load1 = LoadCell_1.getData();
    if(load1 < 0){
      load1 = 0;
      };
    intload1 = load1;
    sprintf(newtonR, "%03dN", intload1);
    
    load2 = LoadCell_2.getData();
    if(load2 < 0){
      load2 = 0;
      };
    intload2 = load2;
    sprintf(newtonL, "%03dN", intload2);
    //fim da load cell
    //Calculos necessários
    Serial.print("Peso direito:");
    Serial.println(load1);
    Serial.print("Peso Esquerdo");
    Serial.println(load2);
    
    dist_r = (pulsos_r/ppv)*(3.14*raio);
    dist_p += dist_r;
    strdistR = String(dist_r, 2) + "m";
    strdistR.toCharArray(chardistR, 10);
    delay(10);
    dist_l = (pulsos_l/ppv)*(3.14*raio);
    strdistL = String(dist_l, 2) + "m";
    strdistL.toCharArray(chardistL, 10);
    delay(10);
    
    Serial.print("pulso direita:");
    Serial.println(pulsos_r);
    Serial.print("Pulso esquerda:");
    Serial.println(pulsos_l);
    Serial.print("Distancia direita:");
    Serial.println(dist_r);
    Serial.print("Distancia esquerda:");
    Serial.println(dist_l);

    Serial.print("Distancia direita2:");
    Serial.print(strdistR);
    Serial.println(chardistR);
    Serial.print("Distancia esquerda2:");
    Serial.print(strdistL);
    Serial.println(chardistL);
    
    dist_t = ((pulsos_r2 + pulsos_l2)/ppv)*(3.14*raio);
    intdist_t = dist_t;
    sprintf(distT, "%03dm", intdist_t);
  
    pot_t = (load1 + load2)*(dist_r + dist_l);
    intpotT= pot_t;
    sprintf(potT, "%03dW", intpotT);
    
    pot_m = pot_t/2;
    intpotM = pot_m;
    sprintf(potM, "%03dW", intpotM);

    pot_r = load1*dist_r;
    dtostrf(pot_r,3, 0, potR);
    strpotR = String(potR) + "W";
    strpotR.toCharArray(charpotR, 10);
    
    pot_l = load2*dist_l;
    dtostrf(pot_l,3, 0, potL);
    strpotL = String(potL) + "W";
    strpotL.toCharArray(charpotL, 10); 
    
    if(pulsos_r > 20 or pulsos_l > 20){
      count++; 
      } 
    timeold = millis();
    pulsos_r = 0;
    pulsos_l = 0;
    count_t++;
    attachInterrupt(digitalPinToInterrupt(analog1), contador_r, FALLING);
    attachPinChangeInterrupt(analog2, contador_l, FALLING);
    }
    if(count_t >=6){
      
      BPM = (count)*60;
      dtostrf(BPM,1, 0, bpm);
      strbpm = String(bpm) + "B/M";
      strbpm.toCharArray(charbpm, 10);
      count_t = 0;
      count = 0;
         
      };
    if(count_t < 6){
      BPM = 0;
      dtostrf(BPM,1, 0, bpm);
      strbpm = String(bpm) + "B/M";
      strbpm.toCharArray(charbpm, 10);
      };
      
    if(dist_p >= 100){

      minutes = float(minutes2) / float(milliSecondsInMinute);
      
      pace = (minutes)/(dist_t);
      
      minutes2 = 0;
      dist_t = 0; 
      strpace = String(pace,2) + "BM";
      strpace.toCharArray(charpace, 10);
      };
    if(dist_p >=100){
      pace = 0;
      strpace = String(pace,2) + "BM";
      strpace.toCharArray(charpace, 10);
      };  
  next_screen();
   
} 
