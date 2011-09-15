/*
 * Smiley
 *
 * copyleft 2011 snootlab
 * free software, have fun !
 *
 * Demo for custom characters on LCD display
 * Should display a simple :) on top left corner
 */

#include <Wire.h>
#include <Deuligne.h>

Deuligne lcd;


// Custom caracter, 5 significant bits for each byte 
byte smiley  [8]={
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000
};


void setup(){
  Wire.begin();
  lcd.init();
  lcd.createChar(0,smiley);
  lcd.setCursor(0,0); // need to re-position after createChar
  lcd.write(0);
}


void loop(){
  // Switch backlight every second
  lcd.backLight(true);
  delay(1000);
  lcd.backLight(false);
  delay(1000);
}
