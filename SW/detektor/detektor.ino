#include "const.h"
#include "DS3231.h"
#include <string.h>
#include <SD.h>

int sensorValue = 0;            
uint16_t range[256]; 
int LEDPin = 1; 
int analogInPin = PA0_PIN; 
int tracePin = PB0_PIN;
char file[9];  
DS3231 RTC; 
RTCDateTime dateTime; 
char buf[1650];  // 1650 bytes is enough for date + 256*6 + 3 chars

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

// Need to test this function
void GetBuildTime(SyncDate * syncDate)
{
  char buildDate[] = __DATE__ " " __TIME__;
  
  char * buildDateSplitted = strtok(buildDate, " ");
  int month = -1;
  for (int i = 0; i < 12; i++)
  {
    // Get month
    if (!strcmp(buildDateSplitted, months[i]))
    {
      month = i + 1;
      break; 
    }
  }
  ((int*)syncDate)[0] = month; 
  int i = 1;
  buildDateSplitted = strtok(NULL, " ");
  while (buildDateSplitted != NULL)
  {
    // Get day, year, hour, minute and second
    ((int*)syncDate)[i] = atoi(buildDateSplitted);
    ++i; 
    buildDateSplitted = strtok(NULL, " :");
  }
}

void setup() {
  // Init serial line
  Serial.begin(9600);
  RTC.begin(); 
  dateTime = RTC.getDateTime();
  SyncDate buildTime;
  GetBuildTime(&buildTime);  
  if((uint16_t)buildTime.year > dateTime.year || (uint8_t)buildTime.month > dateTime.month || (uint8_t)buildTime.day > dateTime.day || (uint8_t)buildTime.hour > dateTime.hour || (uint8_t)buildTime.minute > dateTime.minute)
  {
    // (pseudo)protection of time update after normal setup
    Serial.println("Time synchronization");
    RTC.setDateTime((uint16_t)buildTime.year, (uint8_t)buildTime.month, (uint8_t)buildTime.day, (uint8_t)buildTime.hour, (uint8_t)buildTime.minute, (uint8_t)buildTime.sec); 
  }
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
  snprintf(file, 9, "%ld.csv", rnd); 
  
  dateTime = RTC.getDateTime();
  snprintf(buf, 200, "System time: %02u.%02u.%u %02u:%02u:%02u", dateTime.day, dateTime.month, dateTime.year, dateTime.hour, dateTime.minute, dateTime.second); 
  Serial.println(buf);
  
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

  buf[0] = 0; // zeroing the string
  dateTime = RTC.getDateTime();
  // Czech date format
  sprintf(buf,"%02u.%02u.%u %02u:%02u:%02u;", dateTime.day, dateTime.month, dateTime.year, dateTime.hour, dateTime.minute, dateTime.second); 
  
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
