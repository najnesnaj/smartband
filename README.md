


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
I cut a few corners, by commenting out stuff in the libraries (eg //#include <avr/sleep.h> #include <util/atomic.h>)

My intention is to monitor heart rate.
For now I use the "Nordic nrf Connect App" to read out data from the watch, but I guess soldering the serial port is an easier option ...

There exist different versions of this watch, mine is 2.4 (e.g. version 2.3 has a different pinout for si1142 ( SDA pin 26, SCL pin 28)


For reference, @sanpii has the same board as mine ID107 ver2.4 in his repo https://github.com/sanpii/open-id107

 sda, scl = 14,16 for kx022 and = 22,23 for Si1142. Kx022 scans as 0x1F (7b address) 
(modified SI114.h hardcoded ;)) 


I found a lot of usefull information on https://gitter.im/nRF51822-Arduino-Mbed-smart-watch/Lobby
 
You can have look at the inside of the watch on  https://fccid.io/2AHFTID107/Internal-Photos/Internal-Photos-3028344







Progress :

I soldered a serial port 

And this data I get without covering sensor
90951590	253	0	90951843
1	 BPM 142	 signal size 2996
97059551	252	0	97059803
0
92773492	254	0	92773746
1	 BPM 576	 signal size 2996



and this with my finger in front of sensor

1	 BPM 163	 signal size 1890
121310060	262	0	121310322
0
121178996	259	0	121179255
120078066	254	0	120078320
121821309	261	0	121821570







