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

#ifndef OLEDDisplay_I2C
#define OLEDDisplay_I2C

#include "OLEDDisplay.h"

#ifdef squix78_OLED_SoftwareI2C
#include <SoftwareI2C.h>
SoftwareI2C squix78_OLED_Wire;
#else
#include <Wire.h>
#define squix78_OLED_Wire Wire
#endif

void OLEDDisplay::rawDataWrite(uint8_t data) { squix78_OLED_Wire.write(data); }
void OLEDDisplay::startDataWrite() { squix78_OLED_Wire.beginTransmission(_address); }
void OLEDDisplay::endDataWrite() { squix78_OLED_Wire.endTransmission(); }

void OLEDDisplay::i2cScan()
{
  for(uint8_t address=1; address<128; ++address)
  {
    squix78_OLED_Wire.beginTransmission(address);
    squix78_OLED_Wire.write((uint8_t)0); // register 0
    squix78_OLED_Wire.endTransmission();
    uint8_t device_found = (squix78_OLED_Wire.requestFrom(address, 1) == 1); // read 1 byte of data
    squix78_OLED_Wire.endTransmission();

    if(device_found)
    {
        if ((address == 0x3C) || (address == 0x3D))
        {
          _address = address;
          return;
        }
    }
  }
  
  // this should not be needed, but the address defaults to 0x3C if it wasnt found in the scan
  _address = 0x3C; 
}

bool OLEDDisplay::connect(uint32_t baudrate)
{
  #ifdef squix78_OLED_SoftwareI2C
  squix78_OLED_Wire.init(_sda, _scl);
  #endif
  squix78_OLED_Wire.begin();
  // Let's use ~700khz if ESP8266 is in 160Mhz mode
  // this will be limited to ~400khz if the ESP8266 in 80Mhz mode.
  squix78_OLED_Wire.setClock(baudrate);
  return true;
}

void OLEDDisplay::sendCommand(uint8_t command)
{
  squix78_OLED_Wire.beginTransmission(_address);
  //squix78_OLED_Wire.write(0x80); // not 0x00?
  squix78_OLED_Wire.write((uint8_t)0x00); // Command byte = 0x40
  squix78_OLED_Wire.write(command);
  squix78_OLED_Wire.endTransmission();
}

void OLEDDisplay::sendData(uint8_t data, uint16_t len)
{
  squix78_OLED_Wire.beginTransmission(_address);
  squix78_OLED_Wire.write((uint8_t)0x40); // Data byte = 0x40
  for (uint16_t i=0; i<len; ++i)
  {
    squix78_OLED_Wire.write(data);
  }
  squix78_OLED_Wire.endTransmission();
}

void OLEDDisplay::sendData(uint8_t * data, uint16_t startIdx, uint16_t len)
{
  squix78_OLED_Wire.beginTransmission(_address);
  squix78_OLED_Wire.write((uint8_t)0x40); // Data byte = 0x40
  for (uint16_t i=startIdx; i<startIdx+len; ++i)
  {
    squix78_OLED_Wire.write(data[i]);
  }
  squix78_OLED_Wire.endTransmission();
}

#endif
