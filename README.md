




# smartband
ID107 heartrate nordic nrf51822 smartwatch arduino

This work piggy backs on previous work by many people, mainly : @goran-mahovlic, @rogerclarkmelbourne, @curtpw, @Gordon, @micooke

Note : I have no affiliation with idoosmart / Shenzhen DO Intelligent Technology Co,. Ltd, nor any aliexpress or smartwatch retailer
 

ID107HR

recently I bought this watch for 10 euro (!!) on gearbest
This watch contains a nrf51822 nordic microcontroller, which support BLE (bluetooth), an oled display, a heart rate sensor from Silicon Labs SI1142, an Accelerometer (Kx022), a battery ... 

I use an "ST LINK V2" connector. ( Connect swdio swclk and ground (3 wires) to the watch) (need battery charged)

an arduino programming environment Arduino ide 1.8.5, which needs some extras ....

Under File/Preferences

in field : 
Additional Boards Manager URLs
-> https://sandeepmistry.github.io/arduino-nRF5/package_nRF5_boards_index.json
-> https://micooke.github.io/package_nRF5_smartwatches_index.json


Open the Boards Manager from the Tools -> Board menu and install "Nordic Semiconductor nRF5 Smartwatches"
Select your nRF5 smartwatch from the Tools -> Board menu (ID107HR)


In order to be able to use bluetooth, you need to flash a softdevice to the nrf51822.
The procedure is explained on https://github.com/sandeepmistry/arduino-nRF5 
(Flashing a SoftDevice)


Using the Arduino IDE Library Manager
Choose Sketch -> Include Library -> Manage Libraries...
Type BLEPeripheral into the search box.
Click the row to select the library.
Click the Install button to install the library.


NOTE :

by flashing the Arduino script, you overwrite the firmware (the watch as you knew it, is no more ...)

This is work in progress, and I'm a lousy programmer ....

I cut a few corners (in the si114 library), by commenting out stuff in the libraries (eg //#include <avr/sleep.h> #include <util/atomic.h>)

My intention is to monitor heart rate.
For now I use the "Nordic nrf Connect App" to read out data from the watch, but I guess soldering the serial port is an easier option ...

There exist different versions of this watch, mine is 2.4 (e.g. version 2.3 has a different pinout for si1142 ( SDA pin 26, SCL pin 28) -- this means you might have to change the si114 library hardcoded pins


For reference, @sanpii has the same board as mine ID107 ver2.4 in his repo https://github.com/sanpii/open-id107

 sda, scl = 14,16 for kx022 and = 22,23 for Si1142. Kx022 scans as 0x1F (7b address) 
(modified SI114.h hardcoded ;)) 


I found a lot of usefull information on https://gitter.im/nRF51822-Arduino-Mbed-smart-watch/Lobby
 
You can have look at the inside of the watch on  https://fccid.io/2AHFTID107/Internal-Photos/Internal-Photos-3028344





I used the PSO2 demo, but the output was erratic 

https://moderndevice.com/news/pulse-sensor-i/ 


I modified some parameters (see : BPM_simple.ino).
I used the biggest IR sensor and only the ps1-mux.

I get to see the heartbeat (heartbeat.jpg)

---------------------------------------------------------------------

I use a raspberry pi to program the watch -- see openocd for howto

---------------------------------------------------------------------

it is possible to program via CLI on the raspberry pi, using platformio
-> see directory platformio on how to setup 
-> directory platformio-id107 for an example

---------------------------------------------------------------------

there is a way to use DFU (device firmware update) -- programming the watch over the air, while it is still on your wrist -- see https://github.com/najnesnaj/ota-dfu-smartband

----------------------------------------------------------------------

the libraries necessary to the included in the example, are copied under this repository

------------------------------------------------------------------------



The watch sends the values on the serial bluetooth; (script in KX022_getAcc) 
see blue-py directory for howto readout data on raspberry pi

---------------------------------------------------------------------









