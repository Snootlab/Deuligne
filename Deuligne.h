/*
 * Deuligne.h
 *
 * copyleft 2011 snootlab
 * free software, have fun !
 *
 */
#ifndef SNOOTLCDI2C_h
#define SNOOTLCDI2_h

#include <inttypes.h>
#include <Print.h>


// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00


// overclcking I2C
#define CPU_FREQ 16000000L // (...) - a discuter, car fonction vitesse clock cpu
#define TWI_FREQ_MCP23008 400000L

// IMPORTANT! Wire. must have a begin() before calling init()


class Deuligne : public Print {
public:
  Deuligne(int devI2CAddress=0xA7, int num_lines=2, int lcdwidth=16, int bufferwidth= 40);
  void commandWrite(int command);
  void init();
  void clear();
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
  virtual void write(uint8_t);
  //  virtual void print(uint8_t);
  //  virtual void printIn(uint8_t*);
  void backLight( bool turnOn );

  //non-core---------------
  //void cursorTo(int line_num, int x);
  void setCursor(uint8_t col, uint8_t row);
  //{cursorTo(n,x);}
  
  //void leftScroll(int chars, int delay_time);
  //end of non-core--------

  //4bit only, therefore ideally private but may be needed by user
  //void commandWriteNibble(int nibble);

  //  virtual void print(int value){print((uint8_t)value);}
  //  virtual void printIn(char value[]){print((uint8_t*)value);}

  /**
   * from LiquidCrystal (official arduino) Library
   ***/
  void createChar(uint8_t, uint8_t[]);
  void command(uint8_t);

  void home();

  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  int get_key();

private:
  //void pulseEnablePin();
  //void pushNibble(int nibble);
  //void pushByte(int value);


  int myNumLines;
  int myWidth;
  int myAddress;
  int myBufferwidth;
  int adc_key_in;
  //  int adc_key_val[5];
  int NUM_KEYS;

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;


};
#endif
