#include <U8glib.h>


// ======================================================================================================
// --- Objetos ---
U8GLIB_ST7920_128X64_1X u8g( 6,  //E
                             5,  //R/W
                             4,  //RS
                             7); //RST

// ======================================================================================================
// --- Variáveis Globais ---
uint8_t draw_state = 0;


// ======================================================================================================
void draw();                        //função para desenho de string
void disp_graph_init();             //função de inicialização do display
void u8g_r_frame(uint8_t a);
// --- Configurações Iniciais ---
void setup() 
{

    pinMode(13, OUTPUT);           
    digitalWrite(13, HIGH);
    disp_graph_init(); 
    
} //end setup


// ======================================================================================================
// --- Loop Infinito ---
void loop() 
{
  u8g.firstPage();  
    do {
     u8g_r_frame();
      } while( u8g.nextPage() );
  
    delay(100);
} //end loop


// ======================================================================================================
// --- Desenvolvimento das Funções ---
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

} //end disp_graph_init
void draw() 
{
  
  u8g.setFont(u8g_font_unifont);
 
  u8g.drawStr( 30, 25, "Mecatron!");
  
} //end draw
void u8g_r_frame() {
  u8g.drawRFrame(0, 0,65,32, 0);
  u8g.setFont(u8g_font_courB08);
  u8g.drawStr(10,19, "00:00:00");
  u8g.drawRFrame(64, 0,64,32, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(85,19, "100m");
  u8g.drawRFrame(0, 31,43,33,  0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(10,50, "100N");
  u8g.drawRFrame(42, 31,43,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(46,50, "20BPM");
  u8g.drawRFrame(84, 31,44,33, 0);
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(93,50, "100N");
  
}
