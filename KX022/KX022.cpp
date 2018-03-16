/*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*
*  KX022 Accelerometer library
*
*  @author  Goran Mahovlić
*  @version 1.0
*  @date    26-May-2017
*
*  Library for "KX022 Accelerometer library" from Kionix a Rohm group
*    http://www.kionix.com/product/KX022-1020
*
*/

#include <Wire.h>
#include "KX022.h"

KX022::KX022()
{

    init();
}
 
KX022::~KX022()
{
}
 
void KX022::init()
{
    Wire.begin();

    writeTwoBytes(KX022_Accel_CNTL1_1,KX022_Accel_CNTL1_2);
    writeTwoBytes(KX022_Accel_ODCNTL_1,KX022_Accel_ODCNTL_2);
    writeTwoBytes(KX022_Accel_CNTL3_1,KX022_Accel_CNTL3_2);
    writeTwoBytes(KX022_Accel_TILT_TIMER_1,KX022_Accel_TILT_TIMER_2);
    writeTwoBytes(KX022_Accel_CNTL2_1,KX022_Accel_CNTL2_2);
}

void KX022::writeTwoBytes (int one, int two)
{
    Wire.beginTransmission(KX022_addr_w);
    Wire.write(one);
    Wire.write(two);
    Wire.endTransmission();
}

int KX022::getByte (int address)
{
  int readedValue;
  Wire.beginTransmission(KX022_addr_w);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(KX022_addr_r , 1);  // Or-ed with "1" for read bit
  if(1 <= Wire.available())    // if two bytes were received
  {
    readedValue = Wire.read();
  }
  return readedValue;
}

float KX022::getAccel(int channelNum)
{
  return ((int16_t)((getByte(DATA_OUT_BASE+1 + 2*channelNum)<<8) | (getByte(DATA_OUT_BASE + 2*channelNum)))) / 16384.0;  
}

