/*
 * Snootlab Deuligne joystick testing demo
 * Initial code from nuelectronics
 * 
 * copyleft 2011 snootlab
 * free software, have fun !
 *
 */

#include <Wire.h> // I2C library include
#include <Deuligne.h> // LCD library include

Deuligne lcd; // lcd object declaration

//Key message
char msgs[5][15] = {
  "Right Key OK ", 
  "Up Key OK    ", 
  "Down Key OK  ", 
  "Left Key OK  ", 
  "Select Key OK" };
int key=-1;
int oldkey=-1;


void setup()
{
  Wire.begin(); // join i2c
  lcd.init(); // LCD init

  lcd.clear(); // Clear Display

  lcd.backLight(true); // Backlight ON

  lcd.setCursor(5,0); // Place cursor row 6, 1st line (counting from 0)
  lcd.print("Setup");
  lcd.setCursor(7,1); // Place cursor row 8, 2nd line (counting from 0)
  lcd.print("ok");
  delay(2000);
  lcd.clear();
  lcd.print("Move Joystick");
}

void loop() {


  key = lcd.get_key();		        // read the value from the sensor & convert into key press

  if (key != oldkey)				    // if keypress is detected
  {
    delay(50);		// wait for debounce time
    key = lcd.get_key();	   // read the value from the sensor & convert into key press
    if (key != oldkey)				
    {			
      oldkey = key;
      if (key >=0){
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(0, 1);
        // print the key pressed:
        lcd.print(msgs[key]);
      }
    }
  }

  //delay(1000);
}

