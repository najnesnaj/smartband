from bluepy.btle import *


# connect to device
per = Peripheral('DA:E2:C9:AB:2F:65', "random")
#per = Peripheral('F5:87:4A:AD:6B:DD', "random")

try:

    
    # read test string on uart 00003
    for x in range(0,1000):
        c = per.getCharacteristics(uuid='6e400003b5a3f393e0a9e50e24dcca9e')[0]
        print c.read()

finally:
    per.disconnect()

