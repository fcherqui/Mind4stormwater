# Read time from RTC - By: fcherqui@gmail.com - Sat Apr 16 2020
import utime, config
from machine import I2C
from machine import RTC

class _RTCsync: #read external RTC time and update the Lopy internal RCT
    def synctime():
        utime.timezone(config.diffUTC*60*60) #time difference between UTC and Brisbane (not by plane of course)
        # EPOCH IS CALCULATED BY CONSIDERING THAT THE UTC TIME IS IN THE RTC!!!
        rtc = RTC()
        DS3231_I2C_ADDR = 104 #I2C address of any DS3231 real-time clock
        #Start  I2C communication
        i2c = I2C(0, I2C.MASTER, baudrate=100000, pins=(config.sdaPin, config.sclPin)) #sda=P23, scl=P22
        i2c_components=i2c.scan()
        #print(i2c_components)
        if DS3231_I2C_ADDR in i2c_components:
            #print("RCT connected, all good")
            RTC_time = i2c.readfrom_mem(DS3231_I2C_ADDR, 0, 7) #real the whole time data from the RTC_time
            #Converting this raw datatime in something understandable
            ss = int(hex(RTC_time[0])[2:])
            mm = int(hex(RTC_time[1])[2:])
            hh = int(hex(RTC_time[2])[2:])
            DD = int(hex(RTC_time[4])[2:])
            MM = int(hex(RTC_time[5])[2:])
            YY = 2000+ int(hex(RTC_time[6])[2:])
            #updating the internal RCT of the Lopy4
            rtc.init((YY,MM,DD,hh,mm,ss,0,0))
            #print("Lopy4 Brisbane time: ",time.localtime())
            epoch_now=utime.time()
            epoch_base=1577836800 #epoch for the 01/01/2020 00:00:00
            epoch_diff=max(0,epoch_now-epoch_base) #epoch will be saved in 4 bytes (49710 days = 136 years), 3 bytes is too short (194 days = less than 1 year!!)
            #print("Lopy4 Unix time: %i & epoch difference = %i " % (epoch_now, epoch_diff))
            return epoch_diff
        else:
            print("?????????? No RTC connected, what happened? Setting time to 01/01/2020 00:00:00")
            rtc.init((2020,01,01,00,00,00,0,0)) #defaut RTC clock to 1st January 2020
            return 0
        #printing results
        # datetime = ("%02d/%02d/%04d %02d:%02d:%02d" % (DD,MM,YY,hh,mm,ss))
        # print("raw datatime from RCT= ", RTC_time)
        # print ("data & time from RTC: ", datetime)
        # print("Lopy4 UTC time: ",rtc.now())
        # print("Lopy4 Brisbane time: ",time.localtime())
        # print("Lopy4 Unix time: ",utime.time())
