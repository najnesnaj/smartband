
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








