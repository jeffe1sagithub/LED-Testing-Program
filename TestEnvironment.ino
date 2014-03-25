/*
This program is designed to test LED modules, and their 
components. The sensors that report to this program were 
placed in a controlled environment described in another
document. 


This program is designed for a specific test environment.
Therefore, it cannot be used in other environments. 

by Stephen Jefferson 3/24/2014
*/

//Temperature and humidity sensing
#include "LEDTest.h"
#define TEMPPIN 13    // modify to the pin we connected

LEDTest ledtest(TEMPPIN);



void setup() {
  //1
  Serial.begin(9600); 
  Serial.println("LEDTemp test!");
  ledtest.begin();
  
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
 // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

 float t = ledtest.readTemp();
 // check if returns are valid, if they are NaN (not a number) then something went wrong!
 if (isnan(t) ) {
   Serial.println("Temp read error");
 } else {
   
   Serial.print("Temp = "); 
   Serial.print(t);
   Serial.println(" C");
 }
 
  
}

