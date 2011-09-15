/*
 * First.pde
 *
 * copyleft 2011 snootlab
 * free software, have fun !
 *
 * Basic usage example	
 *
 */

#include <Wire.h>
#include <Deuligne.h>

// Object initialization 

Deuligne lcd;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  lcd.init(); //  LCD panel initializatino
  lcd.clear(); // clear display
 
  lcd.setCursor(0,0); // top left
  lcd.print("Config");
  lcd.setCursor(7,0); // middle top line (7 from 0-15)
  lcd.print("ok");
  delay(1000);
}


void loop()
{
  lcd.clear(); // clear screen
  lcd.backLight(true);
  lcd.setCursor(0,0); // top left corner
  lcd.print("Deuligne");
  lcd.setCursor(7,1); // middle bottom line (7 from 0-15)
  lcd.print((long)millis()/1000);
  delay(1000);
}

