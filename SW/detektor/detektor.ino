#include "const.h"
#include <SD.h>

int sensorValue = 0;            
uint16_t range[256]; 
int LEDPin = 1; 
int analogInPin = PA0_PIN; 
int tracePin = PB0_PIN;
char file[100];  

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

// OBSOLETE
void fallingEdgeDetector(int pin)
{
  int states[3][2] = { {0,1},{2,1},{2,2} }; 
	int state = 0;
  while(state != 2)
  {
    int val = digitalRead(pin); 
    state = states[state][val];  
  }

}

bool fileWrite(const char * fileName, const char * text)
{
  File file = SD.open(fileName, FILE_WRITE); 
  if (!file)
  {
    Serial.println("Cannot open the file");
    return false; 
  }
  int wrLen = file.write(text);
  file.close(); 
  return true; 

  
}

void setup() {
  // Init serial line
  Serial.begin(9600);

  // Init trace pin
  pinMode(PC7_PIN,OUTPUT); 
  digitalWrite(PC7_PIN, HIGH);   

  // Init timer
  noInterrupts();
  setTimer(); 
  interrupts();

  // Reset range array
  for (int i = 0; i < 256; i++)
  {
    range[i] = 0; 
  }


  // Init SD card reader
  if (!SD.begin(CDDATA3))
    Serial.println("Card reader error"); 

  // Generate pseudo-unique file name
  randomSeed(analogRead(PA0_PIN)); 
  long rnd = random(1, 10000); 
  snprintf(file, 99, "%ld.csv", rnd); 
}



void loop() {
  pinMode(tracePin,OUTPUT);
  digitalWrite(tracePin, HIGH); 
  //delayMicroseconds(1);
  //digitalWrite(tracePin, LOW); 

  for(uint16_t i = 0; i < 65000; i++)
  {
    pinMode(tracePin, INPUT); 
    delayMicroseconds(200);     

    // falling edge detector
    while(digitalRead(tracePin) == 0); 
    while(digitalRead(tracePin) == 1);
    
    sensorValue = analogRead(analogInPin);
    sensorValue >>= 2; 
    range[sensorValue]++;
  
    pinMode(tracePin,OUTPUT);
    digitalWrite(tracePin, HIGH); 
  }

  // preparation of the string to write
  char buf[1600];  // 1600 bytes is enough for 256*6 + 3 chars
  buf[0] = 0; // zeroing the string

  // write data to buffer
  for(int i = 0; i < 256; i++)
  {
     sprintf(buf + strlen(buf),"%u;",range[i]); 
  }
  
  // Windows-style newline
  sprintf(buf + strlen(buf), "\r\n"); 
  fileWrite(file, buf); 
  
  
  // Reset range array
  for (int i = 0; i < 256; i++)
  {
    range[i] = 0; 
  }

}
