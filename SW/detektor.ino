/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
//const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int pole[300]; 
uint16_t rozsah[256]; 
int svetlo = 1; 

ISR(TIMER1_COMPA_vect)
{
  noInterrupts();
  svetlo ^= 1; 
  digitalWrite(23,svetlo); 
  interrupts();
}

void setTimer()
{
  //https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
  // http://www.instructables.com/id/Arduino-Timer-Interrupts/
  TCNT1 = 0;
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B = 0b00001101;
  OCR1A = 1000; 
  TIMSK1 = 0b00000010;
}

void setup() {
  // initialize serial communications at 9600 bps:
  
  Serial.begin(9600);
  pinMode(23,OUTPUT); 
  //pinMode(0, OUTPUT);
  //digitalWrite(0, LOW);
  digitalWrite(23, HIGH);   
  noInterrupts();
  setTimer(); 
  interrupts();

  for (int i = 0; i < 256; i++)
  {
    rozsah[i] = 0; 
  }
}

void loop() {
  // read the analog in value:

  for(uint16_t i = 0; i < 65000; i++)
  {
    delayMicroseconds(1000); 
    
    sensorValue = analogRead(analogInPin);
    sensorValue >>= 2; 
    rozsah[sensorValue]++; 
  
    //digitalWrite(0, HIGH); 
    //delayMicroseconds(1);
    //digitalWrite(0, LOW); 

  }
  // map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 0, 255);
  

  // print the results to the Serial Monitor:
 
  for(int i = 0; i < 256; i++)
  {
     Serial.print(rozsah[i]);
     Serial.print(";"); 
  }
  Serial.println(""); 

    for (int i = 0; i < 256; i++)
    {
      rozsah[i] = 0; 
    }

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
}
