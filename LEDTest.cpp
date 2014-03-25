/* Temp reader shit
 very important doc for bitwis shit: http://www.futurlec.com/Datasheet/Sensor/HM2301.pdf
*/

#include "LEDTest.h"

LEDTest::LEDTest(uint8_t pin, uint8_t count) {
  _count = count;
  initialreading = true;
 _pin = pin;

}

void LEDTest::begin(void) {
  // setup pins
  pinMode(_pin, INPUT);
  digitalWrite(_pin, HIGH);	// set initial pin value to high and...
  _previousreadtime = 0;	// set to 0 for first (this keeps shit in line)
}

// returns float (temp in celcius)
float LEDTest::readTemp() {
  float f;

  if (read()) {
      f = data[2] & 0x7F;
      f *= 256;
      f += data[3];
      f /= 10;
      if (data[2] & 0x80)
	f *= -1;

      return f;
    
  }
  Serial.print("Read fail");
  return NAN;
}



// DATA=8 bit integral RH data+8 bit decimal RH data+8 bit integral T data+8 bit decimal T data+8 bit check-sum 

boolean LEDTest::read(void) {
  
  byte previousstate = HIGH;
  byte j = 0, i;
  byte counter = 0;
  unsigned long time;

  // pull up pin and wait (250 milliseconds due to hardware limitation)
  digitalWrite(_pin, HIGH);	// tells hardware its time to go
  delay(250);

  time = millis();

  if (time < _previousreadtime) {
    _previousreadtime = 0;
  }
  if (!initialreading && ((time - _previousreadtime) < 2000)) {
    return true; // return most recent valid reading
    //delay(2000 - (time - _previousreadtime));
  }
  
  initialreading = false;

  _previousreadtime = millis();

  // DATA=8 bit integral RH data+8 bit decimal RH data+8 bit integral T data+8 bit decimal T data+8 bit check-sum
  data[0] = data[1] = data[2] = data[3] = data[4] = 0; // RH (relative humidity is left out for now)
  
  // now pull it low for ~20 millis
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(20);
  cli();
  digitalWrite(_pin, HIGH);
  delayMicroseconds(40);
  pinMode(_pin, INPUT);

  // adjust iteration to timing
  for ( i=0; i< TIMINGLIMIT; i++) {
    counter = 0;
    while (digitalRead(_pin) == previousstate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break; // fail because it should not be high forthis long
      }
    }
    previousstate = digitalRead(_pin);

    if (counter == 255) break; //just in case

    // disregard initial 3 bits
    if ((i >= 4) && (i%2 == 0)) {
      // push bit into storage byte
      data[j/8] <<= 1;			// shift left
      if (counter > _count)
        data[j/8] |= 1;
      j++;
    }

  }

  sei();

  // check shit (40 bits, checksum)
  if ((j >= 40) && 
      (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
    return true;
  }
  

  return false;

}
