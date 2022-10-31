# Set time to the RTC - By: fcherqui@gmail.com - Sat Apr 16 2020
# YOU NEED TO PUT UTC TIME IN THE RTC!!!
import utime
from machine import I2C
from machine import RTC

rtc = RTC()
DS3231_I2C_ADDR = 104 #I2C address of any DS3231 real-time clock
utime.timezone(10*60*60) #time difference between UTC and Brisbane (not by plane of course)

#Start  I2C communication
i2c = I2C(0, I2C.MASTER, baudrate=100000, pins=('P23', 'P22')) #sda=P23, scl=P22
i2c_components=i2c.scan()

if DS3231_I2C_ADDR in i2c_components:
    # SETTING DATE - TIME
    timeData= b'\x00\x15\x01\x03\x13\x05\x20' # sec., mn, hours, dayofweek, dayofmonth, month, year
    #TIME IN UTC (Brisbane - 10 hours)
    i2c.writeto_mem(DS3231_I2C_ADDR, 0, timeData)
    print("time set in the external RTC")
else:
    print("No RTC connected, what happened? Failed to update external RTC")


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
#printing results
datetime = ("%02d/%02d/%04d %02d:%02d:%02d" % (DD,MM,YY,hh,mm,ss))
print("raw datatime from RCT= ", RTC_time)
print ("data & time from RTC: ", datetime)
print("Lopy4 UTC time: ",rtc.now())
print("Lopy4 Brisbane time: ",utime.localtime())
print("Lopy4 Unix time: ",utime.time())
