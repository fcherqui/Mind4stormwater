# boot.py -- run on boot-up
import utime, config
beginning=utime.ticks_ms()
if config.powersaving==False:
    print("#=========================")
    print("# BOOT STARTED ")
    print("#=========================")
    print("%.3f sec.: network/Bluetooth" % ((utime.ticks_ms()-beginning)/1000))
from network import Bluetooth
if config.powersaving==False:
    print("%.3f sec.: network/WLAN" % ((utime.ticks_ms()-beginning)/1000))
from network import WLAN
if config.powersaving==False:
    print("%.3f sec.: pycom" % ((utime.ticks_ms()-beginning)/1000))
import pycom

if config.powersaving==False:
    print("%.3f sec.: wifi on boot" % ((utime.ticks_ms()-beginning)/1000))
if pycom.wifi_on_boot() == True:
    pycom.wifi_on_boot(False)

# print("%i msec.: WLAN OFF" % ((utime.ticks_ms()-beginning)))
# wlan = WLAN()
# wlan.deinit()

# print("%i msec.: bluetooth off" % ((utime.ticks_ms()-beginning)))
# bluetooth = Bluetooth()
# bluetooth.deinit()

######################
##### Mount SD #######
######################
if config.sdmandatory:
    if config.powersaving==False: print("%.3f sec.: mounting SD card" % ((utime.ticks_ms()-beginning)/1000))
    from machine import SD
    try:
        sd = SD()
        if config.powersaving==False: print("_________SD-Card ok")
        sd_mounted = True
    except OSError as err:
        sd_mounted = False
        if config.powersaving==False: print("_________SD not working: {0}".format(err))
    if sd_mounted:
        os.mount(sd, '/sd')
        try:
            os.stat('/sd/data.csv')
            if config.powersaving==False: print("_________Data file exists on SD card")
        except:
            if config.powersaving==False: print("_________NO data file on SD card!!")
            f=open('/sd/data.csv',"w")
            if config.withRTC: f.write("date time, epoch time (UTC),")
            f.write("counter, nprobe, volt1,")
            if config.nsensors==2: f.write("volt2,")
            if config.nsensors==3: f.write("volt3,")
            f.write("battery\n")
            f.close()
            print("_________Data file created on SD card")

if config.powersaving==False:
    print("%.3f sec.: heartbeat off" % ((utime.ticks_ms()-beginning)/1000))
pycom.heartbeat(False)  #disable the pycom heartbeat
if config.powersaving==False:
    print("%.3f sec.: boot done" % ((utime.ticks_ms()-beginning)/1000))
