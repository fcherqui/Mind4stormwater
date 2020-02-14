//libraries and library instance (objects), already in libs.h
//#include <WDTZero.h>            //Watch! The dog will bite you if you sleep too much!!! https://github.com/javos65/WDTZero
//WDTZero MyWatchDoggy;     //Define WDT

/******************************************* 
 * ----------- WATCHDOG FUNCTIONS  --------- 
********************************************/
void Myshutdown() { //programme to be executed before shutdown
  String reallastaction=lastaction;
  loggerln("**************************************");
  loggerln("Too long to wait for "+lastaction=+"!!!!, proceding to reset !");
  String nowitis = String(rtc.getEpoch());
  myFile = SD.open("watchdog.txt", FILE_WRITE);
  if (myFile) {
    myFile.println("reset:,"+ nowitis +","+nowthatIcanread());
    myFile.close();
  }
  ChangeParameter("watch", reallastaction);
  loggerln("**************************************");
  digitalWrite(RELAYprobe_PIN, LOW); //shutdown the probe is case it is not already done...
  digitalWrite(LED_BUILTIN, LOW); //led off
}

void Watchdogstart() { //watchdog function, bites if you're sleeping!
 MyWatchDoggy.attachShutdown(Myshutdown);
 MyWatchDoggy.setup(watchtime);  // initialize WDT-softcounter refesh cycle on 2 minutes interval
}

/*WATCHDOG
 * MyWatchDoggy.setup(WDT_SOFTCYCLE2M);  // initialize WDT-softcounter refesh cycle on 2 minutes interval
 * Other durations: WDT_SOFTCYCLE8S, WDT_SOFTCYCLE16S, WDT_SOFTCYCLE32S, WDT_SOFTCYCLE1M, WDT_SOFTCYCLE2M, WDT_SOFTCYCLE4M, WDT_SOFTCYCLE8M, WDT_SOFTCYCLE16M
 */
