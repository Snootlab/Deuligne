/***
 *
 *
 * Deuligne library
 *
 * copyleft 2011 snootlab
 * free software, have fun !
 *
 **/
#include "Deuligne.h"

extern "C" {
  #include <string.h> //needed for strlen()
  #include <inttypes.h>
#if defined(ARDUINO) &&ARDUINO < 100
  #include "WConstants.h"  //all things wiring / arduino
#endif
}
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

//command bytes for LCD
#define CMD_CLR 0x01
#define CMD_RIGHT 0x1C
#define CMD_LEFT 0x18
#define CMD_HOME 0x02

#define NUM_KEYS 5

//stuff the library user might call---------------------------------

//constructor.  num_lines must be 1 or 2, currently.

static byte dataPlusMask = 0x20; // TODO!!!

static const int adc_key_val[NUM_KEYS] = { 50, 190, 400, 540, 770 };

Deuligne::Deuligne( uint8_t devI2CAddress, uint8_t num_lines, uint8_t lcdwidth, uint8_t bufferwidth)  {
  myNumLines = num_lines;
  myWidth = lcdwidth;
  myAddress = devI2CAddress;
  myBufferwidth= bufferwidth;
}

void SetMCPReg( byte deviceAddr, byte reg, byte val ) {
  Wire.beginTransmission(deviceAddr);
  SNOOT_WIREWRITE(reg);
  SNOOT_WIREWRITE(val);
  Wire.endTransmission();
}

void SendToLCD( byte deviceAddr, byte data ) {
  data |= dataPlusMask;
  SetMCPReg(deviceAddr,0x0A,data);
  data ^= 0x10; // E
  delayMicroseconds(1);
  SetMCPReg(deviceAddr,0x0A,data);
  data ^= 0x10; // E
  delayMicroseconds(1);
  SetMCPReg(deviceAddr,0x0A,data);
  delay(1);
}

void WriteLCDByte( byte deviceAddr, byte bdata ) {
  SendToLCD(deviceAddr,bdata >> 4);
  SendToLCD(deviceAddr,bdata & 0x0F);
}

void Deuligne::init( void ) {
  TWBR = ((CPU_FREQ / TWI_FREQ_MCP23008) - 16) / 2;
  dataPlusMask = 0; // initial: 0
  last_ddramaddr=0;
  SetMCPReg(myAddress,0x05,0x0C); // set CONFREG (0x05) to 0
  SetMCPReg(myAddress,0x00,0x00); // set IOREG (0x00) to 0
  //
  delay(50);
  SendToLCD(myAddress,0x03); 
  delay(5);
  SendToLCD(myAddress,0x03);
  delayMicroseconds(100);
  SendToLCD(myAddress,0x03);
  delay(5);
  SendToLCD(myAddress,0x02);
  WriteLCDByte(myAddress,0x28);
  WriteLCDByte(myAddress,0x08);
  WriteLCDByte(myAddress,0x0C); // turn on, cursor off, no blinking
  delayMicroseconds(60);
  WriteLCDByte(myAddress,LCD_CLEARDISPLAY); // clear display
  delay(3);  
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);
  backLight(1);
  display();
}

void Deuligne::backLight( bool turnOn ) {
  dataPlusMask = turnOn ? (dataPlusMask | 0x80) : (dataPlusMask & ~0x80);
  SetMCPReg(myAddress,0x0A,dataPlusMask);  
}

#if HANDLE_UTF8_CHAR >= 1
void Deuligne::loadAndPrintExtendedChar(uint8_t idx)
{
	static uint8_t  cgram_slot[8] = {255,255,255,255,255,255,255,255};
	static uint8_t cgram_usage[8] = {0};
	int i;
	uint8_t min=255,min_slot=0;
	for (i=0;i<8;i++) {
		if (cgram_slot[i]==idx) { /* Special char found on CGRAM slot <idx>, just print it */
			dataPlusMask |= 0x40; // RS
			WriteLCDByte(myAddress, i);
			dataPlusMask ^= 0x40; // RS
			last_ddramaddr+=1;
			cgram_usage[i]++;
			return;
		}
	}

	for (i=0;i<8;i++) {
		if (cgram_slot[i]==255) { /* Special char has not been found, so find an empty slot and load it */
			cgram_slot[i] = idx;
			command(LCD_SETCGRAMADDR | (i << 3));
			for (uint8_t k=0; k<8; k++) {
				dataPlusMask |= 0x40; // RS
				WriteLCDByte(myAddress,widechars[idx].display[k]);
				dataPlusMask ^= 0x40; // RS
			}
			command(LCD_SETDDRAMADDR | last_ddramaddr);
			dataPlusMask |= 0x40; // RS
			WriteLCDByte(myAddress, i);
			dataPlusMask ^= 0x40; // RS
			last_ddramaddr+=1;
			cgram_usage[i]=1;
			return;
		}
	}

	/* if we got here, no empty slot is available. So empty the least used one and replace it by our needed char */
	for (i=0;i<8;i++) {
		if (cgram_usage[i] < min) {
			min_slot = i;
			min=cgram_usage[i];
		}
	}
	cgram_slot[min_slot] = idx;
	command(LCD_SETCGRAMADDR | (min_slot << 3));
	for (uint8_t k=0; k<8; k++) {
		dataPlusMask |= 0x40; // RS
		WriteLCDByte(myAddress,widechars[idx].display[k]);
		dataPlusMask ^= 0x40; // RS
	}
	command(LCD_SETDDRAMADDR | last_ddramaddr);
	dataPlusMask |= 0x40; // RS
	WriteLCDByte(myAddress, min_slot);
	dataPlusMask ^= 0x40; // RS
	last_ddramaddr+=1;
	cgram_usage[min_slot]=1;
	return;
}

void Deuligne::handle_utf8(uint8_t value)
{
	static uint8_t  bytes[4] = { 0, 0 , 0, 0};
	static uint8_t  byte_index=0;
	int             match=0;
	uint16_t i,j;


	if (value < 0x80) return; /* Sanity check, shall not happen */ 

	bytes[byte_index] = value;
	for (i=0; i < byte_index; i++)
	{
		for (j=0 ; j < (sizeof(widechars)/sizeof(widechar_t)); j++) {
			if (widechars[j].chr[i] == bytes[i]) {
				if (widechars[j].chr[i+1] == bytes[i+1]) {
					match++;
					if (widechars[j].chr[i+2] == 0) {
						loadAndPrintExtendedChar(j);
						goto reinit;
					}
				}
			}
		}
		if (match == 0)
			goto reinit;
	}
	byte_index++;
	return;
reinit:
	byte_index=0;
	bytes[0]=bytes[1]=bytes[2]=bytes[3]=0;
	return;
}
#endif


SNOOT_RETURN Deuligne::write( uint8_t value ) {
#if  HANDLE_UTF8_CHAR >= 1
	if (value>=0x80) { // **all** UTF8 codes are >= to 128 (no byte within a UTF8 char is between 0-127 .
		handle_utf8(value);
	} else {
#else
    {
#endif
		dataPlusMask |= 0x40; // RS
		WriteLCDByte(myAddress,value);
		dataPlusMask ^= 0x40; // RS
		last_ddramaddr+=1;
	}
	return 1;
}

void Deuligne::clear() {
  command(CMD_CLR);
  last_ddramaddr=0;
}

void Deuligne::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {

}

void Deuligne::setCursor(uint8_t col, uint8_t row) {
  uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row > myNumLines ) {
    row = myNumLines-1;    // we count rows starting w/0
  }
  last_ddramaddr = (col + row_offsets[row]);
  command(LCD_SETDDRAMADDR | last_ddramaddr);
}

void Deuligne::command( uint8_t command ) {
  // RS - leave low
  WriteLCDByte(myAddress,command);
  delay(1);
}


/**
 *  From LiquidCrystal (official arduino) Library
 **/

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void Deuligne::createChar(uint8_t location, const uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (uint8_t i=0; i<8; i++) {
	dataPlusMask |= 0x40; // RS
	WriteLCDByte(myAddress,charmap[i]);
	dataPlusMask ^= 0x40; // RS
  }
}



// Turn the display on/off (quickly)
void Deuligne::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Deuligne::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void Deuligne::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Deuligne::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void Deuligne::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Deuligne::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void Deuligne::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void Deuligne::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void Deuligne::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void Deuligne::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void Deuligne::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void Deuligne::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

void Deuligne::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  last_ddramaddr = 0;
  delayMicroseconds(2000);  // this command takes a long time!
}


// Get Joystick value
// Convert ADC value to key number : 
// 0: Right Key
// 1: Up Key
// 2: Down Key
// 3: Left Key
// 4: Select Key

int8_t Deuligne::get_key(){
  int adc_key_in = analogRead(0);    // read the value from the sensor  

  int8_t k = 0;

  //   determine which key is pressed
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (adc_key_in < adc_key_val[k])
    {

      return k;
    }
  }

  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed

  return k;
}
