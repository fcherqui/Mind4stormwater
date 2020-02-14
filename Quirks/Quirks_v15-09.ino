/* PROGRAM TO CONTROL AN OTT PROBE, A SAMPLER AND SEND DATA FROM A MKR 1400 GSM board
Mind4Stormwater project: http://mind4stormwater.org, , feel free to use, reuse, modify, share!!
University of Melbourne / INSA Lyon
Frédéric Cherqui - fcherqui@gmail.com
----- last version January 2020 ----
 * 
 * See _CONFIGhere.h sheet for required inputs and detail of functions...
 * See _versionhistory to understand the evolution of the code
 *  
*/ 
const String versioncode =" v15-09"; //version of the code!  
#include "_libs.h"               //useful libraries & co, easier to put there!

void setup() {
  Serial.begin(9600); // initialize serial communications and wait for port to open:
  rtc.begin(); // initialize RTC
  OLEDsetup();
  Sleep_Init(); //init sleep mode
  InitPins();   //Init all necessary pins
  Watchdogstart(); //start the watchdog...
  SDinit(); //initialize the SD card
  loggerln("Warming up...");
  InitParameters(); //read the parameters from the SD card
  ConnecToTheWorld(); //connect to the network & get the time from the network
  isitboot=false;
  Initchecks();
}

void loop() {
  lastaction="loopWDclear";
  MyWatchDoggy.clear(); // Reset watchdog with every loop to make sure the sketch keeps running.
  lastaction="loopReadsms";
  if (connected) {Readsms();}                              // Check if there are any SMS available only if connected to the network!

  //SYNC TIME always (drifting is something like a minute / hour!!), needtime = false when SynchroTime has been executed and = true after a measure
  lastaction="loopTime";
  if (needtime==true) {
    if (!connected) {ConnecToTheWorld();} //connect if not alreardy
    else {SynchroTime();}
  }
  
  //-- MEASUREMENT STEP --//
  lastaction="loopTestmeasure";
  if ((rtc.getEpoch() - stamp.toInt() >= (period*60)) || (mesnow==true)) { //test whether the period has elapsed or a measure is forced
    lastaction="loopStartMeasure";
    mesnow=false;       //back to normal (force measure if equal 1 so normal = 0)
    smsloop=true;       //will write "waiting sms for the first next time in the SMS loop
    Measures();         //measure all (level, temp, battery, flow, volume) !!
    SaveData();         //save the data on the SD card
    lastaction="loopDisplayData";
    if (oled==1 || oled==2) {showthedata();} //show the measurement on the OLED screen
    if (nogs==0 || ((counter % 5 == 0) && nogs==1)) { //publish the data online: nogs = 0 all measures on GS, nogs = 1 every 5 measures on GS, nogs = 2 never but connection needed!)
      if (!connected) {ConnecToTheWorld();} //connect if not aleardy
      PublishData();
    }
    lastaction="loopTestSampling";
    TestSampling();     //test if sampling is necessary
    lastaction="loopSave";
    SaveParameters();   //save the parameters on the SD Card
    lastaction="loopSleepWait";
    if (connectionfailed>3) { //in case the sim chip failed to connect several (3) times, do a software reset to try to connect
      loggerln("Board_reboot_to_reconnect...");
      PubToGS("\"TYP\":\"Order\",\"FRO\":\""+myphone+"\",\"MSG\":\"Board_reboot_to_reconnect...\"}");
      resetFunc(); //call reset
    }
    if (sleepm==1 && samon !=1) {IdleMode();}    //go to sleep only if IdleMode activated and not sampling
    if (sleepm==2 && samon !=1) {UltraSleepMode();}    //go to sleep only if sleepmode activated      
  }
}
