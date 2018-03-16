/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 by Daniel Eichhorn
 * Copyright (c) 2016 by Fabrice Weinberg
 * Copyright (c) 2017 by Mark Cooke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Credits for parts of this code go to Mike Rankin. Thank you so much for
 * sharing!
 */

#ifndef OLEDDISPLAY_h
#define OLEDDISPLAY_h

#include <Arduino.h>
#include "OLEDDisplay_enums.h"
#include "OLEDDisplayFonts.h"

#ifdef __DEBUG
#define DEBUG_OLEDDISPLAY(...) Serial.print(__VA_ARGS__)
#else
#define DEBUG_OLEDDISPLAY(...)
#endif

// Display settings
#ifndef OLEDDISPLAY_DISPLAY_WIDTH
#define OLEDDISPLAY_DISPLAY_WIDTH 128
#define OLEDDISPLAY_DISPLAY_HEIGHT 64
#endif

#define OLEDDISPLAY_DISPLAY_BUFFER_SIZE (OLEDDISPLAY_DISPLAY_HEIGHT * OLEDDISPLAY_DISPLAY_WIDTH / 8)

// Disable DOUBLE BUFFERING by default
#define OLEDDISPLAY_REDUCE_MEMORY
#ifndef OLEDDISPLAY_REDUCE_MEMORY
#define OLEDDISPLAY_DOUBLE_BUFFER
#endif

// Header Values
#define JUMPTABLE_BYTES 4

#define JUMPTABLE_LSB 1
#define JUMPTABLE_SIZE 2
#define JUMPTABLE_WIDTH 3
#define JUMPTABLE_START 4

#define WIDTH_POS 0
#define HEIGHT_POS 1
#define FIRST_CHAR_POS 2
#define CHAR_NUM_POS 3

// Display commands
#define CHARGEPUMP 0x8D

#define COLUMNADDR 0x21
#define PAGEADDR 0x22

#define COMSCANDEC 0xC8
#define COMSCANINC 0xC0
#define DISPLAYALLON 0xA5
#define DISPLAYALLON_RESUME 0xA4
#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define EXTERNALVCC 0x1
#define INVERTDISPLAY 0xA7
#define MEMORYMODE 0x20
#define NORMALDISPLAY 0xA6
#define SEGREMAP 0xA0
#define SETCOMPINS 0xDA
#define SETCONTRAST 0x81
#define SETDISPLAYCLOCKDIV 0xD5
#define SETDISPLAYOFFSET 0xD3
#define SETHIGHCOLUMN 0x10
#define SETLOWCOLUMN 0x00
#define SETMULTIPLEX 0xA8
#define SETPRECHARGE 0xD9
#define SETSEGMENTREMAP 0xA1
#define SETSTARTLINE 0x40
#define SETVCOMDETECT 0xDB
#define SWITCHCAPVCC 0x2
#define DEACTIVATESCROLL 0x2

#if (OLEDDISPLAY_CHIPSET == OLEDDISPLAY_TYPE_SH1106) // currently these values are unused
#define SH1106_SET_PUMP_VOLTAGE 0X30
#define SH1106_SET_PUMP_MODE 0XAD
#define SH1106_PUMP_ON 0X8B
#define SH1106_PUMP_OFF 0X8A
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
   {                        \
      int16_t t = a;        \
      a         = b;        \
      b         = t;        \
   }
#endif

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

enum OLEDDISPLAY_COLOR
{
   BLACK   = 0,
   WHITE   = 1,
   INVERSE = 2
};

enum OLEDDISPLAY_TEXT_ALIGNMENT
{
   TEXT_ALIGN_LEFT        = 0,
   TEXT_ALIGN_RIGHT       = 1,
   TEXT_ALIGN_CENTER      = 2,
   TEXT_ALIGN_CENTER_BOTH = 3
};

class OLEDDisplay : public Print
{
  public:
   // Initialize the display
   bool init();

   void setScreenSize(uint8_t display_width = OLEDDISPLAY_DISPLAY_WIDTH, uint8_t display_height = OLEDDISPLAY_DISPLAY_HEIGHT);
   uint8_t getDisplayHeight() { return _display_height; }
   uint8_t getDisplayWidth() { return _display_width; }

   // Free the memory used by the display
   void end();

   // Cycle through the initialization
   void resetDisplay(void);

   /* Drawing functions */
   // Sets the color of all pixel operations
   void setColor(OLEDDISPLAY_COLOR color);

   // Draw a pixel at given position
   void setPixel(int16_t x, int16_t y);

   // Draw a line from position 0 to position 1
   void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

   // Draw the border of a rectangle at the given location
   void drawRect(int16_t x, int16_t y, int16_t width, int16_t height);

   // Fill the rectangle
   void fillRect(int16_t x, int16_t y, int16_t width, int16_t height);

   // Draw the border of a circle
   void drawCircle(int16_t x, int16_t y, int16_t radius);

   // Draw all Quadrants specified in the quads bit mask
   void drawCircleQuads(int16_t x0, int16_t y0, int16_t radius, uint8_t quads);

   // Fill circle
   void fillCircle(int16_t x, int16_t y, int16_t radius);

   // Draw a line horizontally
   void drawHorizontalLine(int16_t x, int16_t y, int16_t length);

   // Draw a lin vertically
   void drawVerticalLine(int16_t x, int16_t y, int16_t length);

   // Draws a rounded progress bar with the outer dimensions given by width and
   // height. Progress is
   // a unsigned byte value between 0 and 100
   void drawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress);

   // Draw a bitmap in the internal image format
   void drawFastImage(int16_t x, int16_t y, int16_t width, int16_t height, const char *image);

   // Draw a XBM
   void drawXbm(int16_t x, int16_t y, int16_t width, int16_t height, const char *xbm);

   /* Text functions */

   // Draws a string at the given location
   void drawString(int16_t x, int16_t y, String text);

   // Draws a String with a maximum width at the given location.
   // If the given String is wider than the specified width
   // The text will be wrapped to the next line at a space or dash
   void drawStringMaxWidth(int16_t x, int16_t y, uint16_t maxLineWidth, String text);

   // Returns the width of the const char* with the current
   // font settings
   uint16_t getStringWidth(const char *text, uint16_t length);

   // Convencience method for the const char version
   uint16_t getStringWidth(String text);

   // Specifies relative to which anchor point
   // the text is rendered. Available constants:
   // TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT,
   // TEXT_ALIGN_CENTER_BOTH
   void setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT textAlignment);

   // Sets the current font. Available default fonts
   // ArialMT_Plain_10, ArialMT_Plain_16, ArialMT_Plain_24
   void setFont(const char *fontData);

   /* Display functions */

   // Turn the display on
   void displayOn(void);

   // Turn the display off
   void displayOff(void);

   // Turn the display & charge pump off
   void sleep();

   // Turn the display & charge pump on
   void wake();

   // Inverted display mode
   void invertDisplay(void);

   // Normal display mode
   void normalDisplay(void);

   // Set display contrast
   void setContrast(char contrast);

   // Turn the display upside down
   void flipScreenVertically();

   // Write the buffer to the display memory
   void display(void);

   // Clear the local pixel buffer
   void clear(void);

   // Log buffer implementation

   // This will define the lines and characters you can
   // print to the screen. When you exeed the buffer size (lines * chars)
   // the output may be truncated due to the size constraint.
   bool setLogBuffer(uint16_t lines, uint16_t chars);

   // Draw the log buffer at position (x, y)
   void drawLogBuffer(uint16_t x, uint16_t y);

   // Implementent needed function to be compatible with Print class
   size_t write(uint8_t c);
   size_t write(const char *s);

   uint8_t *buffer;

#ifdef OLEDDISPLAY_DOUBLE_BUFFER
   uint8_t *buffer_back;
#endif

  protected:
   uint8_t _address;
   uint8_t _sda;
   uint8_t _scl;
   int8_t _rst;
   uint8_t _dc;
   uint8_t _cs;

   OLEDDISPLAY_TEXT_ALIGNMENT textAlignment = TEXT_ALIGN_LEFT;
   OLEDDISPLAY_COLOR color                  = WHITE;

   const char *fontData = ArialMT_Plain_10;

   // State values for logBuffer
   uint16_t logBufferSize            = 0;
   uint16_t logBufferFilled          = 0;
   uint16_t logBufferLine            = 0;
   uint16_t logBufferMaxLines        = 0;
   char *logBuffer                   = NULL;
   const uint8_t TRANSFER_BLOCK_SIZE = 16;

   uint8_t display_mode;

   uint8_t _display_width, _display_height;
   uint16_t _display_buffer_size;

   // Send a command to the display (low level function)
   void sendCommand(uint8_t com);
   void sendData(uint8_t data, uint16_t len);
   void sendData(uint8_t *data, uint16_t startIdx, uint16_t len);
   void rawDataWrite(uint8_t c);
   void startDataWrite();
   void endDataWrite();
   void displayPaged(uint8_t minBoundX, uint8_t maxBoundX, uint8_t minBoundY, uint8_t maxBoundY);
   void displayNormal(uint8_t minBoundX, uint8_t maxBoundX, uint8_t minBoundY, uint8_t maxBoundY);

   // virtual void sendBlockData(uint8_t com);

   // Connect to the display
   bool connect(uint32_t baudrate = 700000);

   void i2cScan();

   // Send all the init commands
   void sendInitCommands();

   // converts utf8 characters to extended ascii
   static char *utf8ascii(String s);
   static byte utf8ascii(byte ascii);

   void inline drawInternal(int16_t xMove, int16_t yMove, int16_t width, int16_t height,
                            const char *data, uint16_t offset, uint16_t bytesInData)
       __attribute__((always_inline));

   void drawStringInternal(int16_t xMove, int16_t yMove, char *text, uint16_t textLength,
                           uint16_t textWidth);
};

void OLEDDisplay::setScreenSize(uint8_t display_width, uint8_t display_height)
{
   _display_width       = display_width;
   _display_height      = display_height;
   _display_buffer_size = (_display_height * _display_width / 8);
}

bool OLEDDisplay::init()
{
   DEBUG_OLEDDISPLAY("OLEDDISPLAY_MODE = ");
   DEBUG_OLEDDISPLAY((OLEDDISPLAY_MODE == OLEDDISPLAY_MODE_I2C)?"I2C":(OLEDDISPLAY_MODE == OLEDDISPLAY_MODE_SPI)?"SPI":"BRZO");
   DEBUG_OLEDDISPLAY("\n");

   DEBUG_OLEDDISPLAY("OLEDDISPLAY_CHIPSET = ");
   DEBUG_OLEDDISPLAY((OLEDDISPLAY_CHIPSET == OLEDDISPLAY_TYPE_SH1106)?"SH1106":"SSD1306");
   DEBUG_OLEDDISPLAY("\n");

   // display mode = PAGED for a SSD1306 chipset and a small screen,
   // or if a SH1106 chipset and any screen
   display_mode = (OLEDDISPLAY_CHIPSET == OLEDDISPLAY_TYPE_SH1106) ? OLEDDISPLAY_DISPLAY_PAGED : (_display_width > 64) ? OLEDDISPLAY_DISPLAY_NORMAL : OLEDDISPLAY_DISPLAY_PAGED;

   DEBUG_OLEDDISPLAY("display_mode = ");
   DEBUG_OLEDDISPLAY((display_mode == OLEDDISPLAY_DISPLAY_NORMAL)?"Normal":"Paged");
   DEBUG_OLEDDISPLAY("\n");

   DEBUG_OLEDDISPLAY("SIZE : ");
   DEBUG_OLEDDISPLAY(_display_width);
   DEBUG_OLEDDISPLAY("x");
   DEBUG_OLEDDISPLAY(_display_height);
   DEBUG_OLEDDISPLAY("\n");

   if (!this->connect())
   {
      DEBUG_OLEDDISPLAY("[OLEDDISPLAY][init] Can't establish connection to display\n");
      return false;
   }
   this->buffer = (uint8_t *)malloc(sizeof(uint8_t) * _display_buffer_size);
   if (!this->buffer)
   {
      DEBUG_OLEDDISPLAY("[OLEDDISPLAY][init] Not enough memory to create display\n");
      return false;
   }

#ifdef OLEDDISPLAY_DOUBLE_BUFFER
   this->buffer_back = (uint8_t *)malloc(sizeof(uint8_t) * _display_buffer_size);
   if (!this->buffer_back)
   {
      DEBUG_OLEDDISPLAY("[OLEDDISPLAY][init] Not enough memory to create back buffer\n");
      free(this->buffer);
      return false;
   }
#endif

   sendInitCommands();
   resetDisplay();

   return true;
}

void OLEDDisplay::end()
{
   if (this->buffer) free(this->buffer);
#ifdef OLEDDISPLAY_DOUBLE_BUFFER
   if (this->buffer_back) free(this->buffer_back);
#endif
}

void OLEDDisplay::resetDisplay(void)
{
   clear();
#ifdef OLEDDISPLAY_DOUBLE_BUFFER
   memset(buffer_back, 1, _display_buffer_size);
#endif
   display();
}

void OLEDDisplay::setColor(OLEDDISPLAY_COLOR color) { this->color = color; }

void OLEDDisplay::setPixel(int16_t x, int16_t y)
{
   if (x >= 0 && x < _display_width && y >= 0 && y < _display_height)
   {
      switch (color)
      {
         case WHITE:
            buffer[x + (y / 8) * _display_width] |= (1 << (y & 7));
            break;
         case BLACK:
            buffer[x + (y / 8) * _display_width] &= ~(1 << (y & 7));
            break;
         case INVERSE:
            buffer[x + (y / 8) * _display_width] ^= (1 << (y & 7));
            break;
      }
   }
}

// Bresenham's algorithm - thx wikipedia and Adafruit_GFX
void OLEDDisplay::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
   int16_t steep = abs(y1 - y0) > abs(x1 - x0);
   if (steep)
   {
      _swap_int16_t(x0, y0);
      _swap_int16_t(x1, y1);
   }

   if (x0 > x1)
   {
      _swap_int16_t(x0, x1);
      _swap_int16_t(y0, y1);
   }

   int16_t dx, dy;
   dx = x1 - x0;
   dy = abs(y1 - y0);

   int16_t err = dx / 2;
   int16_t ystep;

   if (y0 < y1)
   {
      ystep = 1;
   }
   else
   {
      ystep = -1;
   }

   for (; x0 <= x1; x0++)
   {
      if (steep)
      {
         setPixel(y0, x0);
      }
      else
      {
         setPixel(x0, y0);
      }
      err -= dy;
      if (err < 0)
      {
         y0 += ystep;
         err += dx;
      }
   }
}

void OLEDDisplay::drawRect(int16_t x, int16_t y, int16_t width, int16_t height)
{
   drawHorizontalLine(x, y, width);
   drawVerticalLine(x, y, height);
   drawVerticalLine(x + width - 1, y, height);
   drawHorizontalLine(x, y + height - 1, width);
}

void OLEDDisplay::fillRect(int16_t xMove, int16_t yMove, int16_t width, int16_t height)
{
   for (int16_t x = xMove; x < xMove + width; x++)
   {
      drawVerticalLine(x, yMove, height);
   }
}

void OLEDDisplay::drawCircle(int16_t x0, int16_t y0, int16_t radius)
{
   int16_t x = 0, y = radius;
   int16_t dp = 1 - radius;
   do
   {
      if (dp < 0)
         dp = dp + 2 * (++x) + 3;
      else
         dp = dp + 2 * (++x) - 2 * (--y) + 5;

      setPixel(x0 + x, y0 + y);  // For the 8 octants
      setPixel(x0 - x, y0 + y);
      setPixel(x0 + x, y0 - y);
      setPixel(x0 - x, y0 - y);
      setPixel(x0 + y, y0 + x);
      setPixel(x0 - y, y0 + x);
      setPixel(x0 + y, y0 - x);
      setPixel(x0 - y, y0 - x);

   } while (x < y);

   setPixel(x0 + radius, y0);
   setPixel(x0, y0 + radius);
   setPixel(x0 - radius, y0);
   setPixel(x0, y0 - radius);
}

void OLEDDisplay::drawCircleQuads(int16_t x0, int16_t y0, int16_t radius, uint8_t quads)
{
   int16_t x = 0, y = radius;
   int16_t dp = 1 - radius;
   while (x < y)
   {
      if (dp < 0)
         dp = dp + 2 * (++x) + 3;
      else
         dp = dp + 2 * (++x) - 2 * (--y) + 5;
      if (quads & 0x1)
      {
         setPixel(x0 + x, y0 - y);
         setPixel(x0 + y, y0 - x);
      }
      if (quads & 0x2)
      {
         setPixel(x0 - y, y0 - x);
         setPixel(x0 - x, y0 - y);
      }
      if (quads & 0x4)
      {
         setPixel(x0 - y, y0 + x);
         setPixel(x0 - x, y0 + y);
      }
      if (quads & 0x8)
      {
         setPixel(x0 + x, y0 + y);
         setPixel(x0 + y, y0 + x);
      }
   }
   if (quads & 0x1 && quads & 0x8)
   {
      setPixel(x0 + radius, y0);
   }
   if (quads & 0x4 && quads & 0x8)
   {
      setPixel(x0, y0 + radius);
   }
   if (quads & 0x2 && quads & 0x4)
   {
      setPixel(x0 - radius, y0);
   }
   if (quads & 0x1 && quads & 0x2)
   {
      setPixel(x0, y0 - radius);
   }
}

void OLEDDisplay::fillCircle(int16_t x0, int16_t y0, int16_t radius)
{
   int16_t x = 0, y = radius;
   int16_t dp = 1 - radius;
   do
   {
      if (dp < 0)
         dp = dp + 2 * (++x) + 3;
      else
         dp = dp + 2 * (++x) - 2 * (--y) + 5;

      drawHorizontalLine(x0 - x, y0 - y, 2 * x);
      drawHorizontalLine(x0 - x, y0 + y, 2 * x);
      drawHorizontalLine(x0 - y, y0 - x, 2 * y);
      drawHorizontalLine(x0 - y, y0 + x, 2 * y);

   } while (x < y);
   drawHorizontalLine(x0 - radius, y0, 2 * radius);
}

void OLEDDisplay::drawHorizontalLine(int16_t x, int16_t y, int16_t length)
{
   if (y < 0 || y >= _display_height)
   {
      return;
   }

   if (x < 0)
   {
      length += x;
      x = 0;
   }

   if ((x + length) > _display_width)
   {
      length = (_display_width - x);
   }

   if (length <= 0)
   {
      return;
   }

   uint8_t *bufferPtr = buffer;
   bufferPtr += (y >> 3) * _display_width;
   bufferPtr += x;

   uint8_t drawBit = 1 << (y & 7);

   switch (color)
   {
      case WHITE:
         while (length--)
         {
            *bufferPtr++ |= drawBit;
         };
         break;
      case BLACK:
         drawBit = ~drawBit;
         while (length--)
         {
            *bufferPtr++ &= drawBit;
         };
         break;
      case INVERSE:
         while (length--)
         {
            *bufferPtr++ ^= drawBit;
         };
         break;
   }
}

void OLEDDisplay::drawVerticalLine(int16_t x, int16_t y, int16_t length)
{
   if (x < 0 || x >= _display_width) return;

   if (y < 0)
   {
      length += y;
      y = 0;
   }

   if ((y + length) > _display_height)
   {
      length = (_display_height - y);
   }

   if (length <= 0) return;

   uint8_t yOffset = y & 7;
   uint8_t drawBit;
   uint8_t *bufferPtr = buffer;

   bufferPtr += (y >> 3) * _display_width;
   bufferPtr += x;

   if (yOffset)
   {
      yOffset = 8 - yOffset;
      drawBit = ~(0xFF >> (yOffset));

      if (length < yOffset)
      {
         drawBit &= (0xFF >> (yOffset - length));
      }

      switch (color)
      {
         case WHITE:
            *bufferPtr |= drawBit;
            break;
         case BLACK:
            *bufferPtr &= ~drawBit;
            break;
         case INVERSE:
            *bufferPtr ^= drawBit;
            break;
      }

      if (length < yOffset) return;

      length -= yOffset;
      bufferPtr += _display_width;
   }

   if (length >= 8)
   {
      switch (color)
      {
         case WHITE:
         case BLACK:
            drawBit = (color == WHITE) ? 0xFF : 0x00;
            do
            {
               *bufferPtr = drawBit;
               bufferPtr += _display_width;
               length -= 8;
            } while (length >= 8);
            break;
         case INVERSE:
            do
            {
               *bufferPtr = ~(*bufferPtr);
               bufferPtr += _display_width;
               length -= 8;
            } while (length >= 8);
            break;
      }
   }

   if (length > 0)
   {
      drawBit = (1 << (length & 7)) - 1;
      switch (color)
      {
         case WHITE:
            *bufferPtr |= drawBit;
            break;
         case BLACK:
            *bufferPtr &= ~drawBit;
            break;
         case INVERSE:
            *bufferPtr ^= drawBit;
            break;
      }
   }
}

void OLEDDisplay::drawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                                  uint8_t progress)
{
   uint16_t radius       = height / 2;
   uint16_t xRadius      = x + radius;
   uint16_t yRadius      = y + radius;
   uint16_t doubleRadius = 2 * radius;
   uint16_t innerRadius  = radius - 2;

   setColor(WHITE);
   drawCircleQuads(xRadius, yRadius, radius, 0b00000110);
   drawHorizontalLine(xRadius, y, width - doubleRadius + 1);
   drawHorizontalLine(xRadius, y + height, width - doubleRadius + 1);
   drawCircleQuads(x + width - radius, yRadius, radius, 0b00001001);

   uint16_t maxProgressWidth = (width - doubleRadius - 1) * progress / 100;

   fillCircle(xRadius, yRadius, innerRadius);
   fillRect(xRadius + 1, y + 2, maxProgressWidth, height - 3);
   fillCircle(xRadius + maxProgressWidth, yRadius, innerRadius);
}

void OLEDDisplay::drawFastImage(int16_t xMove, int16_t yMove, int16_t width, int16_t height,
                                const char *image)
{
   drawInternal(xMove, yMove, width, height, image, 0, 0);
}

void OLEDDisplay::drawXbm(int16_t xMove, int16_t yMove, int16_t width, int16_t height,
                          const char *xbm)
{
   int16_t widthInXbm = (width + 7) / 8;
   uint8_t data;

   for (int16_t y = 0; y < height; y++)
   {
      for (int16_t x = 0; x < width; x++)
      {
         if (x & 7)
         {
            data >>= 1;  // Move a bit
         }
         else
         {  // Read new data every 8 bit
            data = pgm_read_byte(xbm + (x / 8) + y * widthInXbm);
         }
         // if there is a bit draw it
         if (data & 0x01)
         {
            setPixel(xMove + x, yMove + y);
         }
      }
   }
}

void OLEDDisplay::drawStringInternal(int16_t xMove, int16_t yMove, char *text, uint16_t textLength,
                                     uint16_t textWidth)
{
   uint8_t textHeight       = pgm_read_byte(fontData + HEIGHT_POS);
   uint8_t firstChar        = pgm_read_byte(fontData + FIRST_CHAR_POS);
   uint16_t sizeOfJumpTable = pgm_read_byte(fontData + CHAR_NUM_POS) * JUMPTABLE_BYTES;

   uint8_t cursorX = 0;
   uint8_t cursorY = 0;

   switch (textAlignment)
   {
      case TEXT_ALIGN_CENTER_BOTH:
         yMove -= textHeight >> 1;
      // Fallthrough
      case TEXT_ALIGN_CENTER:
         xMove -= textWidth >> 1;  // divide by 2
         break;
      case TEXT_ALIGN_RIGHT:
         xMove -= textWidth;
         break;
   }

   // Don't draw anything if it is not on the screen.
   if (xMove + textWidth < 0 || xMove > _display_width)
   {
      return;
   }
   if (yMove + textHeight < 0 || yMove > _display_height)
   {
      return;
   }

   for (uint16_t j = 0; j < textLength; j++)
   {
      int16_t xPos = xMove + cursorX;
      int16_t yPos = yMove + cursorY;

      byte code = text[j];
      if (code >= firstChar)
      {
         byte charCode = code - firstChar;

         // 4 Bytes per char code
         byte msbJumpToChar = pgm_read_byte(fontData + JUMPTABLE_START +
                                            charCode * JUMPTABLE_BYTES);  // MSB  \ JumpAddress
         byte lsbJumpToChar = pgm_read_byte(fontData + JUMPTABLE_START +
                                            charCode * JUMPTABLE_BYTES + JUMPTABLE_LSB);  // LSB /
         byte charByteSize = pgm_read_byte(fontData + JUMPTABLE_START + charCode * JUMPTABLE_BYTES +
                                           JUMPTABLE_SIZE);  // Size
         byte currentCharWidth = pgm_read_byte(
             fontData + JUMPTABLE_START + charCode * JUMPTABLE_BYTES + JUMPTABLE_WIDTH);  // Width

         // Test if the char is drawable
         if (!(msbJumpToChar == 255 && lsbJumpToChar == 255))
         {
            // Get the position of the char data
            uint16_t charDataPosition =
                JUMPTABLE_START + sizeOfJumpTable + ((msbJumpToChar << 8) + lsbJumpToChar);
            drawInternal(xPos, yPos, currentCharWidth, textHeight, fontData, charDataPosition,
                         charByteSize);
         }

         cursorX += currentCharWidth;
      }
   }
}

void OLEDDisplay::drawString(int16_t xMove, int16_t yMove, String strUser)
{
   uint16_t lineHeight = pgm_read_byte(fontData + HEIGHT_POS);

   // char* text must be freed!
   char *text = utf8ascii(strUser);

   uint16_t yOffset = 0;
   // If the string should be centered vertically too
   // we need to now how heigh the string is.
   if (textAlignment == TEXT_ALIGN_CENTER_BOTH)
   {
      uint16_t lb = 0;
      // Find number of linebreaks in text
      for (uint16_t i = 0; text[i] != 0; i++)
      {
         lb += (text[i] == 10);
      }
      // Calculate center
      yOffset = (lb * lineHeight) / 2;
   }

   uint16_t line  = 0;
   char *textPart = strtok(text, "\n");
   while (textPart != NULL)
   {
      uint16_t length = strlen(textPart);
      drawStringInternal(xMove, yMove - yOffset + (line++) * lineHeight, textPart, length,
                         getStringWidth(textPart, length));
      textPart = strtok(NULL, "\n");
   }
   free(text);
}

void OLEDDisplay::drawStringMaxWidth(int16_t xMove, int16_t yMove, uint16_t maxLineWidth,
                                     String strUser)
{
   uint16_t firstChar  = pgm_read_byte(fontData + FIRST_CHAR_POS);
   uint16_t lineHeight = pgm_read_byte(fontData + HEIGHT_POS);

   char *text = utf8ascii(strUser);

   uint16_t length       = strlen(text);
   uint16_t lastDrawnPos = 0;
   uint16_t lineNumber   = 0;
   uint16_t strWidth     = 0;

   uint16_t preferredBreakpoint = 0;
   uint16_t widthAtBreakpoint   = 0;

   for (uint16_t i = 0; i < length; i++)
   {
      strWidth += pgm_read_byte(fontData + JUMPTABLE_START +
                                (text[i] - firstChar) * JUMPTABLE_BYTES + JUMPTABLE_WIDTH);

      // Always try to break on a space or dash
      if (text[i] == ' ' || text[i] == '-')
      {
         preferredBreakpoint = i;
         widthAtBreakpoint   = strWidth;
      }

      if (strWidth >= maxLineWidth)
      {
         if (preferredBreakpoint == 0)
         {
            preferredBreakpoint = i;
            widthAtBreakpoint   = strWidth;
         }
         drawStringInternal(xMove, yMove + (lineNumber++) * lineHeight, &text[lastDrawnPos],
                            preferredBreakpoint - lastDrawnPos, widthAtBreakpoint);
         lastDrawnPos = preferredBreakpoint + 1;
         // It is possible that we did not draw all letters to i so we need
         // to account for the width of the chars from `i - preferredBreakpoint`
         // by calculating the width we did not draw yet.
         strWidth            = strWidth - widthAtBreakpoint;
         preferredBreakpoint = 0;
      }
   }

   // Draw last part if needed
   if (lastDrawnPos < length)
   {
      drawStringInternal(xMove, yMove + lineNumber * lineHeight, &text[lastDrawnPos],
                         length - lastDrawnPos,
                         getStringWidth(&text[lastDrawnPos], length - lastDrawnPos));
   }

   free(text);
}

uint16_t OLEDDisplay::getStringWidth(const char *text, uint16_t length)
{
   uint16_t firstChar = pgm_read_byte(fontData + FIRST_CHAR_POS);

   uint16_t stringWidth = 0;
   uint16_t maxWidth    = 0;

   while (length--)
   {
      stringWidth += pgm_read_byte(fontData + JUMPTABLE_START +
                                   (text[length] - firstChar) * JUMPTABLE_BYTES + JUMPTABLE_WIDTH);
      if (text[length] == 10)
      {
         maxWidth    = max(maxWidth, stringWidth);
         stringWidth = 0;
      }
   }

   return max(maxWidth, stringWidth);
}

uint16_t OLEDDisplay::getStringWidth(String strUser)
{
   char *text      = utf8ascii(strUser);
   uint16_t length = strlen(text);
   uint16_t width  = getStringWidth(text, length);
   free(text);
   return width;
}

void OLEDDisplay::setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT textAlignment)
{
   this->textAlignment = textAlignment;
}

void OLEDDisplay::setFont(const char *fontData) { this->fontData = fontData; }

void OLEDDisplay::displayOn(void) { sendCommand(DISPLAYON); }

void OLEDDisplay::displayOff(void) { sendCommand(DISPLAYOFF); }

void OLEDDisplay::sleep()
{
    sendCommand(DISPLAYOFF);
#if (OLEDDISPLAY_CHIPSET == OLEDDISPLAY_TYPE_SH1106)
    sendCommand(SH1106_PUMP_OFF);
#else
    sendCommand(CHARGEPUMP);            // turn off charge pump
    sendCommand(0x10);
#endif
    //delay(100);                   // power stabilisation delay
    // power down Vbat
    // delay(50);
    // power down Vdd
}

void OLEDDisplay::wake()
{
#if (OLEDDISPLAY_CHIPSET == OLEDDISPLAY_TYPE_SH1106)
    sendCommand(SH1106_PUMP_ON);
#else
    sendCommand(CHARGEPUMP);            // 0x8D - enable charge pump
    sendCommand(0x14);
#endif    
    sendCommand(DISPLAYON);
    //delay(100);
    display();
}

void OLEDDisplay::invertDisplay(void) { sendCommand(INVERTDISPLAY); }

void OLEDDisplay::normalDisplay(void) { sendCommand(NORMALDISPLAY); }

void OLEDDisplay::setContrast(char contrast)
{
   sendCommand(SETCONTRAST);
   sendCommand(contrast);
}

void OLEDDisplay::flipScreenVertically()
{
   sendCommand(SEGREMAP | 0x01);
   sendCommand(COMSCANDEC);  // Rotate screen 180 Deg
}

void OLEDDisplay::clear(void) { memset(buffer, 0, _display_buffer_size); }

void OLEDDisplay::drawLogBuffer(uint16_t xMove, uint16_t yMove)
{
   uint16_t lineHeight = pgm_read_byte(fontData + HEIGHT_POS);
   // Always align left
   setTextAlignment(TEXT_ALIGN_LEFT);

   // State values
   uint16_t length  = 0;
   uint16_t line    = 0;
   uint16_t lastPos = 0;

   for (uint16_t i = 0; i < this->logBufferFilled; i++)
   {
      // Everytime we have a \n print
      if (this->logBuffer[i] == 10)
      {
         length++;
         // Draw string on line `line` from lastPos to length
         // Passing 0 as the lenght because we are in TEXT_ALIGN_LEFT
         drawStringInternal(xMove, yMove + (line++) * lineHeight, &this->logBuffer[lastPos], length,
                            0);
         // Remember last pos
         lastPos = i;
         // Reset length
         length = 0;
      }
      else
      {
         // Count chars until next linebreak
         length++;
      }
   }
   // Draw the remaining string
   if (length > 0)
   {
      drawStringInternal(xMove, yMove + line * lineHeight, &this->logBuffer[lastPos], length, 0);
   }
}

bool OLEDDisplay::setLogBuffer(uint16_t lines, uint16_t chars)
{
   if (logBuffer != NULL) free(logBuffer);
   uint16_t size = lines * chars;
   if (size > 0)
   {
      this->logBufferLine     = 0;      // Lines printed
      this->logBufferMaxLines = lines;  // Lines max printable
      this->logBufferSize     = size;   // Total number of characters the buffer can hold
      this->logBuffer         = (char *)malloc(size * sizeof(uint8_t));
      if (!this->logBuffer)
      {
         DEBUG_OLEDDISPLAY(
             "[OLEDDISPLAY][setLogBuffer] Not enough memory to create log "
             "buffer\n");
         return false;
      }
   }
   return true;
}

size_t OLEDDisplay::write(uint8_t c)
{
   if (this->logBufferSize > 0)
   {
      // Don't waste space on \r\n line endings, dropping \r
      if (c == 13) return 1;

      bool maxLineNotReached = this->logBufferLine < this->logBufferMaxLines;
      bool bufferNotFull     = this->logBufferFilled < this->logBufferSize;

      // Can we write to the buffer?
      if (bufferNotFull && maxLineNotReached)
      {
         this->logBuffer[logBufferFilled] = utf8ascii(c);
         this->logBufferFilled++;
         // Keep track of lines written
         if (c == 10) this->logBufferLine++;
      }
      else
      {
         // Max line number is reached
         if (!maxLineNotReached) this->logBufferLine--;

         // Find the end of the first line
         uint16_t firstLineEnd = 0;
         for (uint16_t i = 0; i < this->logBufferFilled; i++)
         {
            if (this->logBuffer[i] == 10)
            {
               // Include last char too
               firstLineEnd = i + 1;
               break;
            }
         }
         // If there was a line ending
         if (firstLineEnd > 0)
         {
            // Calculate the new logBufferFilled value
            this->logBufferFilled = logBufferFilled - firstLineEnd;
            // Now we move the lines infront of the buffer
            memcpy(this->logBuffer, &this->logBuffer[firstLineEnd], logBufferFilled);
         }
         else
         {
            // Let's reuse the buffer if it was full
            if (!bufferNotFull)
            {
               this->logBufferFilled = 0;
            }  // else {
               //  Nothing to do here
               //}
         }
         write(c);
      }
   }
   // We are always writing all uint8_t to the buffer
   return 1;
}

size_t OLEDDisplay::write(const char *str)
{
   if (str == NULL) return 0;
   size_t length = strlen(str);
   for (size_t i = 0; i < length; i++)
   {
      write(str[i]);
   }
   return length;
}

// Private functions
void OLEDDisplay::sendInitCommands(void)
{
   #if (OLEDDISPLAY_CHIPSET == OLEDDISPLAY_TYPE_SH1106) // currently these values are unused
   //sendCommand(SH1106_SET_PUMP_VOLTAGE); // 0X30
   //sendCommand(SH1106_SET_PUMP_MODE); // 0XAD
   #endif
   sendCommand(DISPLAYOFF);
   sendCommand(SETDISPLAYCLOCKDIV);
   sendCommand(0x80); // sendCommand(0xF0);  // Increase speed of the display max ~96Hz (normally 0x80)
   sendCommand(SETMULTIPLEX);
   sendCommand(_display_height - 1);
   sendCommand(SETDISPLAYOFFSET);
   sendCommand(0x00);
   sendCommand(SETSTARTLINE);
   sendCommand(CHARGEPUMP);
   sendCommand(0x14);
   sendCommand(MEMORYMODE);
   sendCommand(0x00);
   sendCommand(SEGREMAP | 0x01);
   sendCommand(COMSCANDEC);  // changed from COMSCANINC
   sendCommand(SETCOMPINS);
   sendCommand(0x12);
   sendCommand(SETCONTRAST);
   sendCommand(0xCF);
   sendCommand(SETPRECHARGE);
   sendCommand(0x1F);             // changed from 0xF1
   sendCommand(SETVCOMDETECT);  // 0xDB
   sendCommand(0x40);
   sendCommand(DISPLAYALLON_RESUME);
   sendCommand(NORMALDISPLAY);
   sendCommand(DEACTIVATESCROLL);
   sendCommand(DISPLAYON);
}

void inline OLEDDisplay::drawInternal(int16_t xMove, int16_t yMove, int16_t width, int16_t height,
                                      const char *data, uint16_t offset, uint16_t bytesInData)
{
   if (width < 0 || height < 0) return;
   if (yMove + height < 0 || yMove > _display_height) return;
   if (xMove + width < 0 || xMove > _display_width) return;

   uint8_t rasterHeight = 1 + ((height - 1) >> 3);  // fast ceil(height / 8.0)
   int8_t yOffset       = yMove & 7;

   bytesInData = bytesInData == 0 ? width * rasterHeight : bytesInData;

   int16_t initYMove  = yMove;
   int8_t initYOffset = yOffset;

   for (uint16_t i = 0; i < bytesInData; i++)
   {
      // Reset if next horizontal drawing phase is started.
      if (i % rasterHeight == 0)
      {
         yMove   = initYMove;
         yOffset = initYOffset;
      }

      byte currentByte = pgm_read_byte(data + offset + i);

      int16_t xPos = xMove + (i / rasterHeight);
      int16_t yPos = ((yMove >> 3) + (i % rasterHeight)) * _display_width;

      int16_t yScreenPos = yMove + yOffset;
      int16_t dataPos    = xPos + yPos;

      if (dataPos >= 0 && dataPos < _display_buffer_size && xPos >= 0 && xPos < _display_width)
      {
         if (yOffset >= 0)
         {
            switch (this->color)
            {
               case WHITE:
                  buffer[dataPos] |= currentByte << yOffset;
                  break;
               case BLACK:
                  buffer[dataPos] &= ~(currentByte << yOffset);
                  break;
               case INVERSE:
                  buffer[dataPos] ^= currentByte << yOffset;
                  break;
            }
            if (dataPos < (_display_buffer_size - _display_width))
            {
               switch (this->color)
               {
                  case WHITE:
                     buffer[dataPos + _display_width] |= currentByte >> (8 - yOffset);
                     break;
                  case BLACK:
                     buffer[dataPos + _display_width] &= ~(currentByte >> (8 - yOffset));
                     break;
                  case INVERSE:
                     buffer[dataPos + _display_width] ^= currentByte >> (8 - yOffset);
                     break;
               }
            }
         }
         else
         {
            // Make new offset position
            yOffset = -yOffset;

            switch (this->color)
            {
               case WHITE:
                  buffer[dataPos] |= currentByte >> yOffset;
                  break;
               case BLACK:
                  buffer[dataPos] &= ~(currentByte >> yOffset);
                  break;
               case INVERSE:
                  buffer[dataPos] ^= currentByte >> yOffset;
                  break;
            }

            // Prepare for next iteration by moving one block up
            yMove -= 8;

            // and setting the new yOffset
            yOffset = 8 - yOffset;
         }

         yield();
      }
   }
}

// Code form http://playground.arduino.cc/Main/Utf8ascii
uint8_t OLEDDisplay::utf8ascii(byte ascii)
{
   static uint8_t LASTCHAR;

   if (ascii < 128)
   {  // Standard ASCII-set 0..0x7F handling
      LASTCHAR = 0;
      return ascii;
   }

   uint8_t last = LASTCHAR;  // get last char
   LASTCHAR     = ascii;

   switch (last)
   {  // conversion depnding on first UTF8-character
      case 0xC2:
         return (ascii);
         break;
      case 0xC3:
         return (ascii | 0xC0);
         break;
      case 0x82:
         if (ascii == 0xAC) return (0x80);  // special case Euro-symbol
   }

   return 0;  // otherwise: return zero, if character has to be ignored
}

// You need to free the char!
char *OLEDDisplay::utf8ascii(String str)
{
   uint16_t k      = 0;
   uint16_t length = str.length() + 1;

   // Copy the string into a char array
   char *s = (char *)malloc(length * sizeof(char));
   if (!s)
   {
      DEBUG_OLEDDISPLAY(
          "[OLEDDISPLAY][utf8ascii] Can't allocate another char array. Drop "
          "support for UTF-8.\n");
      return (char *)str.c_str();
   }
   str.toCharArray(s, length);

   length--;

   for (uint16_t i = 0; i < length; i++)
   {
      char c = utf8ascii(s[i]);
      if (c != 0)
      {
         s[k++] = c;
      }
   }

   s[k] = 0;

   // This will leak 's' be sure to free it in the calling function.
   return s;
}

void OLEDDisplay::display(void)
{
   uint8_t minBoundY = 0xFF;
   uint8_t maxBoundY = 0x00;

   uint8_t minBoundX = 0xFF;
   uint8_t maxBoundX = 0x00;

#ifdef OLEDDISPLAY_DOUBLE_BUFFER
   // Calculate the Y bounding box of changes
   // and copy buffer[pos] to buffer_back[pos];
   for (uint8_t y = 0; y < (_display_height / 8); y++)
   {
      for (uint8_t x = 0; x < _display_width; x++)
      {
         uint16_t pos = x + y * _display_width;
         if (buffer[pos] != buffer_back[pos])
         {
            minBoundY = min(minBoundY, y);
            maxBoundY = max(maxBoundY, y);
            minBoundX = min(minBoundX, x);
            maxBoundX = max(maxBoundX, x);
         }
         buffer_back[pos] = buffer[pos];
      }
      yield();
   }

   // If the minBoundY wasn't updated
   // we can savely assume that buffer_back[pos] == buffer[pos]
   // holdes true for all values of pos
   if (minBoundY == 0xFF) return;
#endif
   switch (display_mode)
   {
      case OLEDDISPLAY_DISPLAY_NORMAL:
         // Note : minBound{X,Y} and maxBound{X,Y} are only used when double buffering
         displayNormal(minBoundX, maxBoundX, minBoundY, maxBoundY);
         break;
      default:
         displayPaged(minBoundX, maxBoundX, minBoundY, maxBoundY);
   }
}

void OLEDDisplay::displayNormal(uint8_t minBoundX, uint8_t maxBoundX, uint8_t minBoundY, uint8_t maxBoundY)
{
#ifdef OLEDDISPLAY_DOUBLE_BUFFER
   sendCommand(COLUMNADDR);
   sendCommand(minBoundX);
   sendCommand(maxBoundX);

   sendCommand(PAGEADDR);
   sendCommand(minBoundY);
   sendCommand(maxBoundY);

   #if (OLEDDISPLAY_MODE !=OLEDDISPLAY_MODE_SPI)
   startDataWrite();
   uint8_t k = 0;
   #endif
   for (uint16_t y = minBoundY; y <= maxBoundY; y++)
   {
      #if (OLEDDISPLAY_MODE == OLEDDISPLAY_MODE_SPI)
      sendData(buffer, y * _display_width + minBoundX, maxBoundX - minBoundX);
      #else
      uint16_t x = 0;
      for (x = 0; x <= maxBoundX - minBoundX; x+= TRANSFER_BLOCK_SIZE)
      {
        sendData(buffer, y * _display_width + minBoundX, maxBoundX - minBoundX);
      }
      sendData(buffer, y * _display_width + x, (maxBoundX - minBoundX) - x);
      #endif
      yield();
   }
   #if (OLEDDISPLAY_MODE != OLEDDISPLAY_MODE_SPI)
   if (k != 0)
   {
      endDataWrite();
   }
   #endif
#else
   sendCommand(COLUMNADDR);
   sendCommand(0x0);
   sendCommand(_display_width - 1);

   sendCommand(PAGEADDR);
   sendCommand(0x0);
   sendCommand((_display_height / 8) - 1);

   #if(OLEDDISPLAY_MODE == OLEDDISPLAY_MODE_SPI)
   sendData(buffer, 0, _display_buffer_size);
   #else  // I2C or BRZO
   for (uint16_t i = 0; i < _display_buffer_size; i += TRANSFER_BLOCK_SIZE)
   {
      sendData(buffer, i, TRANSFER_BLOCK_SIZE);
   }
   #endif
#endif
}

void OLEDDisplay::displayPaged(uint8_t minBoundX, uint8_t maxBoundX, uint8_t minBoundY, uint8_t maxBoundY)
{
#ifdef OLEDDISPLAY_DOUBLE_BUFFER

   #if (OLEDDISPLAY_MODE != OLEDDISPLAY_MODE_SPI)
   startDataWrite();
   uint8_t k = 0;
   #endif

   for (uint16_t y = minBoundY; y <= maxBoundY; y++)
   {
      sendCommand(0xB0 | y);        // send page address
      sendCommand((minBoundX + (uint8_t)OLEDDISPLAY_CHIPSET)&(0x0F);  // send column address
      sendCommand((0x12 - (uint8_t)OLEDDISPLAY_CHIPSET)|(minBoundX >> 4));

      #if (OLEDDISPLAY_MODE == OLEDDISPLAY_MODE_SPI)
      sendData(buffer, y * _display_width + minBoundX, maxBoundX - minBoundX);
      #else
      uint16_t x = 0;
      for (x = 0; x <= maxBoundX - minBoundX; x+= TRANSFER_BLOCK_SIZE)
      {
        sendData(buffer, y * _display_width + minBoundX, maxBoundX - minBoundX);
      }
      sendData(buffer, y * _display_width + x, (maxBoundX - minBoundX) - x);
      #endif
      yield();
   }   
   #if (OLEDDISPLAY_MODE !=OLEDDISPLAY_MODE_SPI)
   if (k != 0)
   {
      endDataWrite();
   }
   #endif
#else
   for (uint8_t r = 0; r < _display_height / 8; r++)
   {
      sendCommand(0xB0 | r);        // send page address
      sendCommand(0x00 + (uint8_t)OLEDDISPLAY_CHIPSET);  // send column address
      sendCommand(0x12 - (uint8_t)OLEDDISPLAY_CHIPSET);

      #if (OLEDDISPLAY_MODE == OLEDDISPLAY_MODE_SPI)
      sendData(buffer, r * _display_width, _display_width);
      #else  // I2C or BRZO
      for (uint16_t c = r * _display_width; c < (r + 1) * _display_width; c += TRANSFER_BLOCK_SIZE)
      {
         sendData(buffer, c, TRANSFER_BLOCK_SIZE);
      }
      #endif
      yield();
   }
#endif
}

#endif
