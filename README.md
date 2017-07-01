Improved Adafruit_PCD8544 - library
===========================================
This is a library with some added functionality over Adafruit's stock library, including performance-optimizations for select hardware.

### Optimized hardware so far ###
- ESP8266 (both hardware- and software-SPI)
- STM32F1 (both hardware- and software-SPI)
- ESP32 (both hardware- and software-SPI)

### New functionality and/or improvements ###
- Implemented invertDisplay()
- Use SPI-transactions when supported
- Add new functions getPixelBuffer(), powerSaving() and scroll()
  - getPixelBuffer() returns the address to the raw 504-byte buffer used for all graphics-operations
  - powerSaving(bool) turns the display's power-saving mode on or off
  - scroll(direction, num_pixels) scrolls the display-buffer's contents. "direction" may be one of SCROLL_UP, SCROLL_DOWN, SCROLL_LEFT or SCROLL_RIGHT (*NB! You cannot combine those, call scroll() multiple times instead.*)
- User-selectable SPI-bus when using hardware-SPI, pseudo-code example as follows:
```
SPIClass mySPI2;
Adafruit_PCD8544 display = Adafruit_PCD8544(5, 15, 4, &mySPI2);

void setup(){
//Write code to initialize mySPI2 as however you please, including selecting the SPI-bus it uses
//On STM32F1, for example, calling "mySPI2.setModule(2);" would make it use the 2nd SPI-bus
//Call display.begin *after* you have set mySPI2 up as you want, not before
display.begin();
}
```

Original README.txt
===========================================
This is a library for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution

To download. click the DOWNLOADS button in the top right corner, rename the uncompressed folder Adafruit_PCD8544. Check that the Adafruit_PCD8544 folder contains Adafruit_PCD8544.cpp and Adafruit_PCD8544.h

Place the Adafruit_PCD8544 library folder your <arduinosketchfolder>/libraries/ folder. You may need to create the libraries subfolder if its your first library. Restart the IDE.

You will also have to download the Adafruit GFX Graphics core which does all the circles, text, rectangles, etc. You can get it from
https://github.com/adafruit/Adafruit-GFX-Library
and download/install that library as well 
