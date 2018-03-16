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
 * The above copyright notice and this permission notice shall be included in all
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
 * Credits for parts of this code go to Mike Rankin. Thank you so much for sharing!
 */

#ifndef OLEDDisplay_SPI
#define OLEDDisplay_SPI

#include "OLEDDisplay.h"
#include <SPI.h>

// Configure SPI settings - Max clk frequency for display is 10MHz
SPISettings oledSettings(10000000, MSBFIRST, SPI_MODE0);

void OLEDDisplay::rawDataWrite(uint8_t data) { SPI.transfer(data); }
void OLEDDisplay::startDataWrite() { digitalWrite(_dc, HIGH); digitalWrite(_cs, LOW); }
void OLEDDisplay::endDataWrite() { digitalWrite(_cs, HIGH); }

bool OLEDDisplay::connect(uint32_t baudrate)
{
  if (_rst >= 0)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
  }
  pinMode(_dc, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);

  digitalWrite(_dc, LOW);
  digitalWrite(_cs, HIGH);
  
  SPI.begin();
  #ifdef SPI_HAS_TRANSACTION
    SPI.beginTransaction(oledSettings);
  #else
    SPI.setClockDivider(2);
  #endif

  // Display reset routine
  if (_rst >= 0)
  {
    pinMode(_rst, OUTPUT);  // Set RST pin as OUTPUT
    digitalWrite(_rst, HIGH); // VDD (3.3V) goes high at start
    delay(5);
    digitalWrite(_rst, LOW); // Bring RST low, reset the display
    delay(10); // wait 10ms
    digitalWrite(_rst, HIGH); // Set RST HIGH, bring out of reset
  }

  return true;
}

void OLEDDisplay::sendCommand(uint8_t command)
{
  //digitalWrite(_cs, HIGH);
  digitalWrite(_dc, LOW); // DC LOW for command
  digitalWrite(_cs, LOW);
  SPI.transfer(command);
  digitalWrite(_cs, HIGH);
}

void OLEDDisplay::sendData(uint8_t data, uint16_t len)
{
  digitalWrite(_dc, HIGH);  // DC HIGH for data
  digitalWrite(_cs, LOW);
  for (uint16_t i = 0; i < len; ++i) { SPI.transfer(data);}
  digitalWrite(_cs, HIGH);
}

void OLEDDisplay::sendData(uint8_t * data, uint16_t startIdx, uint16_t len)
{
  digitalWrite(_dc, HIGH);  // DC HIGH for data
  digitalWrite(_cs, LOW);
  for (uint16_t i = startIdx; i < startIdx+len; ++i) { SPI.transfer(data[i]);}
  digitalWrite(_cs, HIGH);
}

#endif
