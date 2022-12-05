#include <Arduino.h>

//Lista de pines entrada encoder//
const int channelPinA = 2;
const int channelPinB = 3;
const int timeThreshold = 5;

long timeCounter = 0;


volatile int ISRCounter = 0;
int counter = 0;

bool IsCW = true;

//Velocidad cominucacion serial
int serialSpeed = 9600;



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
  pinMode(channelPinA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(channelPinA), doEncode, CHANGE);
}


void loop() {

 if (counter != ISRCounter)
   {
      counter = ISRCounter;
      Serial.println(counter);
   }
  

}

