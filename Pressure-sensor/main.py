##################################################
####  MIND4STORMWATER read voltage from adc   ####
####         version 3.04 - May 2020          ####
##################################################
##  Project website: http://mind4stormwater.org ##
##  feel free to use, reuse, modify, share!!    ##
##  University of Melbourne / INSA Lyon         ##
##  Frédéric Cherqui - fcherqui@gmail.com       ##
##  & Rob James                                 ##
##################################################
import utime
import config
beginning=utime.ticks_ms()
if config.powersaving==False:
    print("#=========================")
    print("# MAIN PROGRAM STARTED ")
    print("#=========================")
    print("%.3f sec.: pycom lib" % ((utime.ticks_ms()-beginning)/1000))
import pycom
pycom.rgbled(config.OFF)
if config.powersaving==False:
    print("%.3f sec.: socket lib" % ((utime.ticks_ms()-beginning)/1000))
import socket
if config.powersaving==False:
    print("%.3f sec.: machine Pin lib" % ((utime.ticks_ms()-beginning)/1000))
from machine import Pin
if config.powersaving==False:
    print("%.3f sec.: ADS1115/ADS1115 lib" % ((utime.ticks_ms()-beginning)/1000))
from ADS1115 import ADS1115
if config.powersaving==False:
    print("%.3f sec.: machine I2C lib & I2C" % ((utime.ticks_ms()-beginning)/1000))
from machine import I2C
i2c = I2C(0, I2C.MASTER, pins=(config.sdaPin, config.sclPin)) #sda=P23, scl=P22
adc = ADS1115(i2c)
if config.powersaving==False:
    print("%.3f sec.: scan I2C for components" % ((utime.ticks_ms()-beginning)/1000))
i2c_components=i2c.scan()
#print(i2c_components)
if config.powersaving==False:
    print("%.3f sec.: all libs imported" % ((utime.ticks_ms()-beginning)/1000))

#######################
### INIT EVERYTHING ###
#######################
if config.powersaving==False:
    print("%.3f sec.: starts (LED WHITE)" % ((utime.ticks_ms()-beginning)/1000))
    pycom.rgbled(config.WHITE)

#CONFIG COMMUNICATION
networkname="none"
if config.powersaving==False:
    print("%.3f sec.: Establish communication (LED OFF)" % ((utime.ticks_ms()-beginning)/1000))
if config.sigfoxok==True:
    from network import Sigfox
    sigfox = Sigfox(mode=Sigfox.SIGFOX, rcz=Sigfox.RCZ1) # init Sigfox for: RCZ1 (Europe), RCZ4 (Australia)
    s = socket.socket(socket.AF_SIGFOX, socket.SOCK_RAW) # create a Sigfox socket
    s.setblocking(True) # make the socket blocking
    s.settimeout(30) #timeout 10 seconds
    s.setsockopt(socket.SOL_SIGFOX, socket.SO_RX, False) # configure it as uplink only
    networkname="Sigfox"
if config.loraok==True:
    from Lora import _LoraEU868
    from Lora import _LoraSpecial
    if config.TTNregion=="EU868":
        s = _LoraEU868.setupLora()
        networkname="LoRa EU868"
    if config.TTNregion=="Special":
        s = _LoraSpecial.setupLora()
        networkname="LoRa Special"
if config.powersaving==False: #and result.is_valid()
        print("%.3f sec.: Communication established with %s (BLINK WHITE)" % ((utime.ticks_ms()-beginning)/1000, networkname))
        pycom.rgbled(config.WHITE)
        utime.sleep(0.5)
        pycom.rgbled(config.OFF)
        utime.sleep(0.2)
        pycom.rgbled(config.WHITE)
        utime.sleep(0.5)
        pycom.rgbled(config.OFF)

################################
### LOOP IF NO NEED TO SLEEP ###
################################
while (True):
    if config.powersaving==False:
        print("%.3f sec.: ===============================================================" % ((utime.ticks_ms()-beginning)/1000))
    #=====================
    # LOAD time of last measure
    #=====================
    f = open("wtime")
    wtime=int(f.read()) #read the forecast wakeup time
    mtime=utime.ticks_ms() #time of the start of the measure
    btime=max(0,(mtime-wtime)) #calculate the boot time
    #print("btime=mtime - wtime ==> %i = %i - %i " % (btime/1000, mtime /1000, wtime/1000))
    f.close()

    #=====================
    # LOAD MEASURE counter
    #=====================
    f = open("counter")
    num=f.read()
    f.close()
    f=open("counter","w")
    if num=="":
        num=0
    newcounter=int(num)+1
    if newcounter>65000:
        newcounter=0
    f.write(str(newcounter))
    f.close()
    if config.powersaving==False: #and result.is_valid()
        print("%.3f sec.: ==> Measure %i at %i minute(s) %i second(s)" % ((utime.ticks_ms()-beginning)/1000, newcounter, (mtime/1000/60), (mtime/1000)%60))

    #========================================
    # MESURE VOLTAGE(S) FROM THE PRESSURE SENSOR
    #========================================
    nprobe=config.nsensors
    code=nprobe
    ADS1115_ADDR = 72 #I2C address of any ADS1115
    if ADS1115_ADDR in i2c_components:
        if config.powersaving==False:
            pycom.rgbled(config.YELLOW)
            print("%.3f sec.: Measure voltage (LED YELLOW)" % ((utime.ticks_ms()-beginning)/1000))
        volt1 = max(0,adc.get_voltage(0)*1000) # sonde 1 PROFESSIONAL
        battery = max(0,adc.get_voltage(1)*1000*2) # Battery
        volt2 = max(0,adc.get_voltage(2)*1000) # sonde 2 (low-cost)
        if config.powersaving==False:
            if nprobe==1: print("     volt1 = %i mV & battery = %i" % (volt1,battery))
            if nprobe==2: print("     volt1 = %i mV | volt2=%i mV & battery = %i" % (volt1, volt2, battery))
            if nprobe==3: print("     volt1 = %i mV | volt2=%i mV | volt3=%i mV & battery = %i" % (volt1, volt2,volt3,battery))
    else:
        code=code+40
        volt1=0
        volt2=0
        volt3=0
        battery=0
        print("%.3f sec.: !!!! ADS1115 missing " % ((utime.ticks_ms()-beginning)/1000))

    #=====================
    # Reading the time from the RTC
    #=====================
    if config.withRTC:
        DS3231_I2C_ADDR = 104 #I2C address of any DS3231 real-time clock
        if DS3231_I2C_ADDR in i2c_components:
            if config.powersaving==False:
                pycom.rgbled(config.ORANGE)
                print("%.3f sec.: Reading RTC time" % ((utime.ticks_ms()-beginning)/1000))
            if config.powersaving==False: print("%.3f sec.: readtime / _RTCsync lib" % ((utime.ticks_ms()-beginning)/1000))
            from readtime import _RTCsync
            if config.powersaving==False: print("%.3f sec.: machine RTC lib" % ((utime.ticks_ms()-beginning)/1000))
            from machine import RTC
            epoch=_RTCsync.synctime() #sync time and send the time of the first measure (epoch difference from 01/01/2020)
            DT=utime.localtime()
            datetime = ("%02d/%02d/%04d %02d:%02d:%02d" % (DT[2],DT[1],DT[0],DT[3],DT[4],DT[5]))
            if config.powersaving==False: print("%.3f sec.: time is %i days %i hours %i minutes %i seconds)" % ((utime.ticks_ms()-beginning)/1000, epoch, epoch//(3600*24),(epoch//3600)%24, (epoch//60)%60, epoch%60))
        else:
            code=code+10
            print("%.3f sec.: !!!! RTC clock missing " % ((utime.ticks_ms()-beginning)/1000))

    #=====================
    # SAVE data on the SD card (if present)
    #=====================
    if (config.sdmandatory and not sd_mounted):
        code=code+20
        print("%.3f sec.: !!!! SD reader missing " % ((utime.ticks_ms()-beginning)/1000))
    if config.sdmandatory and sd_mounted:
        if config.powersaving==False:
            pycom.rgbled(config.RED)
            print("%.3f sec.: Saving on SD (LED PURPLE)" % ((utime.ticks_ms()-beginning)/1000))
            print("%.3f sec.: deinit / init SD" % ((utime.ticks_ms()-beginning)/1000))
        sd.deinit()
        sd.init()
        if config.powersaving==False: print("%.3f sec.: writing on SD card" % ((utime.ticks_ms()-beginning)/1000))
        f=open('/sd/data.csv',"a")
        if config.withRTC: f.write("%s, %s," % (datetime,epoch))
        f.write("%i,%i,%i," % (newcounter,nprobe,volt1))
        if config.nsensors==2: f.write("%i," % (volt2))
        if config.nsensors==3: f.write("%i," % (volt3))
        f.write("%.2f\n" % (battery))
        f.close()
        i2c = I2C(0, I2C.MASTER, baudrate=100000, pins=(config.sdaPin, config.sclPin)) #sda=P23, scl=P22
        if config.powersaving==False: print("%.3f sec.: copied on SD-Card, I2C restarted" % ((utime.ticks_ms()-beginning)/1000))

    #==================
    # BYTIZING THE DATA
    #==================
    if config.powersaving==False:
        pycom.rgbled(config.PURPLE)
        print("%.3f sec.: bytizing the data (LED PURPLE)" % ((utime.ticks_ms()-beginning)/1000))
    newcounter_1 = int(newcounter//256)
    newcounter_2 = int(newcounter%256)
    volt1_1 = int(volt1//256)
    volt1_2 = int(volt1%256)
    batt_1 = int(battery//256)
    batt_2 = int(battery%256)
    if nprobe>=2:
        volt2_1 = int(volt2//256)
        volt2_2 = int(volt2%256)
    if nprobe==3:
        volt3_1 = int(volt3//256)
        volt3_2 = int(volt3%256)

    ## TEST SIGFOX MESSAGE SENT
    #newcounter_1=0
    #newcounter_2=1
    #code=2
    #volt1_1=0
    #volt1_2=10
    #volt2_1=0
    #volt2_2=20
    #volt3_1=0
    #volt3_2=30
    #batt_1=0
    #batt_2=99

    #=================
    # SENDING THE DATA
    #=================
    if config.powersaving==False:
        pycom.rgbled(config.GREEN)
        print("%.3f sec.: sending the data (LED GREEN)" % ((utime.ticks_ms()-beginning)/1000))
    if config.powersaving==False:
        if nprobe==1: print("     Sending message:",newcounter_1, newcounter_2, code, volt1_1, volt1_2, batt_1, batt_2)
        if nprobe==2: print("     Sending message:",newcounter_1, newcounter_2, code, volt1_1, volt1_2, volt2_1, volt2_2, batt_1, batt_2)
        if nprobe==3: print("     Sending message:",newcounter_1, newcounter_2, code, volt1_1, volt1_2, volt2_1, volt2_2, volt3_1, volt3_2, batt_1, batt_2)
    if (config.loraok or config.sigfoxok):
        if nprobe==1: s.send(bytes([newcounter_1, newcounter_2, code, volt1_1, volt1_2, batt_1, batt_2]))
        if nprobe==2: s.send(bytes([newcounter_1, newcounter_2, code, volt1_1, volt1_2, volt2_1, volt2_2, batt_1, batt_2]))
        if nprobe==3: s.send(bytes([newcounter_1, newcounter_2, code, volt1_1, volt1_2, volt2_1, volt2_2, volt3_1, volt3_2, batt_1, batt_2]))
        utime.sleep(0.1)
        if config.powersaving==False:
            print("     Message sent through ==>",networkname)

    #==================
    # GOTO SLEEP OR NOT
    #==================
    pycom.rgbled(config.BLUE)
    if config.powersaving==False:
        print("%.3f sec.: goto sleep or wait (LED BLUE)" % ((utime.ticks_ms()-beginning)/1000))
    #calculate the sleep duration (all in milliseconds)
    stime=utime.ticks_ms() #timestamp of beginning of sleep
    sleepduration_ms=max(10000,int(mtime+config.sleeptime*60*1000-btime-stime)) #mini sleep 10 seconds
    sleepduration_s=sleepduration_ms/1000 #[seconds] sleep duration
    wtime=int(stime+sleepduration_ms) #forecast wake up time
    #print("sdur = mtime + sleeptime - btime - stime ==> %i = %i + %i - %i - %i " % (sleepduration_s, mtime/1000, config.sleeptime*60, btime/1000, stime/1000))

    #SAVE the next wake up time
    f=open("wtime","w")
    f.write(str(wtime))
    f.close()

    #SLEEP now (or wait)
    if config.powersaving==False:
        print("%.3f sec.: Define pins" % ((utime.ticks_ms()-beginning)/1000))
    donepin = Pin(config.relay,mode=Pin.OUT, pull=Pin.PULL_DOWN)
    donepin(0)

    if config.needsleep == True:
        if config.powersaving==False:
            print("%.3f sec.: going to sleep now (nano timer relay)" % ((utime.ticks_ms()-beginning)/1000))
        while True:
            donepin(0)
            utime.sleep(0.1)
            donepin(1)
            utime.sleep(0.1)
    if config.powersaving==False:
        print("%.3f sec.: waiting %i minute(s) %i second(s) before a new measure" % ((utime.ticks_ms()-beginning)/1000, sleepduration_s//60, sleepduration_s%60))
    for i in range(0, sleepduration_s):
        utime.sleep(1) #waiting 60 seconds i times ==> waiting i*60 between measures if sleep is not needed...
