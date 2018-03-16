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
*  KX022 Accelerometer library
*
*  @author  Goran Mahovlić
*  @version 1.0
*  @date    26-May-2017
*
*  @author  Mark Cooke
*  @version 1.1
*  @date    18-Sep-2017
*  @brief   Remove separate r and w addressing (didnt work for me)
*           Now allows change in accelerometer range and accel data output rate. 
*           New functions : getAccelXYZ()
*
*  Library for "KX022 Accelerometer library" from Kionix a Rohm group
*    http://www.kionix.com/product/KX022-1020
*/

#ifndef KX022_H
#define KX022_H

#include <Arduino.h>
#include <Wire.h>

#define KX022_ADDR_L 0x1E
#define KX022_ADDR_H 0x1F
#define KX022_ADDR_RESET 0x1D  // ? I cant find the reference to this
#define KX022_WHOAMI_REG 0x0F
#define KX022_WHOAMI_RESPONSE 0x14
#define KX022_CNTL1_REG 0x18
#define KX022_CNTL1_VALUE_STANDBY 0x41
#define KX022_ODCNTL_REG 0x1B
#define KX022_ODCNTL_VALUE 0x02
#define KX022_CNTL3_REG 0x1A
#define KX022_CNTL3_VALUE 0xD8
#define KX022_TILT_TIMER_REG 0x22
#define KX022_TILT_TIMER_VALUE 0x01
#define KX022_CNTL2_REG 0x18
#define KX022_CNTL1_VALUE_OPERATE 0xC1

// Note : 400Hz+ are not available in low power mode
enum KX022_ACC_OUTPUT_RATE
{
   KX022_ACC_OUTPUT_RATE_12_5_HZ,
   KX022_ACC_OUTPUT_RATE_25_HZ,
   KX022_ACC_OUTPUT_RATE_50_HZ,
   KX022_ACC_OUTPUT_RATE_100_HZ,
   KX022_ACC_OUTPUT_RATE_200_HZ,
   KX022_ACC_OUTPUT_RATE_400_HZ,
   KX022_ACC_OUTPUT_RATE_800_HZ,
   KX022_ACC_OUTPUT_RATE_1600_HZ,
   KX022_ACC_OUTPUT_RATE_0_781_HZ,
   KX022_ACC_OUTPUT_RATE_1_563_HZ,
   KX022_ACC_OUTPUT_RATE_3_125_HZ,
   KX022_ACC_OUTPUT_RATE_6_25_HZ
};

#define KX022_ACC_OUTPUT_RATE_MASK 0x00001111

const float KX022_ODR[] = {12.5,  25.0,   50.0,  100.0, 200.0, 400.0,
                           800.0, 1600.0, 0.781, 1.563, 3.125, 6.25};

const float KX022_ACC_SENSITIVITY[] = {16384.0, 8192.0, 4096.0};

#define KX022_RANGE_2G 0b00000000  // range +/-2g
#define KX022_RANGE_4G 0b00001000  // range +/-4g
#define KX022_RANGE_8G 0b00011000  // range +/-8g
#define KX022_RANGE_MASK 0b00011000

#define DATA_OUT_BASE 0x06

template <class T = TwoWire>
class KX022
{
  public:
   KX022(T &i2c_reference, uint8_t i2c_address = KX022_ADDR_H) : _i2c_address(i2c_address), _range(KX022_RANGE_4G) { _i2c = &i2c_reference; }
   // Initialise KX022 sensor - configure sensor setting
   void init(uint8_t range = KX022_RANGE_4G,
             uint8_t rate  = KX022_ACC_OUTPUT_RATE_50_HZ);
   float getAccel(uint8_t channelNum);
   void getAccelXYZ(float (&xyz)[3]);
  protected:
   T * _i2c;
   uint8_t _range;
   uint8_t _i2c_address;
   void writeTwoBytes(uint8_t one, uint8_t two);
   uint8_t getByte(uint8_t reg_address);
   void getRawXYZ(int16_t xyz[3], uint8_t base_reg_location = DATA_OUT_BASE);
};

template <class T>
void KX022<T>::init(uint8_t range, uint8_t rate)
{
   _range = range;
   
   _i2c->begin();

   writeTwoBytes(KX022_CNTL1_REG,
                 (KX022_CNTL1_VALUE_STANDBY & ~KX022_ACC_OUTPUT_RATE_MASK) |
                     (range & KX022_ACC_OUTPUT_RATE_MASK));
   writeTwoBytes(KX022_ODCNTL_REG,
                 (KX022_ODCNTL_VALUE & ~KX022_ACC_OUTPUT_RATE_MASK) |
                     (rate & KX022_ACC_OUTPUT_RATE_MASK));
   writeTwoBytes(KX022_CNTL3_REG, KX022_CNTL3_VALUE);
   writeTwoBytes(KX022_TILT_TIMER_REG, KX022_TILT_TIMER_VALUE);

   // wait for registers to be set prior to coming out of standby = 1.2/ODR
   // (pg39 of the datasheet)
   delay((uint32_t)(1200.0f / KX022_ODR[rate]));
   writeTwoBytes(KX022_CNTL1_REG,
                 (KX022_CNTL1_VALUE_OPERATE & ~KX022_ACC_OUTPUT_RATE_MASK) |
                     (range & KX022_ACC_OUTPUT_RATE_MASK));
}

template <class T>
void KX022<T>::writeTwoBytes(uint8_t one, uint8_t two)
{
   _i2c->beginTransmission(_i2c_address);
   _i2c->write(one);
   _i2c->write(two);
   _i2c->endTransmission();
}

template <class T>
uint8_t KX022<T>::getByte(uint8_t reg_address)
{
   uint8_t value_ = 0;
   _i2c->beginTransmission(_i2c_address);
   _i2c->write(reg_address);
   _i2c->endTransmission();
   _i2c->requestFrom(_i2c_address, 1);
   if (_i2c->available())
   {
      value_ = _i2c->read();
   }
   return value_;
}

template <class T>
void KX022<T>::getRawXYZ(int16_t xyz[3], uint8_t base_reg_location)
{
   _i2c->beginTransmission(_i2c_address);
   _i2c->write(base_reg_location);
   Wire.endTransmission();
   Wire.requestFrom(_i2c_address, 6);
   if (Wire.available())
   {
      xyz[0] = static_cast<int16_t>(Wire.read() | (Wire.read() << 8));
      xyz[1] = static_cast<int16_t>(Wire.read() | (Wire.read() << 8));
      xyz[2] = static_cast<int16_t>(Wire.read() | (Wire.read() << 8));
   }
}

template <class T>
float KX022<T>::getAccel(uint8_t channelNum)
{
   int16_t measurement =
       static_cast<int16_t>((getByte(DATA_OUT_BASE + 1 + 2 * channelNum) << 8) |
                            getByte(DATA_OUT_BASE + 2 * channelNum));
   return static_cast<float>(measurement) / KX022_ACC_SENSITIVITY[_range >> 3];
}

template <class T>
void KX022<T>::getAccelXYZ(float (&xyz)[3])
{
   int16_t xyz_[3] = {0};
   getRawXYZ(xyz_);
   xyz[0] = static_cast<float>(xyz_[0])  / KX022_ACC_SENSITIVITY[_range >> 3];
   xyz[1] = static_cast<float>(xyz_[1])  / KX022_ACC_SENSITIVITY[_range >> 3];
   xyz[2] = static_cast<float>(xyz_[2])  / KX022_ACC_SENSITIVITY[_range >> 3];
}

#endif