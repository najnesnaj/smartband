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

#ifndef OLEDDisplay_BRZO
#define OLEDDisplay_BRZO

#include "OLEDDisplay.h"
#include <brzo_i2c.h>

#if F_CPU == 160000000L
  #define BRZO_I2C_SPEED 1000
#else
  #define BRZO_I2C_SPEED 800
#endif

void OLEDDisplay::rawDataWrite(uint8_t data) { brzo_i2c_write({data}, 1, true); }
void OLEDDisplay::startDataWrite() { brzo_i2c_start_transaction(this->_address, BRZO_I2C_SPEED); }
void OLEDDisplay::endDataWrite() { brzo_i2c_end_transaction(); }

bool OLEDDisplay::connect(uint32_t baudrate)
{
  brzo_i2c_setup(_sda, _scl, 0);
  return true;
}

void OLEDDisplay::sendCommand(uint8_t command)
{
  uint8_t _command[2] = {0x80 /* command mode */, com};
  brzo_i2c_start_transaction(_address, BRZO_I2C_SPEED);
  brzo_i2c_write(_command, 2, true);
  brzo_i2c_end_transaction();
}

void OLEDDisplay::sendData(uint8_t data, uint16_t len)
{
  brzo_i2c_start_transaction(_address, BRZO_I2C_SPEED);
  brzo_i2c_write({0x40}, 1, true);
  for (uint16_t i=0; i<len; ++i)
  {
    brzo_i2c_write({data}, 1, true);
  }
  brzo_i2c_end_transaction();
}

void OLEDDisplay::sendData(uint8_t * data, uint16_t startIdx, uint16_t len)
{
  uint8_t *brzo_buffer;
  brzo_buffer = (uint8_t*)malloc(sizeof(uint8_t) * len+1);
  brzo_buffer[0] = 0x40;
  memcpy(brzo_buffer, &data[startIdx], len);

  brzo_i2c_start_transaction(_address, BRZO_I2C_SPEED);
  brzo_i2c_write(brzo_buffer, len+1, true);
  brzo_i2c_end_transaction();

  free(brzo_buffer);
}

#endif
