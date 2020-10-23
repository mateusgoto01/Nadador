#include <U8glib.h>


#define buttom1 8
U8GLIB_ST7920_128X64_1X u8g( 6, 5, 4, 7);
                           
uint8_t draw_state1 = 0;
uint8_t draw_state2 = 0;
byte b1 = LOW; // variavel para o nivel lógico
int cb1 = 0; 

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
  u8g.drawStr(10,19, "00:00:01");
  u8g.drawRFrame(64, 0,64,32, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(85,19, "100m");
  u8g.drawRFrame(0, 31,43,33,  0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(6,50, "100mE");
  u8g.drawRFrame(42, 31,43,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(46,50, "20BPM");
  u8g.drawRFrame(84, 31,44,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(91,50, "100mD");
  
  
}
void u8g_frame2(){
  u8g.drawRFrame(0, 0,128,32, 0);
  u8g.drawStr(43,19, "00:00:02");
  u8g.drawRFrame(0, 31,43,33,  0);
  u8g.drawStr(8,50, "5 BM");
  u8g.drawRFrame(42, 31,43,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(46,50, "100WT");
  u8g.drawRFrame(84, 31,44,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(88,50, "100Wm");
  
  }
void u8g_frame3() {
  u8g.drawRFrame(0, 0,65,32, 0);
  u8g.setFont(u8g_font_courB08);
  u8g.drawStr(10,19, "00:00:03");
  u8g.drawRFrame(64, 0,64,32, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(85,19, "100m");
  u8g.drawRFrame(0, 31,43,33,  0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(6,50, "100NE");
  u8g.drawRFrame(42, 31,43,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(46,50, "20BPM");
  u8g.drawRFrame(84, 31,44,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(91,50, "100ND");
  
  
}
void u8g_frame4() {
  u8g.drawRFrame(0, 0,128,32, 0);
  u8g.drawStr(43,19, "00:00:04");
  u8g.drawRFrame(0, 31,43,33,  0);
  u8g.drawStr(5,50, "123WE");
  u8g.drawRFrame(42, 31,43,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(46,50, "100WT");
  u8g.drawRFrame(84, 31,44,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(88,50, "100WD");  
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
      delay(100);
      if(draw_state1 >3)
      draw_state1 = 0;      
    }
        u8g.firstPage();  
        do {
        draw1(draw_state1);
         } 
        while( u8g.nextPage() ); 
  }  
void goto_delay(int seconds){
  unsigned long start_millis = millis();
  unsigned long current_millis = 0;
  unsigned long delta_t = 0;

  while (delta_t < seconds*1000){
    current_millis = millis();
    delta_t = current_millis - start_millis;
  }
}
  
void setup() 
{ 
    pinMode(buttom1, INPUT);
    disp_graph_init();    
} 

void loop() 
{
    
  next_screen();
   
} 
