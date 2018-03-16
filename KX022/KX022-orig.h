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

#ifndef KX022_H
#define KX022_H

#define KX022_addr_w              (0x3E)
#define KX022_addr_r              (0x3F)
#define KX022_Accel_CNTL1_1       (0x18)
#define KX022_Accel_CNTL1_2       (0x41)
#define KX022_Accel_ODCNTL_1      (0x1B)
#define KX022_Accel_ODCNTL_2      (0x02)
#define KX022_Accel_CNTL3_1       (0x1A)
#define KX022_Accel_CNTL3_2       (0xD8)
#define KX022_Accel_TILT_TIMER_1  (0x22)
#define KX022_Accel_TILT_TIMER_2  (0x01)
#define KX022_Accel_CNTL2_1       (0x18)
#define KX022_Accel_CNTL2_2       (0xC1)

#define DATA_OUT_BASE 0x06

class KX022
{
public:
    /**
    * KX022 constructor
    */
    KX022();
    /**
    * KX022 destructor
    */
    ~KX022();
 
    /** Initializa KX022 sensor
     *
     *  Configure sensor setting
     *
     */
    void init(void);
    float getAccel(int channelNum);
	

private:
	void writeTwoBytes (int one, int two);
	int getByte (int address);
};
 
#endif