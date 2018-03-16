#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#define SH1107_EXTERNAL_CONTROL
#if defined(_VARIANT_T28_) | defined(SH1107_EXTERNAL_CONTROL)
  #define SFEOLED_SH1107
#endif
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#if defined(SH1107_EXTERNAL_CONTROL)
  #include "angry_cookie_64x32_xbm.h"
  #define OLED_WIDTH 64
  #define OLED_HEIGHT 128
  #include <SSD1306Spi.h>
  SSD1306Spi oled(8, 9, 10);
#elif defined(_VARIANT_T28_)
  #include "angry_cookie_64x32_xbm.h"
  #define OLED_WIDTH 64
  #define OLED_HEIGHT 128
  #include <SSD1306Spi.h>
  SSD1306Spi oled(OLED_RST, OLED_DC, OLED_CS);
#elif defined (_VARIANT_IDO003_) | defined (_VARIANT_ID100HR_) | defined(_VARIANT_ID107HR_)
  #include "angry_cookie_64x32_xbm.h"
  #define OLED_WIDTH 64
  #define OLED_HEIGHT 32
  #include <SSD1306Spi.h>
  SSD1306Spi oled(OLED_RST, OLED_DC, OLED_CS);
#else
  #include "angry_cookie_128x64_xbm.h"
  #define OLED_WIDTH 128
  #define OLED_HEIGHT 64
  #include <SSD1306Wire.h>
  SSD1306Wire oled(0x3C);
#endif

uint32_t tButton;
#define debounceTime_ms 200

bool on_off_state = false;
bool wake_sleep_state = false;

void setup()
{
  Serial.begin(9600);
  Serial.println(__FILE__);

  #if defined(PIN_BUTTON1)
  #ifdef _VARIANT_WAVESHARE_BLE400_
  pinMode(PIN_BUTTON1, INPUT);
  #else
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  #endif
  pinMode(PIN_BUTTON2, INPUT_PULLUP);
  #endif
  
  oled.setScreenSize(OLED_WIDTH, OLED_HEIGHT);
  oled.init(); 
  oled.drawXbm(14, 0, angry_cookie::width, angry_cookie::height, angry_cookie::bits);
  oled.display();
  
  oled.flipScreenVertically();
  oled.setTextAlignment(TEXT_ALIGN_LEFT);
  oled.setFont(ArialMT_Plain_10);
  oled.drawString(0,  0, "github.com/");
  oled.drawString(0, 10, "micooke");
  oled.drawString(0, 20, __TIME__);
  oled.drawRect(0, 0, OLED_WIDTH, OLED_HEIGHT);
  
  oled.display();
  
  tButton = millis();
}

void loop()
{
  #if defined(PIN_BUTTON1)
  if (millis() - tButton > debounceTime_ms)
  {
    if (!digitalRead(PIN_BUTTON1))
    {
      if (wake_sleep_state)
      {
        Serial.println("sleep");
        oled.sleep();
      }
      else
      {
        Serial.println("wake");
        oled.wake();
      }
      wake_sleep_state = !wake_sleep_state;

      tButton = millis();
    }
    #ifdef _VARIANT_WAVESHARE_BLE400_
    if (!digitalRead(PIN_BUTTON2))
    #else
    if (digitalRead(PIN_BUTTON2))
    #endif
    {
      if (on_off_state)
      {
        Serial.println("off");
        oled.displayOff();
      }
      else
      {
        Serial.println("on");
        oled.displayOn();
      }
      on_off_state = !on_off_state;

      tButton = millis();
    }
  }
  #endif
  
  yield();
}
