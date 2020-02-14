//configuration file
//DEBUG or PROGRAMMING parameters
#define withserial  true    //if you want all displayed on the serial monitor

//MEASUREMENT parameters and default values (default_ means that the value can be changed by sms)
//flow=coeff1 * coeffc * coeffb * hlev^3/2
const bool switch2enable=false;           //two switchs on the system?, if false only switch1 will be considered
#define coeff1            2.953     //default value, cannot be changed by sms
#define default_batco     59.89     //value to correct the reading of the battery level, LEVEL= reading * BATCOEFFICIENT (default=59.61 with 10 KOhms / 43 KOhms + external battery)
#define default_coeffb    1.2       //default value
#define default_coeffc    0.6       //default value
#define default_myphone   "unknown"        //default board phone number
#define default_period    6.0       //default duration [min] bewteen measurements (as string), min=2 minutes!
#define default_sitenam  "QKCxxxx"  //default site name: will trigger message is it not set by sms or on the SD card
#define default_switch    2.0       //value threshold for the switch to be considered ON, i.e. switch underwater (if more than the defaut value), max = 3.3V
#define default_vpulse    100.00    //default cumulated volume that start sampling [m3]
#define default_wband     100.00    //default water level difference [m] for the neutral zone, if (wlevel<wpulse-wband), sampling is stopped
#define default_wpulse    100.00    //default water level that start sampling [m]
#define periodOLED        15        //default period during which the OLED screen is ON (in minutes)
#define pulse_duration    30        //duration of the pulse to trigger one sample in MILLISECONDS!!
#define LOGGERFILE   "_log.csv"  //file where all logs are saved

//PIN definition
#define BATTERY_PIN       A4      //PIN to read the battery level
#define SWITCH1_PINread   A2      //PIN to read the voltage of the switch 1
#define SWITCH2_PINread   A3      //PIN to read the voltage of the switch 2
#define SWITCH1_PINOUT    3       //PIN to power the switch 1
#define SWITCH2_PINOUT    0       //PIN to power the switch 2
#define RELAYprobe_PIN    2       //PIN linked to the relay to power of the probe  (relay1 = pin1 & relay2 = pin2)
#define RELAYsampler_PIN  1       //PIN linked to the relay to power of the sampler  (relay1 = pin1 & relay2 = pin2)
#define data_PIN          A1      //PIN for SDI-12 communication with the probe
#define SD_CHIP_SELECT    4       //Communication PIN SD Card
#define timer_PIN         5       //PIN connected to the timer relay (6 LED built in?)
#define default_timeot    30      //defaut timeout for the connection to the network

//INTERNAL VARIABLES
float batco = default_batco;        //value to correct the reading of the battery level, LEVEL= reading * BATCOEFFICIENT
float batlevel = 0;                 //will be used to monitor and send the battery level [V]
bool isitboot = true;               //to know that the board just boot (for connection = mandatory!)
float coeffb=default_coeffb;        //coefficient used in the flow formulae (FLOAT)
float coeffc=default_coeffc;        //coefficient used in the flow formulae (FLOAT)
bool connected = false;             //status of the connection to the GSM world (true = connected!)
int connectionfailed = 0;           //count the number of connection problem
long int counter = 0;               //counter of measure
float flow = 0;                     //will be used to calculate the flow [m3/s]
String gsmessage="";                //message send to google spreadsheet
String gsresp = "";                 //response from Google spreadsheet
float hlev=0;                       //head level used to calculate the flow [m]
String lastaction="";               //keep the memory of the last action
String logFile="datalog.csv";       //log file name includes now the site name
int logoff=0;                       //disable the log on sd card (logoff=1) or keep it (logoff=0)
bool mesnow=false;                  //start a new measure if mesnow=true
String myphone="";                  //phone number of the MKR board
bool needtime=true;                 //to know if new time from GSM is needed
int nogs = 0;                       //0 if nogs is off and 1 to prevent from publishing on GS
int nosms=0;                        //0 if nosms is off and 1 to prevent from sending sms
float off1 = 0;                     //value to offset 1 (distance end of weir - probe) [m]
float off2 = 0;                     //value to offset 2 (distance end of weir - 0 on rule) [m]
int oled = 0;                       //1, 2, 3 or 4 if the OLED screen is ON
String oledon="";                   //timestamp of the OLED screen ON
float ottlev = 0;                   //will be used for the water level from the OTT [m]
float otttemp = 0;                  //will be used for the water temperature from the OTT [C]
float period = default_period;      // time intervall between measures in minutes
int samnum = 0;                     //number of samples taken for an event
int samon = 0;                      //1 if sampling is in process, 0 if no event to sample now
int samweek = 0;                    //1 if NO sampling during week-end, 0 if sampling always possible
char sendernumber[20];              //phone number
String sigstrength="";              //signal strength, need to be here because use to publish on GS and save on SD Card!
String sitenam = "";                //Name of the site
int sleepm = 0;                     //0 if sleepmode is off and 1 for Idle and 2 for Sleep
bool smsloop=true;                  //true when loop in sms waiting mode for the first time
String stamp = "";                  //number of seconds that have elapsed since January 1, 1970 UTC: see https://www.epochconverter.com/
String stampold = "";               //old timestamp of the previous mesure (used to calculated the volume that went through the weir)
int timeot = 30;                    //timeout time in seconds for the GSM and GPRS connection
String underwater="";                //condition "11" = underwater / "00" = above water based on the swith info / "01" or "10" : switch do not agree
float vcumul = 0;                   //cumulated volume already gone through the weir (Q.time)
float vevent = 0;                   //cumulated volume already gone through the weir SINCE SAMPLING HAS STARTED (Q.time)
float vpulse = default_vpulse;      //cumulative volume that will start the sampling (compared to vevent)
float wband = default_wband;        //stop sampling if water level below (wpulse-wband)
String watch ="0";                    //watch = something if watchdog reset last time (0 if no reset) and something = lastaction= last sub procedure called...
#define watchtime WDT_SOFTCYCLE2M   //watchdog time
float wlev=0;                       //water level used to validate the mesure [m]
float wpulse = default_wpulse;      //start sampling process if water level > wpulse
