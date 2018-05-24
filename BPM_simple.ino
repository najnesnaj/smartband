
#include <SPI.h>

#include <SSD1306Spi.h>
#include <SI114.h>



#define OLED_WIDTH 64
#define OLED_HEIGHT 32
SSD1306Spi oled(OLED_RST, OLED_DC, OLED_CS); // (pin_rst, pin_dc, pin_cs)

float xyz[3];
uint32_t tPage;
bool B1_isPressed = false;
uint8_t page_num = 0;
const uint8_t page_count = 2;

//heartrate sensor si114

int BPM;

long meting;
const int portForSI114 = 0; //for SI114 JJ
PortI2C myBus (portForSI114);
PulsePlug pulse (myBus);

void draw_page(uint8_t idx = 0);
String content = "";
void setup()
{

  Serial.begin(115200);
  Serial.println(__FILE__);

  pinMode(PIN_BUTTON1, INPUT_PULLUP);

  //JJ acc.init();

  oled.setScreenSize(OLED_WIDTH, OLED_HEIGHT);
  oled.init();
  oled.flipScreenVertically();
  oled.setTextAlignment(TEXT_ALIGN_LEFT);
  oled.setFont(ArialMT_Plain_10);
  draw_page(page_num++);
  delay(3000); // show splash for 3s
  tPage = millis();
  initPulseSensor();




}

void loop()
{
  int byte;





  if (!B1_isPressed & !digitalRead(PIN_BUTTON1)) // timer used for button debounce
  {
    page_num = (page_num + 1 < page_count) ? page_num + 1 : 0;
  }
  B1_isPressed = !digitalRead(PIN_BUTTON1);

  if (millis() - tPage > 20) // 20ms = 50Hz
  {
    tPage = millis();
    draw_page(page_num);
  }
  delay(10);


  readPulseSensorPSO2(); //heart rate sensor

  yield();
}


void draw_page(uint8_t idx)
{
  switch (idx)
  {
    case 1:
      page_accelerometer(); break;
    default:
      page_startup();
      break;
  }
}

void page_startup()
{
  String myString = String(BPM);

  oled.clear();
  if (pulse.isPresent()) {

    //  Serial.print("SI114x Pulse Sensor found on Port ");
    //    Serial.println(portForSI114);
    oled.drawString(0, 0, "FOUND");
  }

  else {

    //  Serial.print("No SI114x found on Port ");
    // Serial.println(portForSI114);
    oled.drawString(0, 0, "NOT FOUND");
  }



  oled.drawString(0, 0, content );

  //oled.drawString(30,0,myString ); //display heart beat

  oled.drawString(0, 10, __DATE__);


  oled.drawString(0, 20, __TIME__);
  oled.display();
}

void page_accelerometer()
{
  char fltBuf[5];
  String myString = String(BPM);
  //JJ  acc.getAccelXYZ(xyz);

  oled.clear();
  // float2chars(xyz[0], fltBuf);
  // oled.drawString(0, 0, "X:"); oled.drawString(10, 0, fltBuf);

  oled.drawString(0, 0, "BPM:" ); oled.drawString(30, 0, content ); //display heart beat
  /* float2chars(xyz[1], fltBuf);
    oled.drawString(0, 10, "Y:"); oled.drawString(10, 10, fltBuf);
    float2chars(xyz[2], fltBuf);
    oled.drawString(0, 20, "Z:"); oled.drawString(10, 20, fltBuf);*/

  oled.display();


}

void initPulseSensor() {



  pulse.setReg(PulsePlug::HW_KEY, 0x17);

  // pulse.setReg(PulsePlug::COMMAND, PulsePlug::RESET_Cmd);



  Serial.print("PART: ");

  Serial.print(pulse.getReg(PulsePlug::PART_ID));

  Serial.print(" REV: ");

  Serial.print(pulse.getReg(PulsePlug::REV_ID));

  Serial.print(" SEQ: ");

  Serial.println(pulse.getReg(PulsePlug::SEQ_ID));



  pulse.setReg(PulsePlug::INT_CFG, 0x03);       // turn on interrupts

  pulse.setReg(PulsePlug::IRQ_ENABLE, 0x04);    // turn on interrupt on PS1
  pulse.setReg(PulsePlug::IRQ_MODE1, 0x30);
  //  pulse.setReg(PulsePlug::IRQ_MODE2, 0x01);     // interrupt on ps3 measurement

  pulse.setReg(PulsePlug::MEAS_RATE, 0x84);     // see datasheet

  pulse.setReg(PulsePlug::ALS_RATE, 0x08);      // see datasheet

  pulse.setReg(PulsePlug::PS_RATE, 0x08);       // see datasheet



  // Current setting for LEDs pulsed while taking readings

  // PS_LED21  Setting for LEDs 1 & 2. LED 2 is high nibble

  // each LED has 16 possible (0-F in hex) possible settings

  // read the

  pulse.setReg(PulsePlug::PS_LED21, 0x11);      // LED current for 2 (IR1 - high nibble) & LEDs 1 (red - low nibble)

  //pulse.setReg(PulsePlug::PS_LED3, 0x02);       // LED current for LED 3 (IR2)



  Serial.print( "PS_LED21 = ");

  Serial.println(pulse.getReg(PulsePlug::PS_LED21), BIN);

  Serial.print("CHLIST = ");

  Serial.println(pulse.readParam(0x01), BIN);



  pulse.writeParam(PulsePlug::PARAM_CH_LIST, 0x77);         // all measurements on



  // increasing PARAM_PS_ADC_GAIN will increase the LED on time and ADC window

  // you will see increase in brightness of visible LED's, ADC output, & noise

  // datasheet warns not to go beyond 4 because chip or LEDs may be damaged

  pulse.writeParam(PulsePlug::PARAM_PS_ADC_GAIN, 0x00);





  // You can select which LEDs are energized for each reading.

  // The settings below turn on only the LED that "normally" would be read

  // ie LED1 is pulsed and read first, then LED2 is pulsed and read etc.

  pulse.writeParam(PulsePlug::PARAM_PSLED12_SELECT, 0x21);  // 21 = LED 2 & LED 1 (red) resp.

  // pulse.writeParam(PulsePlug::PARAM_PSLED3_SELECT, 0x04);   // 4 = LED 3 only



  // Sensors for reading the three LEDs

  // 0x03: Large IR Photodiode

  // 0x02: Visible Photodiode - cannot be read with LEDs on - just for ambient measurement

  // 0x00: Small IR Photodiode

  pulse.writeParam(PulsePlug::PARAM_PS1_ADCMUX, 0x03);      // PS1 photodiode select
  //  pulse.writeParam(PulsePlug::PARAM_PS2_ADCMUX, 0x03);      // PS2 photodiode select

  //  pulse.writeParam(PulsePlug::PARAM_PS3_ADCMUX, 0x03);      // PS3 photodiode select



  pulse.writeParam(PulsePlug::PARAM_PS_ADC_COUNTER, B01110000);    // B01110000 is default

  // pulse.setReg(PulsePlug::COMMAND, PulsePlug::PSALS_AUTO_Cmd);     // starts an autonomous read loop
  pulse.setReg(PulsePlug::COMMAND, PulsePlug::PS_AUTO_cmd);
  Serial.println(pulse.getReg(PulsePlug::CHIP_STAT), HEX);

  Serial.print("end init");

}

void readPulseSensorPSO2() {


  //initPulseSensor();

  //   Serial.print((long)pulse.als_ir);        //  ambient IR

  //pulse.getReg(PulsePlug::CHIP_STAT);
  // Serial.println("LEDDATA");
  pulse.fetchLedData();
  // pulse.fetchData();
  //  pulse.getReg(PulsePlug::IRQ_STATUS);  //zonder oproep van status -- geen metingen
  //   Serial.println(pulse.getReg(PulsePlug::PS1_DATA0), HEX);
  //   Serial.println(pulse.getReg(PulsePlug::PS1_DATA1), HEX);

  // Serial.println(pulse.getReg(PulsePlug::PS_LED21), BIN);
  meting = (long)pulse.ps1;
  if ((meting > 1000000) && (meting < 200000000))
    Serial.println(meting);
  //  Serial.println((long)pulse.ps2);
  //   Serial.println((long)pulse.ps3);
  //Serial.println((long)pulse.als_vis);
  //  Serial.println((long)pulse.ps2);
  //    Serial.println((long)pulse.ps3);   //niet gevoelig voor LED licht
  // Serial.println((long)pulse.als_ir);       //  ambient visible -- geeft nooit resultaat

  //  Serial.println(pulse.getReg(PulsePlug::CHIP_STAT), HEX);
  // Serial.println("LEDDATA");
  //  pulse.fetchData();
  // Serial.println(pulse.resp);
  // Serial.println(pulse.als_vis); //  ambient visible -- geeft nooit resultaat

  //  Serial.println(pulse.als_ir);       //  ambient visible
  // Serial.println(pulse.ps1);
  // Serial.println(pulse.getReg(PulsePlug::CHIP_STAT), HEX);

  //  Serial.println(pulse.getReg(PulsePlug::HW_KEY), HEX);
  //  Serial.println(pulse.getReg(PulsePlug::MEAS_RATE), HEX);
  //  Serial.println(pulse.getReg(PulsePlug::ALS_VIS_DATA1), HEX);

}





