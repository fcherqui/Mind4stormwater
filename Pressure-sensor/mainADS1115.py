import pycom
from machine import I2C, Pin
from ADS1115 import ADS1115

# Setup ADS1115 using defaults (0x48 address and 4.096V max range)
i2c = I2C(0, I2C.MASTER, pins=(config.sdaPin, config.sclPin)) #sda=P23, scl=P22
adc = ADS1115(i2c)

while (True):
    print("\nChannel 0 voltage: {}V".format(adc.get_voltage(0)))
    print("Channel 0 ADC value: {}\n".format(adc.read(0)))
    for i in range(0, config.sleeptime*60):
        utime.sleep(1) #waiting 60 seconds i times ==> waiting i*60 between measures if sleep is not needed...
