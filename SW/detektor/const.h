#pragma once

// pin layout
#define PB0_PIN 0
#define PB1_PIN 1
#define PB2_PIN 2
#define PB3_PIN 3
#define PB4_PIN 4
#define PB5_PIN 5
#define PB6_PIN 6
#define PB7_PIN 7
#define PD0_PIN 8
#define PD1_PIN 9
#define PD2_PIN 10
#define PD3_PIN 11
#define PD4_PIN 12
#define PD5_PIN 13
#define PD6_PIN 14
#define PD7_PIN 15
#define PC0_PIN 16
#define PC1_PIN 17
#define PC2_PIN 18
#define PC3_PIN 19
#define PC4_PIN 20
#define PC5_PIN 21
#define PC6_PIN 22
#define PC7_PIN 23
#define PA0_PIN 24
#define PA1_PIN 25
#define PA2_PIN 26
#define PA3_PIN 27
#define PA4_PIN 28
#define PA5_PIN 29
#define PA6_PIN 30
#define PA7_PIN 31

// Buildtime months options:
const char months[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

// Placeholder for the RTC sync time, do not change the order of the items!
struct SyncDate
{
  int month;
  int day;
  int year;
  int hour;
  int minute;
  int sec;
};


/*
------------------------------------
-----------SD CARD READER-----------
------------------------------------

CD/DATA3 (CS) - PD4
CMD (MOSI)- PB3
CLK (CLK) - PB5
DAT0 (MISO) - PB4
detect - PB2

*/

const int CDDATA3 = PC2_PIN; // CS
const int CMD = PB5_PIN;  // MOSI
const int CLK = PB7_PIN;  // CLK
const int DAT0 = PB6_PIN; // MISO

/* ----------------------------------*/

/*
------------------------------------
-------------RTC MODULE-------------
------------------------------------
*/

const int I2C_SCL = PC0_PIN;
const int I2C_SDA = PC1_PIN; 
 
/*----------------------------------*/

