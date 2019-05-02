#include "const.h"

int sensorValue = 0;            
uint16_t range[256]; 
int LEDPin = 1; 
int analogInPin = PA0_PIN; 
int tracePin = PB0_PIN; 

ISR(TIMER1_COMPA_vect)
{
  noInterrupts();
  LEDPin ^= 1; 
  digitalWrite(PC7_PIN,LEDPin); 
  interrupts();
}

void setTimer()
{
  //https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
  // http://www.instructables.com/id/Arduino-Timer-Interrupts/
  TCNT1 = 0;
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B = 0b00001101; // clock / 1024
  // 1000000 Hz / 2**10 = 976,5625 Hz
  OCR1A = 977; 
  TIMSK1 = 0b00000010;
}

void fallingEdgeDetector(int pin)
{
  
  int states[3][2] = { {0,1},{2,1},{2,2} }; 
	int state = 0;
  while(true)
  {
    int val = digitalRead(pin); 
    state = states[state][val]; 
    if(state == 2)
      return; 
  }

}

void setup() {
  
  Serial.begin(9600);
  pinMode(PC7_PIN,OUTPUT); 
  digitalWrite(PC7_PIN, HIGH);   
  noInterrupts();
  setTimer(); 
  interrupts();

  for (int i = 0; i < 256; i++)
  {
    range[i] = 0; 
  }
}

void loop() {
  pinMode(tracePin,OUTPUT);
  digitalWrite(tracePin, HIGH); 
  delayMicroseconds(1);
  digitalWrite(tracePin, LOW); 
  
  for(uint16_t i = 0; i < 65000; i++)
  {
    pinMode(tracePin, INPUT); 
    fallingEdgeDetector(tracePin); 
   
    sensorValue = analogRead(analogInPin);
    sensorValue >>= 2; 
    range[sensorValue]++; 
  
    pinMode(tracePin,OUTPUT);
    digitalWrite(tracePin, HIGH); 
    delayMicroseconds(1);
    digitalWrite(tracePin, LOW); 

  }

  for(int i = 0; i < 256; i++)
  {
     Serial.print(range[i]);
     Serial.print(";"); 
  }
  Serial.println(""); 

  for (int i = 0; i < 256; i++)
  {
    range[i] = 0; 
  }

}
