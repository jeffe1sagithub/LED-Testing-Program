/* Temp reader shit
 very important doc: http://www.futurlec.com/Datasheet/Sensor/HM2301.pdf
*/

#ifndef LEDTest_H
#define LEDTest_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define TIMINGLIMIT 85

class LEDTest {
 private:
  byte data[6];
  byte _pin, _count;
  unsigned long _previousreadtime;
  boolean read(void); 
  boolean initialreading;

 public:
  LEDTest(byte pin, byte count=6);
  void begin(void);
  float readTemp();

};
#endif
