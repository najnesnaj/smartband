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

#ifndef OLEDDisplay_enums_h
#define OLEDDisplay_enums_h

/*
typedef enum OLED_MODE { MODE_SPI, MODE_I2C, MODE_BRZO } oled_mode;
typedef enum OLED_TYPE { TYPE_SSD1306 = 0, TYPE_SH1106 = 2 } oled_type;
typedef enum OLED_DISPLAYMODE { DISPLAY_NORMAL, DISPLAY_PAGED } oled_displaymode;
*/

#define OLEDDISPLAY_MODE_SPI 0
#define OLEDDISPLAY_MODE_I2C 1
#define OLEDDISPLAY_MODE_BRZO 2

#define OLEDDISPLAY_TYPE_SSD1306 0
#define OLEDDISPLAY_TYPE_SH1106 2

#define OLEDDISPLAY_DISPLAY_NORMAL 0
#define OLEDDISPLAY_DISPLAY_PAGED 1

#endif
