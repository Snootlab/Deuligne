/*
 * Deuligne.h
 *
 * copyleft 2011 snootlab
 * free software, have fun !
 *
 */
#ifndef SNOOTLCDI2C_h
#define SNOOTLCDI2C_h

#if defined(ARDUINO) && ARDUINO >= 100
#define SNOOT_RETURN size_t
#define SNOOT_WIREWRITE Wire.write
#else
#define SNOOT_RETURN void
#define SNOOT_WIREWRITE Wire.send
#endif

#if ! defined(HANDLE_UTF8_CHAR)
#define HANDLE_UTF8_CHAR 1
#endif


#include <inttypes.h>
#include <Print.h>
#include <Wire.h>

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
#define LCD_5x11DOTS 0x04
#define LCD_5x8DOTS 0x00


// overclcking I2C
#define CPU_FREQ 16000000L // (...) - a discuter, car fonction vitesse clock cpu
#define TWI_FREQ_MCP23008 400000L


#if HANDLE_UTF8_CHAR >= 1
typedef struct {
	uint8_t chr[4];
	uint8_t display[8];
} widechar_t;

static const widechar_t widechars[] =
{
	{ "é" , { 0b00010, 0b00100, 0b01110, 0b10001, 0b11111, 0b10000, 0b01110, 0b00000}},
	{ "è" , { 0b01000, 0b00100, 0b01110, 0b10001, 0b11111, 0b10000, 0b01110, 0b00000}},
	{ "ê" , { 0b00100, 0b01010, 0b01110, 0b10001, 0b11111, 0b10000, 0b01110, 0b00000}},
	{ "ë" , { 0b01010, 0b00000, 0b01110, 0b10001, 0b11111, 0b10000, 0b01110, 0b00000}},
	{ "à" , { 0b01000, 0b00100, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111, 0b00000}},
	{ "â" , { 0b00100, 0b01010, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111, 0b00000}},
	{ "ù" , { 0b01000, 0b00100, 0b10001, 0b10001, 0b10001, 0b10011, 0b01101, 0b00000}},
	{ "û" , { 0b00100, 0b01010, 0b10001, 0b10001, 0b10001, 0b10011, 0b01101, 0b00000}},
	{ "ç" , { 0b01110, 0b10000, 0b10000, 0b10001, 0b01110, 0b00100, 0b01000, 0b00000}},
	{ "ô" , { 0b00100, 0b01010, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000}},
	{ "î" , { 0b00100, 0b01010, 0b00000, 0b01100, 0b00100, 0b00100, 0b01110, 0b00000}},
	{ "ï" , { 0b01010, 0b00000, 0b01100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000}},
	{ "£" , { 0b00110, 0b01000, 0b01000, 0b11110, 0b01000, 0b01001, 0b10110, 0b00000}},
	{ "€" , { 0b00110, 0b01001, 0b11100, 0b01000, 0b11100, 0b01001, 0b00110, 0b00000}},
	{ "µ" , { 0b00000, 0b00000, 0b00000, 0b10001, 0b10001, 0b11001, 0b10110, 0b10000}},
	{ "œ" , { 0b00000, 0b00000, 0b01010, 0b10101, 0b10111, 0b10100, 0b01011, 0b00000}},

	/* Please complete with the UTF-8 chars you need */
};
#endif

class Deuligne : public Print {
public:
  Deuligne(uint8_t devI2CAddress=0xA7, uint8_t num_lines=2, uint8_t lcdwidth=16, uint8_t bufferwidth= 40);
  void commandWrite(int command);
  void init();
  void clear();
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
  virtual SNOOT_RETURN write(uint8_t);
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
  void createChar(uint8_t,const uint8_t[]);
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

  int8_t get_key();

private:
  //void pulseEnablePin();
  //void pushNibble(int nibble);
  //void pushByte(int value);
 #if HANDLE_UTF8_CHAR >= 1
  void loadAndPrintExtendedChar(uint8_t value);
  void handle_utf8(uint8_t value);
#endif

  uint8_t last_ddramaddr;

  uint8_t myNumLines;
  uint8_t myWidth;
  uint8_t myAddress;
  uint8_t myBufferwidth;

  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t utf_hi_char;


};
#endif
