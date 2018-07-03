
install platformio https://github.com/platformio/platform-nordicnrf51

platformio platform install platform-nordicnrf51

replace original platformio core with sandeep mistry's core

https://github.com/sandeepmistry/arduino-nRF5/tree/master/cores/nRF5

~/.platformio/packages/framework-arduinonordicnrf5/cores/nRF5

----> here you have to replace itoa.c and itoa.h by the ones in this repository

create a directory "board" and place  id107.json under this directory

~/.platformio/boards

id107.json contains compilerflags which refer to softdevice S130 : -DS130 -DNRF51_S130

if not - BLEperipheral will not work
===================================================================

under directory platformio-id107 is an example ini-file
