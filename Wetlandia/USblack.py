## US BLACK version 20191214 for JSN-SRT04 Ultrasonic sensor
# added: ignore 0 duration measures

import pycom #import the module that allows to use the pycom features, especially the LED
import time
import utime #import the module that allowed to use micro second clock, made for measuring the time between trigger and echo
import machine
import config
from machine import Pin
from math import *

timeout=65 #stop loop after 65 ms (max value = 65 000 us which is around 22 meters!)

class measureUSblack:
    def single_measure(trigpin,echopin):
        #initialization
        echo = Pin(echopin,mode=Pin.IN) #initialize the echo Pin from the one specified in config file
        trigger = Pin(trigpin,mode=Pin.OUT) #initialize the trigger Pin from the one specified in config file
        trigger(0) #turn off trigger pin for 2µs before starting the single_measure
        utime.sleep_us(2)

        #sending a sound wave with the trigger Pin for 20µs (10µs for HCSR04T)
        trigger(1)
        utime.sleep_us(20) # 20 microseconds for the trigger (specific to this sensor)
        trigger(0)

        #waiting for echo Pin to receive the rising edge of the sound wave
        startT=utime.ticks_ms() #used for the timeout test
        timediff=0 #used for the timeout test
        while echo() == 0 and (timediff<timeout):
            timediff=utime.ticks_ms()-startT #used for the timeout test
            pass

        start=utime.ticks_us() #used to measure the duration of the pulse

        if timediff<timeout: #if the first loop didn"t timemout, 2nd loop
            timediff=0
            startT=utime.ticks_ms()
        #waiting for echo Pin to receive the receding edge of the sound wave
        while echo() == 1 and (timediff<timeout):
            timediff=utime.ticks_ms()-startT
            pass
        finish=utime.ticks_us() #used to measue the duration of the pulse
        utime.sleep_ms(50) #pause to make sure there is no interferences with the next measure
        if timediff>=timeout:
            dtime=0 #loop timeout: invalid measure
            #print("problem measuring with timediff= %d ms" % timediff)
        else:
            dtime = (finish-start)/2
        return dtime #time is in µs

    def advanced_measuretime(trigpin,echopin):

        time_samples = [] #initialize a list to store the measures
        for count in range(config.nb_samples):
            newduration=measureUSblack.single_measure(trigpin,echopin)
            #print(newduration)
            if newduration !=0: #ignore duration of 0
                time_samples.append(newduration) #make a single measure and append it in the list
            #print("TimeSimple= %d us" % time_samples[count])

        if len(time_samples) !=0: #if all the measures = 0, there is no table!!
            time_samples= sorted(time_samples) #sort the list
            med = time_samples[int(len(time_samples)/2)]
            mean = sum(time_samples, 0.0)/len(time_samples)
            dev = sqrt(sum([(x-mean)**2 for x in time_samples])/len(time_samples))
            if mean<700 or mean>29999:
                return (0,0,0)
            else:
                return (med,mean,dev)
        else:
            return (0,0,0)
