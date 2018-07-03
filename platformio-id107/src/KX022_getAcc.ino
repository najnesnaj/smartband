// this scripts prints the readings of PS1 to serial and (!) bluetoothserial




//#include <Wire.h>
#include <SPI.h>
//#include <KX022.h>
#include <SSD1306Spi.h>
#include <SI114.h>
#include "variant.h"
//#define OLED_RST        (30)
//#define OLED_CS         (29)
//#define OLED_DC         (0)
#define PIN_BUTTON1             (4)



#include <BLEPeripheral.h>
//#include "BLESerial.h"
//setting up BLE
// define pins (varies per shield/board)
#define BLE_REQ   10
#define BLE_RDY   2
#define BLE_RST   9
//#define GET_PULSE_READING
// #define PRINT_LED_VALS
// create peripheral instance, see pinouts above
BLEPeripheral                    blePeripheral       = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);
//BLESerial bleSerial(10, 2, 9);
// create service
BLEService                       testService         = BLEService("fff0");
// create counter characteristic
BLEUnsignedShortCharacteristic   testCharacteristic  = BLEUnsignedShortCharacteristic("fff1", BLERead | BLEWrite | BLEWriteWithoutResponse | BLENotify /*| BLEIndicate*/);
// create user description descriptor for characteristic
BLEDescriptor                    testDescriptor      = BLEDescriptor("2901", "counter");

// last counter update time
unsigned long long               lastSent            = 0;



//#define USE_SOFTWAREI2C
//#ifdef USE_SOFTWAREI2C
//#include <SoftwareI2C.h>
//SoftwareI2C sWire(14, 16);
//KX022<SoftwareI2C> acc(sWire);
//#else
//KX022<> acc(Wire);
//KX022<TwoWire>
//KX022 acc(acc); // TwoWire is the default class, so this is the same as above
//#endif

#define OLED_WIDTH 64
#define OLED_HEIGHT 32
SSD1306Spi oled(OLED_RST, OLED_DC, OLED_CS); // (pin_rst, pin_dc, pin_cs)

float xyz[3];
uint32_t tPage;
bool B1_isPressed = false;
uint8_t page_num = 0;
const uint8_t page_count = 2;

//heartrate sensor si114

const int SAMPLES_TO_AVERAGE = 5;
int binOut;     // 1 or 0 depending on state of heartbeat

int BPM;
long meting;
unsigned long red;        // read value from visible red LED

unsigned long IR1;        // read value from infrared LED1

unsigned long IR2J;       // read value from infrared LED2

unsigned long total;     // all three LED reads added together

int signalSize;          // the heartbeat signal minus the offset


const int portForSI114 = 0; //for SI114 JJ
PortI2C myBus (portForSI114);
PulsePlug pulse (myBus);

void draw_page(uint8_t idx = 0);
String content = "";
void setup()
{

  //bleSerial.setLocalName("UART");
  //bleSerial.begin();
  // Wire.begin();

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

  //bleSerial.poll();

  //   Serial.print(" LOOP ");

/*  if (!B1_isPressed & !digitalRead(PIN_BUTTON1)) // timer used for button debounce
  {
    page_num = (page_num + 1 < page_count) ? page_num + 1 : 0;
    sd_power_gpregret_set(0xB1); //reset the nrf51822
    NVIC_SystemReset();
  }
  B1_isPressed = !digitalRead(PIN_BUTTON1);
*/
  if (millis() - tPage > 20) // 20ms = 50Hz
  {
    tPage = millis();
    draw_page(page_num);
  }
  delay(10);

  //  while ((byte = bleSerial.read()) > 0)
  // {

  // oled.clear();
  //    oled.drawString(0, 20, "prijs");
  //    oled.display();
  //  }
  readPulseSensorPSO2(); //heart rate sensor
  //  bleSerial.write("smartass");
  //  while ((byte = bleSerial.read()) > 0) {
  //    content.concat(byte);
  //  }
  //  if (content != "") {
  //  Serial.println(content);
  //    oled.clear();
  //    oled.drawString(0, 20, content);
  //    oled.display();
  //  }
  yield();
}

void float2chars(float &in, char (&out)[5])
{
  bool sign_bit = (in < 0);
  uint16_t tmp = sign_bit ? (-in * 10) : (in * 10);
  out[0] = (sign_bit) ? '-' : ' ';
  out[1] = char('0' + (tmp / 10));
  out[2] = '.';
  out[3] = char('0' + (tmp % 10));
  out[4] = '\0';
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
  /*  if (pulse.isPresent()) {

      //  Serial.print("SI114x Pulse Sensor found on Port ");
      //    Serial.println(portForSI114);
      oled.drawString(0, 0, "FOUND");
    }

    else {

      //  Serial.print("No SI114x found on Port ");
      // Serial.println(portForSI114);
      oled.drawString(0, 0, "NOT FOUND");
    }

  */


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

  // pulse.writeParam(PulsePlug::PARAM_PS2_ADCMUX, 0x03);      // PS2 photodiode select

  // pulse.writeParam(PulsePlug::PARAM_PS3_ADCMUX, 0x03);      // PS3 photodiode select



  pulse.writeParam(PulsePlug::PARAM_PS_ADC_COUNTER, B01110000);    // B01110000 is default

  // pulse.setReg(PulsePlug::COMMAND, PulsePlug::PSALS_AUTO_Cmd);     // starts an autonomous read loop
  pulse.setReg(PulsePlug::COMMAND, PulsePlug::PS_AUTO_cmd);
  Serial.println(pulse.getReg(PulsePlug::CHIP_STAT), HEX);

  Serial.print("end init");

}

void readPulseSensorPSO2() {




  //   Serial.print((long)pulse.als_ir);        //  ambient IR

  // Serial.println(pulse.getReg(PulsePlug::CHIP_STAT), HEX);
  //Serial.println("LEDDATA");
  pulse.fetchLedData();
  // pulse.fetchData();
  // pulse.getReg(PulsePlug::IRQ_STATUS);  //zonder oproep van status -- geen metingen
  //   Serial.println(pulse.getReg(PulsePlug::PS1_DATA0), HEX);
  //   Serial.println(pulse.getReg(PulsePlug::PS1_DATA1), HEX);

  // Serial.println(pulse.getReg(PulsePlug::PS_LED21), BIN);


  meting = (long)pulse.ps1;
  if ((meting > 1000000) && (meting < 200000000))
    Serial.println(meting);
 // if (bleSerial) {
  //  if ((meting > 1000000) && (meting < 200000000))
   //   bleSerial.println(meting);
   // bleSerial.println(" tick-tacks!");
//     delay(10);
 // }

  //  Serial.println((long)pulse.ps1);
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





  void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());
  }

  void blePeripheralDisconnectHandler(BLECentral& central) {
  // central disconnected event handler
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());
  }
  void characteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // characteristic value written event handler
  Serial.print(F("Characteristic event, writen: "));
  Serial.println(testCharacteristic.value(), DEC);
  }

  void characteristicSubscribed(BLECentral& central, BLECharacteristic& characteristic) {
  // characteristic subscribed event handler
  Serial.println(F("Characteristic event, subscribed"));
  }

  void characteristicUnsubscribed(BLECentral& central, BLECharacteristic& characteristic) {
  // characteristic unsubscribed event handler
  Serial.println(F("Characteristic event, unsubscribed"));
  }

int smooth(float data, float filterVal, float smoothedVal) {



  if (filterVal > 1) {      // check to make sure param's are within range

    filterVal = .99;

  }

  else if (filterVal <= 0) {

    filterVal = 0;

  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;

}

