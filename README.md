# Requirements

* Arduino: http://arduino.cc

# Hardware

Connect the following pins from MCP23008 to LCD

* P0 - D4
* P1 - D5
* P2 - D6
* P3 - D7
* P4 - RS
* P5 - RW (not used, set to 0 to ground for write)
* P6 - Bl (backlight switch)
* P7 - E

# Install

## on Mac

### Using Finder

* In Finder, go to "_Documents_", create folder "_Arduino_" inside, and create folder "_libraries_" inside "_Arduino_" folder
* Download [Deuligne lib](http://github.com/Snootlab/Deuligne/zipball/master), and copy unzipped content - should be a folder named Snootlab-Deuligne-xxxx - to "_libraries_" folder

### Using Terminal

Copy Deuligne lib in your sketchbook, with the default path set in Arduino preferences:

    git clone https://github.com/Snootlab/Deuligne.git
    mkdir -p ~/Documents/Arduino/libraries/
    ln -sf Deuligne ~/Documents/Arduino/libraries/Deuligne
