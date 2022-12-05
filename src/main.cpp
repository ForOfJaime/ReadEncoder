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

long timeCounter = 0;


volatile int ISRCounter = 0;
int counter = 0;

bool IsCW = true;

//Velocidad cominucacion serial
int serialSpeed = 9600;

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


void setup() {
  Serial.begin(serialSpeed);
  u8g2.begin();
  pinMode(channelPinA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(channelPinA), doEncode, CHANGE);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  for (int i = 1; i < 6; i++)
  {
    u8g2.setCursor(10,i*10);
    u8g2.print(screens[i - 1]);
  }
  u8g2.sendBuffer();
}


void loop() {




  if (counter != ISRCounter){
      counter = ISRCounter;
      String str = String(counter/2);
      Serial.println(str);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.setCursor(20,20);
      u8g2.print(str);
      u8g2.sendBuffer();
    }
}

