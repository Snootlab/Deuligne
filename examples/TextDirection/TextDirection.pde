/*

 *
 * copyleft 2011 snootlab
 * free software, have fun !
 *

 based on arduino LiquidCrystal Library - TextDirection
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch demonstrates how to use leftToRight() and rightToLeft()
 to move the cursor.
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe 
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 
*/

// include the library code:
#include <Wire.h>
#include <Deuligne.h>

// initialize the library with the numbers of the interface pins
Deuligne lcd;

int thisChar = 'a';

void setup() {
  Wire.begin();
  // set up the LCD's number of columns and rows: 
  lcd.init();
  // turn on the cursor:
  lcd.cursor();
  Serial.begin(9600);
}

void loop() {
  // reverse directions at 'm':
  if (thisChar == 'm') {
    // go right for the next letter
    lcd.rightToLeft(); 
  }
  // reverse again at 's':
  if (thisChar == 's') {
    // go left for the next letter
    lcd.leftToRight(); 
  }
  // reset at 'z':
  if (thisChar > 'z') {
    // go to (0,0):
    lcd.home(); 
    // start again at 0
    thisChar = 'a';
  }
  // print the character
  lcd.print(thisChar, BYTE);
  // wait a second:
  delay(1000);
  // increment the letter:
  thisChar++;
}








