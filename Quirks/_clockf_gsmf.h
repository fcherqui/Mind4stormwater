//libraries and library instance (objects), comments if already in libs.h
//RTClock
//#include <RTCZero.h>            //Managing Real Time Clock of the MKR and sleepmode
//RTCZero rtc;              //Real Time clock

//MKRGSM
#include <MKRGSM.h>             //usefull to work with an MKR board
GSM gsmAccess(false);            //access to the network, true if you want to have all messages from the GSM chip
GSM_SMS sms;              //access to sms
//GSMLocation location;   //TO know the location --> in case I need the location...
GSMScanner scanner;       //to know the signal strength
GSMSSLClient client;      //use secure url (supposed to...)
GPRS gprs;                //to the date

//GSM parameters
#define PINNUMBER     "????"         // ask a friend or your telco!
#define GPRS_APN      "mdata.net.au" // replace your GPRS APN  --> "mdata.net.au" for ALDI
#define GPRS_LOGIN        ""      //no login required for the carrier for ALDI
#define GPRS_PASSWORD     ""      //no password required for the carrier for ALDI

/*************************************** 
 * ----------- RESET FUNCTION  --------- 
***************************************/
void(* resetFunc) (void) = 0;//declare reset function at address 0

/**************************************** 
 * ----------- CLOCK FUNCTIONS  --------- 
****************************************/
int Weekday(String thismoment) {// Calculate day of week with the Gaussian algorithm, 0=Sunday
  int dw=((thismoment.toInt()/86400)+4)%7;
  return dw;
}

void SynchroTime() { //take the time from GSM network
  lastaction="SynchroTime-1";
  loggerln("Fetching time");
  lastaction="SynchroTime-2";
  long int timeis=gsmAccess.getTime();
  rtc.setEpoch(timeis);
  needtime=false; //no need to fectch time!
  loggerln(nowthatIcanread()+", Utime = "+String(rtc.getEpoch())+", "+", week-day =" +String(Weekday(String(rtc.getEpoch()))));
  lastaction="SynchroTime-3";
  client.stop(); // Disconnect
  smsloop=true;       //will write "waiting sms for the first next time in the SMS loop
}

/*************************************** 
 * ----------- GSM FUNCTIONS  --------- 
***************************************/
void ConnecToTheWorld() { //connection to the GSM word
  lastaction="ConnecToTheWorld-1";
  loggerln("Connection & Time");
  lastaction="ConnecToTheWorld-2";
  String msg="Connect world, at "+print2digits(rtc.getSeconds())+ ", ";
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on
  gsmAccess.shutdown(); //Disconnects from the GSM network, seems to avoid timeout for gsmAccess.begin...
  delay(500); // wait need?
  lastaction="ConnecToTheWorld-3";
  gsmAccess.setTimeout(timeot*1000); //stop gsm after 'timeot' seconds if cannot connect
  gprs.setTimeout(timeot*1000); //stop gsm after 'timeot' seconds if cannot connect
  connected = false;  // connection state 
  int trials=0;       //limit for the numbers of connection trials...
  lastaction="ConnecToTheWorld-4";
  while (!connected  && (trials<4 || isitboot==true)) {
      if ((gsmAccess.begin(PINNUMBER) == GSM_READY)) {
      connected = true;
      msg=msg+print2digits(rtc.getSeconds())+": GSM Trial #"+String(trials)+": Connected!, ";
    } else {
      msg=msg+print2digits(rtc.getSeconds())+": GSM Trial #"+String(trials)+": Failed...,";
      delay(1000);
    }
    trials++;
  }
  lastaction="ConnecToTheWorld-5";
  trials=0;       //limit for the numbers of connection trials...
  connected = false;  // connection state
  lastaction="ConnecToTheWorld-6";
  while (!connected  && (trials<4 || isitboot==true)) {
      if (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY) {
      connected = true;
      msg=msg+print2digits(rtc.getSeconds())+": GPRS Trial GPRS #"+String(trials)+": Connected!";
    } else {
      msg=msg+print2digits(rtc.getSeconds())+": GPRS Trial GPRS #"+String(trials)+": Failed...,";
      delay(1000);
    }
    trials++;
  }
  loggerln(msg);
  lastaction="ConnecToTheWorld-7";
  if (connected ==true) {SynchroTime();sigstrength = scanner.getSignalStrength();connectionfailed=0;}
  else {connectionfailed=connectionfailed+1;}
  lastaction="ConnecToTheWorld-8";
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
}
