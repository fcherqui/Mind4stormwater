#########################################
#### WETLANDIA water level monitoring ###
####     version 3.1 - dec. 2019      ###
#########################################
## Mind4Stormwater project: http://mind4stormwater.org,
## feel free to use, reuse, modify, share!!
## University of Melbourne / INSA Lyon
## Frédéric Cherqui - fcherqui@gmail.com
#########################################

import config
import utime
import time
import pycom
from Lora import _LoraAU915
from Lora import _LoraAS923
from network import Sigfox
from USblack import measureUSblack
import socket
import binascii #module that makes conversion between binary and Ascii
import ustruct
import machine
from machine import Pin
from machine import ADC
from dth import DTH
# from machine  import WDT
#
# wdt = WDT(timeout=65000)  #START WATCHDOG
# print("watchdog ON")
# wdt.feed() #RESET WATCHDOG
# print("watchdog +")

#INIT EVERYTHING
pycom.rgbled(config.OFF)
donepin = Pin(config.relay,mode=Pin.OUT)
donepin(0)
adc = ADC()

#CONFIG COMMUNICATION
if config.powersaving==False:
    pycom.rgbled(config.WHITE)
    utime.sleep(1)
if config.sigfoxok==True:
    sigfox = Sigfox(mode=Sigfox.SIGFOX, rcz=Sigfox.RCZ4) # init Sigfox for: RCZ1 (Europe), RCZ4 (Australia)
    s = socket.socket(socket.AF_SIGFOX, socket.SOCK_RAW) # create a Sigfox socket
    s.setblocking(True) # make the socket blocking
    s.settimeout(30) #timeout 10 seconds
    s.setsockopt(socket.SOL_SIGFOX, socket.SO_RX, False) # configure it as uplink only
if config.loraok==True and config.TTNregion=="AU915":
    s = _LoraAU915.setupLora()
if config.loraok==True and config.TTNregion=="AS923":
    s = _LoraAS923.setupLora()

################################
### LOOP IF NO NEED TO SLEEP ###
################################
while (True):
    #START PROGR
    #LOAD MEASURE counter
    f = open("counter")
    num=f.read()
    f.close()
    f=open("counter","w")
    newcounter=int(num)+1
    if newcounter>65000:
        newcounter=0
    f.write(str(newcounter))
    f.close()
    if config.powersaving==False: #and result.is_valid()
        print("Measure # ", newcounter)
        pycom.rgbled(config.RED)
        utime.sleep(0.5)
        pycom.rgbled(config.OFF)
        utime.sleep(0.2)
        pycom.rgbled(config.RED)
        utime.sleep(0.5)
        pycom.rgbled(config.OFF)
    us1med,us1mean,us1dev = measureUSblack.advanced_measuretime(config.trig,config.echo)
    if config.powersaving==False:
        print("Ultrasonic time = "+ str(us1med)+ " us")
        print("Ultrasonic distance = "+ str(us1med*0.34)+ " mm")
    if config.isDHT11==True:
        th = DTH(config.DHT,0) #for DHT11 sensor (blue)
        nameDTH="11"
    else:
        th = DTH(config.DHT,1) #for DHT22 sensor (white)
        nameDTH="22"
    # time.sleep(2)
    result = th.read()
    if config.powersaving==False: #and result.is_valid()
        print("Temperature DHT"+nameDTH+" = %d C" % result.temperature)
        print("Humidity DHT"+nameDTH+"  = %d %%" % result.humidity)
    meanbatt=0
    j=0
    if config.powersaving==False:
        pycom.rgbled(config.BLUE)
    while j < 5:
        utime.sleep(0.2)
        batt = adc.channel(attn=3, pin=config.voltmeter)
        meanbatt += batt.voltage()/1000
        j += 1
    batt_volt = (meanbatt/j)*config.batcoeff #multiplcator coef to adjust the real value of the battery voltage
    if config.powersaving==False:
        print("Battery Voltage = %.3f V" % batt_volt)
    if config.powersaving==False:
        pycom.rgbled(config.PURPLE)
        utime.sleep(0.2)
        pycom.rgbled(config.OFF)
        utime.sleep(0.1)
        pycom.rgbled(config.PURPLE)
        utime.sleep(0.2)
        pycom.rgbled(config.OFF)
        print("Sending message...")
    #######################################
    ####### FAKE VALUES TO TEST ###########
    #us1med=750
    #result.temperature=12.2
    #result.humidity=23.56
    #######################################
    newcounter_1 = int(newcounter//256)
    newcounter_2 = int(newcounter%256)
    us1_1 = int(us1med//256)
    us1_2 = int(us1med%256)
    #minimun number of bytes (for Sigfox google file)
    batt12 = int(batt_volt*25)
    temp12 = int(result.temperature*5)
    humi12 = int(result.humidity*2)
    s.send(bytes([newcounter_1, newcounter_2, us1_1,us1_2,batt12,temp12,humi12]))
    utime.sleep(0.1)
    if config.powersaving==False:
        print("Message:",newcounter_1, newcounter_2, us1_1,us1_2,batt_volt,result.temperature,result.humidity)
        print("Message sent through SIGFOX:",bool(config.sigfoxok))
        print("Message sent through LoRa - ",config.TTNregion," :",bool(config.loraok))
        print("------------------------------------")
        pycom.rgbled(config.GREEN)
        utime.sleep(0.2)
        pycom.rgbled(config.OFF)
        utime.sleep(0.1)
        pycom.rgbled(config.GREEN)
        utime.sleep(0.2)
        pycom.rgbled(config.OFF)
        utime.sleep(0.1)
    if config.needsleep == True:
        if config.powersaving==False:
            print("going to sleep now")
        pycom.rgbled(config.BLUE)
        while True:
            donepin(0)
            utime.sleep(0.1)
            donepin(1)
            utime.sleep(0.1)
    # print("waiting 10 seconds before a new measure")
    # utime.sleep(10) #waiting 5 minutes between measures if sleep is not needed...
