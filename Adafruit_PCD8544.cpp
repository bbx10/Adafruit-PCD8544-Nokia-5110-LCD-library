/*********************************************************************
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
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

//#include <Wire.h>
#if defined(ESP8266)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#ifdef __AVR__
  #include <util/delay.h>
#endif

#ifndef _BV
  #define _BV(x) (1 << (x))
#endif

#include <stdlib.h>

#include <Adafruit_GFX.h>
#include "Adafruit_PCD8544.h"

// If the SPI library has transaction support, these functions
// establish settings and protect from interference from other
// libraries.  Otherwise, they simply do nothing.
#ifdef SPI_HAS_TRANSACTION
#if defined (ESP8266)
SPISettings spiSettings = SPISettings(4000000, MSBFIRST, SPI_MODE0);
#elif defined (__STM32F1__)
SPISettings spiSettings = SPISettings(4500000, MSBFIRST, SPI_MODE0);
#else
SPISettings spiSettings = SPISettings(PCD8544_SPI_CLOCK_DIV, MSBFIRST, SPI_MODE0);
#endif
/*static inline void spi_begin(void) __attribute__((always_inline));
static inline void spi_begin(void) {
  _SPI->beginTransaction(spiSettings);
}
static inline void spi_end(void) __attribute__((always_inline));
static inline void spi_end(void) {
  _SPI->endTransaction();
}*/
/*#else
#define spi_begin()
#define spi_end()*/
#endif

inline void Adafruit_PCD8544::spi_begin(void) {
#ifdef SPI_HAS_TRANSACTION
  _SPI->beginTransaction(spiSettings);
#endif
}
inline void Adafruit_PCD8544::spi_end(void) {
#ifdef SPI_HAS_TRANSACTION
  _SPI->endTransaction();
#endif
}

// the memory buffer for the LCD
uint8_t pcd8544_buffer[LCDWIDTH * LCDHEIGHT / 8] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFF, 0xFC, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80, 0xC0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x87, 0x8F, 0x9F, 0x9F, 0xFF, 0xFF, 0xFF,
0xC1, 0xC0, 0xE0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE,
0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0xC0, 0xE0, 0xF1, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x0F, 0x87,
0xE7, 0xFF, 0xFF, 0xFF, 0x1F, 0x1F, 0x3F, 0xF9, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xF0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
0x7E, 0x3F, 0x3F, 0x0F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFC, 0xF0, 0xE0, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFC, 0xF0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01,
0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x1F, 0x3F, 0x7F, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


// reduces how much is refreshed, which speeds it up!
// originally derived from Steve Evans/JCW's mod but cleaned up and
// optimized
//#define enablePartialUpdate

#ifdef enablePartialUpdate
static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;
#endif

void Adafruit_PCD8544::scroll(uint8_t direction, uint8_t pixels, uint8_t fillColor){
  uint8_t oldpixels;
  uint8_t newpixels;
  uint8_t _fillColor = (fillColor & 1) * 255;
  int i, j;
  if (pixels==0) {
    return;
  }
  switch(direction){
    case SCROLL_DOWN:
      if (pixels>7) {
          if (pixels>=LCDHEIGHT) {
            memset(pcd8544_buffer, _fillColor, LCDWIDTH*LCDHEIGHT/8);
            return;
          }
        memmove(&pcd8544_buffer[LCDWIDTH*((pixels)>>3)],&pcd8544_buffer[0],LCDWIDTH*((LCDHEIGHT-pixels)>>3));
        memset(pcd8544_buffer, _fillColor, LCDWIDTH*((pixels)>>3));
      }
      if (pixels % 8 != 0) {
        for (i=0;i<LCDWIDTH;i++) {
          newpixels = 0;
          for (j=0;j<LCDHEIGHT/8;j++) {
            oldpixels = pcd8544_buffer[i+j*LCDWIDTH]>>(8-(pixels % 8));
            pcd8544_buffer[i+j*LCDWIDTH] = (pcd8544_buffer[i+j*LCDWIDTH] << (pixels % 8))|newpixels;
            newpixels=oldpixels;
          }
        }
      }
      if(fillColor) for(i=0; i<pixels; i++) Adafruit_GFX::drawFastHLine(0, i, LCDWIDTH - 1, fillColor);
      break;
    case SCROLL_UP:
      if (pixels>7) {
          if (pixels>=LCDHEIGHT) {
            memset(pcd8544_buffer, _fillColor, LCDWIDTH*LCDHEIGHT/8);
            return;
          }
        memmove(&pcd8544_buffer[LCDWIDTH*((pixels)>>3)],&pcd8544_buffer[0],LCDWIDTH*((LCDHEIGHT-pixels)>>3));
        memset(pcd8544_buffer, _fillColor, LCDWIDTH*((pixels)>>3));
      }
      if (pixels % 8 != 0) {
        for (i=0;i<LCDWIDTH;i++) {
          newpixels = 0;
          for (j=0;j<LCDHEIGHT/8;j++) {
            oldpixels = pcd8544_buffer[i+(LCDHEIGHT/8-1-j)*LCDWIDTH] << (8-(pixels % 8));
            pcd8544_buffer[i+(LCDHEIGHT/8-1-j)*LCDWIDTH] = (pcd8544_buffer[i+(LCDHEIGHT/8-1-j)*LCDWIDTH] >> (pixels % 8))|newpixels;
            newpixels=oldpixels;
          }
        }
      }
      if(fillColor) for(i=0; i<pixels; i++) Adafruit_GFX::drawFastHLine(0, LCDHEIGHT - i - 1, LCDWIDTH - 1, fillColor);
      break;
    case SCROLL_LEFT:
      if (pixels>=LCDWIDTH) {
      memset(pcd8544_buffer, _fillColor, LCDWIDTH*LCDHEIGHT/8);
      return;
      }
      for (i=0;i<LCDHEIGHT/8;i++) {
        memmove(&pcd8544_buffer[i*LCDWIDTH],&pcd8544_buffer[i*LCDWIDTH+pixels],LCDWIDTH-pixels);
        memset(&pcd8544_buffer[i*LCDWIDTH-pixels], _fillColor, pixels);
      }
      for(i=0; i<pixels; i++) Adafruit_GFX::drawFastVLine(LCDWIDTH - i - 1, 0, LCDHEIGHT, fillColor);
      break;
    case SCROLL_RIGHT:
      if (pixels>=LCDWIDTH) {
        memset(pcd8544_buffer, _fillColor, LCDWIDTH*LCDHEIGHT/8);
        return;
        }
      for (i=0;i<LCDHEIGHT/8;i++) {
        memmove(&pcd8544_buffer[i*LCDWIDTH+pixels],&pcd8544_buffer[i*LCDWIDTH],LCDWIDTH-pixels);
      }
      for(i=0; i<pixels; i++) Adafruit_GFX::drawFastVLine(i, 0, LCDHEIGHT, fillColor);
      break;
    default:
      break;
  }
}

uint8_t * Adafruit_PCD8544::getPixelBuffer(){
  return (uint8_t *) &pcd8544_buffer;
}

void Adafruit_PCD8544::powerSaving(boolean i) {
  if(!i){
	if (isHardwareSPI()) spi_begin();
	command(PCD8544_FUNCTIONSET);
	if (isHardwareSPI()) spi_end();
	}
  else {
	clearDisplayRAM();
	if (isHardwareSPI()) spi_begin();
	command(PCD8544_FUNCTIONSET | PCD8544_POWERDOWN);
	if (isHardwareSPI()) spi_end();
	}
}

static void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax) {
#ifdef enablePartialUpdate
  if (xmin < xUpdateMin) xUpdateMin = xmin;
  if (xmax > xUpdateMax) xUpdateMax = xmax;
  if (ymin < yUpdateMin) yUpdateMin = ymin;
  if (ymax > yUpdateMax) yUpdateMax = ymax;
#endif
}

Adafruit_PCD8544::Adafruit_PCD8544(int8_t SCLK, int8_t DIN, int8_t DC,
    int8_t CS, int8_t RST) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {
  _din = DIN;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = CS;
}

Adafruit_PCD8544::Adafruit_PCD8544(int8_t SCLK, int8_t DIN, int8_t DC,
    int8_t RST) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {
  _din = DIN;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = -1;
}

Adafruit_PCD8544::Adafruit_PCD8544(int8_t DC, int8_t CS, int8_t RST, SPIClass *useSPI):
//Adafruit_PCD8544::Adafruit_PCD8544(int8_t DC, int8_t CS, int8_t RST):
  Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {
  // -1 for din and sclk specify using hardware SPI
  _din = -1;
  _sclk = -1;
  _dc = DC;
  _rst = RST;
  _cs = CS;
  _SPI = useSPI;
}


// the most basic function, set a single pixel
void Adafruit_PCD8544::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    return;

  int16_t t;
  switch(rotation){
    case 1:
      t = x;
      x = y;
      y =  LCDHEIGHT - 1 - t;
      break;
    case 2:
      x = LCDWIDTH - 1 - x;
      y = LCDHEIGHT - 1 - y;
      break;
    case 3:
      t = x;
      x = LCDWIDTH - 1 - y;
      y = t;
      break;
  }

  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color)
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] |= _BV(y%8);
  else
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] &= ~_BV(y%8);

  updateBoundingBox(x,y,x,y);
}


// the most basic function, get a single pixel
uint8_t Adafruit_PCD8544::getPixel(int8_t x, int8_t y) {
  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return 0;

  return (pcd8544_buffer[x+ (y/8)*LCDWIDTH] >> (y%8)) & 0x1;
}


void Adafruit_PCD8544::begin(uint8_t contrast, uint8_t bias) {
  if (isHardwareSPI()) {
    // Setup hardware _SPI->
    _SPI->begin();
#ifdef ESP8266
    // Datasheet says 4 MHz is max SPI clock speed
    _SPI->setFrequency(4000000);
#elif defined (__STM32F1__)
    _SPI->setClockDivider(SPI_CLOCK_DIV16);
#else
    _SPI->setClockDivider(PCD8544_SPI_CLOCK_DIV);
#endif
    _SPI->setDataMode(SPI_MODE0);
    _SPI->setBitOrder(MSBFIRST);

#ifdef USE_FAST_PINIO
    csport    = portOutputRegister(digitalPinToPort(_cs));
    cspinmask = digitalPinToBitMask(_cs);
    dcport    = portOutputRegister(digitalPinToPort(_dc));
    dcpinmask = digitalPinToBitMask(_dc);
#endif
  }
  else {
    // Setup software _SPI->

    // Set software SPI specific pin outputs.
    pinMode(_din, OUTPUT);
    pinMode(_sclk, OUTPUT);

#ifdef USE_FAST_PINIO
    // Set software SPI ports and masks.
    clkport     = portOutputRegister(digitalPinToPort(_sclk));
    clkpinmask  = digitalPinToBitMask(_sclk);
    mosiport    = portOutputRegister(digitalPinToPort(_din));
    mosipinmask = digitalPinToBitMask(_din);
    csport    = portOutputRegister(digitalPinToPort(_cs));
    cspinmask = digitalPinToBitMask(_cs);
    dcport    = portOutputRegister(digitalPinToPort(_dc));
    dcpinmask = digitalPinToBitMask(_dc);
#endif
  }

  // Set common pin outputs.
  pinMode(_dc, OUTPUT);
  if (_rst > 0)
      pinMode(_rst, OUTPUT);
  if (_cs > 0)
      pinMode(_cs, OUTPUT);

  // toggle RST low to reset
  if (_rst > 0) {
    digitalWrite(_rst, LOW);
    delay(500);
    digitalWrite(_rst, HIGH);
  }

  if (isHardwareSPI()) spi_begin();

  // get into the EXTENDED mode!
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );

  // LCD bias select (4 is optimal?)
  command(PCD8544_SETBIAS | bias);

  // set VOP
  if (contrast > 0x7f)
    contrast = 0x7f;

  command( PCD8544_SETVOP | contrast); // Experimentally determined


  // normal mode
  command(PCD8544_FUNCTIONSET);

  // Set display to Normal
  command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

  if (isHardwareSPI()) spi_end();

  // initial display line
  // set page address
  // set column address
  // write display data

  // set up a bounding box for screen updates

  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
  // Push out pcd8544_buffer to the Display (will show the AFI logo)
  display();
}


inline void Adafruit_PCD8544::spiWrite(uint8_t d) {
  if (isHardwareSPI()) {
    // Hardware SPI write.
    _SPI->transfer(d);
  }
  else {
#ifndef USE_FAST_PINIO
    // Software SPI write with bit banging.
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      digitalWrite(_sclk, LOW);
      if (d & bit) digitalWrite(_din, HIGH);
      else         digitalWrite(_din, LOW);
      digitalWrite(_sclk, HIGH);
    }
#else
    // Software SPI write with bit banging.
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      *clkport &= ~clkpinmask;
      if(d & bit) *mosiport |=  mosipinmask;
      else        *mosiport &= ~mosipinmask;
      *clkport |=  clkpinmask;
    }
#endif
  }
}

bool Adafruit_PCD8544::isHardwareSPI() {
  return (_din == -1 && _sclk == -1);
}

void Adafruit_PCD8544::command(uint8_t c) {
  #ifdef USE_FAST_PINIO
  *dcport &= ~dcpinmask; //LOW
  if(_cs) *csport &= ~cspinmask; //LOW
  spiWrite(c);
  if(_cs) *csport |= cspinmask; //HIGH
  #else
  digitalWrite(_dc, LOW);
  if (_cs > 0)
    digitalWrite(_cs, LOW);
  spiWrite(c);
  if (_cs > 0)
    digitalWrite(_cs, HIGH);
  #endif
}

void Adafruit_PCD8544::data(uint8_t c) {
  #ifdef USE_FAST_PINIO
  *dcport |= dcpinmask;
  if(_cs) *csport &= ~cspinmask;
  spiWrite(c);
  if(_cs) *csport |= cspinmask;
  #else
  digitalWrite(_dc, HIGH);
  if (_cs > 0)
    digitalWrite(_cs, LOW);
  spiWrite(c);
  if (_cs > 0)
    digitalWrite(_cs, HIGH);
  #endif
}

void Adafruit_PCD8544::setContrast(uint8_t val) {
  if (val > 0x7f) {
    val = 0x7f;
  }
  if (isHardwareSPI()) spi_begin();
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
  command( PCD8544_SETVOP | val);
  command(PCD8544_FUNCTIONSET);
  if (isHardwareSPI()) spi_end();
}

void Adafruit_PCD8544::display(void) {
  uint8_t col, maxcol, p;

  if (isHardwareSPI()) spi_begin();
#ifndef enablePartialUpdate
  command(PCD8544_SETYADDR);
  command(PCD8544_SETXADDR);
#ifdef USE_FAST_PINIO
  *dcport |= dcpinmask;
  if(_cs) *csport &= ~cspinmask;
#else
  digitalWrite(_dc, HIGH);
  if (_cs > 0)
    digitalWrite(_cs, LOW);
#endif
  if(isHardwareSPI())
//Add hardware-specific optimized methods for pushing those 504 bytes over SPI as needed
//This one's for ESP8266
#if defined(ESP8266)
    _SPI->writeBytes(pcd8544_buffer, 504);
//Resort to the default if no optimized method available
#elif defined (__STM32F1__)
    {
      _SPI->setDataSize (0);
      _SPI->dmaSend(pcd8544_buffer, 504);
    }
#else
    for(int i=0; i<504; i++) spiWrite(pcd8544_buffer[i]);
#endif
//Also just resort to the default if no H/W SPI
  else for(int i=0; i<504; i++) spiWrite(pcd8544_buffer[i]);
#ifdef USE_FAST_PINIO
  if(_cs) *csport |= cspinmask;
#else
  if (_cs > 0)
    digitalWrite(_cs, HIGH);
#endif
  command(PCD8544_SETYADDR );  // no idea why this is necessary but it is to finish the last byte?
  spi_end();
#else
  for(p = 0; p < 6; p++) {
    // check if this page is part of update
    if ( yUpdateMin >= ((p+1)*8) ) {
      continue;   // nope, skip it!
    }
    if (yUpdateMax < p*8) {
      break;
    }
    command(PCD8544_SETYADDR | p);

    col = xUpdateMin;
    maxcol = xUpdateMax;
    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH-1;

    command(PCD8544_SETXADDR | col);

    #ifdef USE_FAST_PINIO
    *dcport |= dcpinmask;
    if(_cs) *csport &= ~cspinmask;
    #else
    digitalWrite(_dc, HIGH);
    if (_cs > 0)
      digitalWrite(_cs, LOW);
    #endif
    #ifdef ESP8266
    if(isHardwareSPI()){
      // Align to 32 bits.
      while ((reinterpret_cast<uintptr_t>(&pcd8544_buffer[(LCDWIDTH*p)+col]) & 0X3) && col <= maxcol) {
        spiWrite(pcd8544_buffer[(LCDWIDTH*p)+col]);
        col++;
      }
      _SPI->writeBytes(&pcd8544_buffer[(LCDWIDTH*p)+col], maxcol - col + 1);
    } else
    #elif defined(__STM32F1__)
    if(isHardwareSPI()){
      _SPI->setDataSize (0);
      _SPI->dmaSend((uint8_t *)&pcd8544_buffer[(LCDWIDTH*p)+col], maxcol - col + 1);
    } else
    #endif
    for(; col <= maxcol; col++) {
      spiWrite(pcd8544_buffer[(LCDWIDTH*p)+col]);
    }
    #ifdef USE_FAST_PINIO
    if(_cs) *csport |= cspinmask;
    #else
    if (_cs > 0)
      digitalWrite(_cs, HIGH);
    #endif

  }

  command(PCD8544_SETYADDR );  // no idea why this is necessary but it is to finish the last byte?
  if (isHardwareSPI()) spi_end();
  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT-1;
  yUpdateMax = 0;
#endif
}

// clear everything
void Adafruit_PCD8544::clearDisplay(uint8_t color) {
  if(color) color=255;
  memset(pcd8544_buffer, color, LCDWIDTH*LCDHEIGHT/8);
  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
  cursor_y = cursor_x = 0;
}

void Adafruit_PCD8544::invertDisplay(boolean i) {
  if (isHardwareSPI()) spi_begin();
  command(PCD8544_FUNCTIONSET);
  command(PCD8544_DISPLAYCONTROL | (i ? PCD8544_DISPLAYINVERTED : PCD8544_DISPLAYNORMAL));
  if (isHardwareSPI()) spi_end();
}

void Adafruit_PCD8544::clearDisplayRAM(uint8_t color) {
  uint16_t counter=504;
  
  if (isHardwareSPI()) spi_begin();
  command(PCD8544_SETYADDR);
  command(PCD8544_SETXADDR);
  #ifdef USE_FAST_PINIO
  *dcport |= dcpinmask;
  if(_cs) *csport &= ~cspinmask;
  #else
  digitalWrite(_dc, HIGH);
  if (_cs > 0) digitalWrite(_cs, LOW);
  #endif
  #ifdef ESP8266
  if(isHardwareSPI()) _SPI->writePattern(&color, 1, counter);
  else
  #elif defined (__STM32F1__)
  if(isHardwareSPI()){
  _SPI->setDataSize (0);
  _SPI->dmaSend(&color, 504, 0);
  }
  else
  #endif
  while(counter--) spiWrite(color);
  #ifdef USE_FAST_PINIO
  if(_cs) *csport |= cspinmask;
  #else
  if (_cs > 0) digitalWrite(_cs, HIGH);
  #endif
  command(PCD8544_SETYADDR );  // no idea why this is necessary but it is to finish the last byte?
  if (isHardwareSPI()) spi_end();
}
