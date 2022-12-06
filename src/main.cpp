#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8);

//Lista de pines entrada encoder//
const int channelPinA = 3;
const int channelPinB = 2;
const int timeThreshold = 5;
const int pulsador = 6;

long timeCounter = 0;


volatile int ISRCounter = 1;
int counter = 1;

bool IsCW = true;

//Velocidad cominucacion serial
int serialSpeed = 9600;

bool menuInicialited = false;
bool menuFirstStart = false;

String screens[] = {"Boost", "Oil Temp", "Oil Press", "Water temp", "Options"};


void doEncode()
{
   if (millis() > timeCounter + timeThreshold)
   {
      if (digitalRead(channelPinA) == digitalRead(channelPinB))
      {
        IsCW = true;
        ISRCounter++;
      }
      else
      {
         IsCW = false;
         ISRCounter--;
      }
      timeCounter = millis();
   }
}

void printMainMenu(){
  
  while(menuInicialited == true){
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.clearBuffer();
    for (int i = 1; i < 6; i++)
    {
      u8g2.setCursor(13,(i*12));
      u8g2.print(screens[i - 1]);
    }
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawGlyph(0,((counter / 2) *12 + 2), 0x23F5);	
    Serial.println(counter / 2);
    u8g2.sendBuffer(); 
    if (counter != ISRCounter){
    counter  = ISRCounter;
    }
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.clearBuffer();
    if (menuFirstStart == false){
      menuFirstStart = true;
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.clearBuffer();
      for (int i = 1; i < 6; i++)
      {
      u8g2.setCursor(13,(i*12));
      u8g2.print(screens[i - 1]);
      }
      u8g2.setFont(u8g2_font_unifont_t_symbols);
      u8g2.drawGlyph(0,((counter / 2) *12 + 2), 0x23F5);	
      Serial.println(counter / 2);
      u8g2.sendBuffer(); 
      if (counter != ISRCounter){
      counter  = ISRCounter;
      }
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.clearBuffer();
    }
    
  }
  
}

void setup() {
  Serial.begin(serialSpeed);
  u8g2.begin();
  pinMode(channelPinA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(channelPinA), doEncode, CHANGE);
}


void loop() {
 printMainMenu();
  if(!digitalRead(pulsador)){
    menuInicialited = true;
  }

  
}

