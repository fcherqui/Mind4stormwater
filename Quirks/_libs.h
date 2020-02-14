#include "0_phonebook.h"        //config file!!
#include "_variables.h"    //to use all the variables from the other file (configuration file)

//libraries and instances (object) called now because needed in log_oled.h
//SD card
#include <SD.h>                 //to manage files on SD Card
File myFile;                        //specific class to use files
//RTClock
#include <RTCZero.h>            //Managing Real Time Clock of the MKR and sleepmode
RTCZero rtc;              //Real Time clock
//watchdog
#include <WDTZero.h>            //Watch! The dog will bite you if you sleep too much!!! https://github.com/javos65/WDTZero
WDTZero MyWatchDoggy;     //Define WDT

#include "_log_oled.h"           //log messages on SD card, the serial monitor and the OLED screen
#include "_clockf_gsmf.h"             //all about clock management
#include "_googlef.h"            //all about google spreadsheet
#include "_sdcardf.h"            //all about SD Card management
#include "_measuresf.h"          //all functions related to measures
#include "_samplingf.h"          //all functions related to sampling
#include "_paramf.h"             //all functions related to paramters saved on SD Card
#include "_watchf.h"             //all functions related to sleep mode et watchdog
#include "_smsf.h"               //all functions related to sms...
#include "_Initf.h"             //all about clock management
#include "_sleepf.h"             //all functions related to sleep mode et watchdog
