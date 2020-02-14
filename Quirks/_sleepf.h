/*************************************** 
 * ----------- SLEEP FUNCTIONS --------- 
****************************************/
void ISR() { } //interrupt service routine (ISR), called when interrupt is triggered  executes after MCU wakes up (needed but can be empty)

void Sleep_Time(int sleepduration) { /*set new alarm*/
    lastaction="Sleep_Time";
    rtc.setAlarmEpoch(rtc.getEpoch() + sleepduration*60);
    rtc.enableAlarm(RTCZero::MATCH_HHMMSS);   // Exact alarm time in hour, minute, seconds

/* previous version...    
    int alarmMin = (rtc.getMinutes() + sleepduration) % 60;
    rtc.setAlarmMinutes(alarmMin);   // Set the alarm
    rtc.setAlarmSeconds(rtc.getSeconds());   // Set the alarm
    rtc.enableAlarm(RTCZero::MATCH_MMSS);   // Match only seconds (Periodic alarm every minute)
    rtc.attachInterrupt(ISR); //creates an interrupt that wakes the SAMD21 which is triggered by a FTC alarm
*/
}

void Sleep_Init() { //Init sleepmode
  lastaction="Sleep_Init";
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; // Set sleep mode
}

void SleepNow() { //start and end of the sleeping period
  lastaction="SleepNow";
  digitalWrite(LED_BUILTIN, LOW); //led off
  digitalWrite(RELAYprobe_PIN, LOW); //shutdown the probe in case it is not already done...
  digitalWrite(RELAYsampler_PIN, LOW); //shutdown the probe in case it is not already done...
  digitalWrite(timer_PIN, LOW); //shutdown the probe in case it is not already done...
  digitalWrite(SWITCH1_PINOUT, LOW); //shutdown the switch 1 in case it is not already done...
  digitalWrite(SWITCH2_PINOUT, LOW); //shutdown the switch 2 in case it is not already done...
  for (int i = 0; i < 20; i++) { pinMode(i, INPUT); } //ALL PIN TO INPUT TO SAVE POWER, (seems better than OUTPUT!)
  USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE; // Disable USB, doesn't seems to make a big difference...
  __WFI(); //Enter sleep mode
  /*==Sweet dreams MKR...==*/
  InitPins();   //need to put back the Pin in OUTPUT + !!!
}

void UltraSleepMode() { //call SleepNow() function
  lastaction="UltraSleepMode";
  int sleep_time=max(1,period-2); //== could be 1 min before the measure!!!
  loggerln("RTCZERO sleep: "+String(sleep_time)+" mn");
  gsmAccess.shutdown(); //Disconnects from the GSM network identified on the SIM card by powering the modem off.
  connected = false; 
  //MyWatchDoggy.setup(WDT_OFF); //no watchdog during the sleep //--> is it necessary?
  Sleep_Time(sleep_time);
  loggerln("...Zzzz...");
  SleepNow();
  //Watchdogstart();
  SDinit(); //initialize the SD card
  loggerln("Time to work NOW!");
  needtime=true;  //need to connect and take time after sleep
}

void IdleMode() { //disconnect from the network to wait for the next measure
  lastaction="IdleMode";
  int sleep_time=max(1,period-2);
  loggerln("Idle offline for "+String(sleep_time)+" mn");
  gsmAccess.shutdown(); //Disconnects from the GSM network identified on the SIM card by powering the modem off.
  connected = false; 
  unsigned long startedWaiting = millis();
  int nouse=0;
  loggerln("(....................)");
  while (((millis() - startedWaiting) < (sleep_time)*60*1000)) { //wait for sleep-period minutes and blink twice regularly
    MyWatchDoggy.clear();
    nouse+=1; delay(1000);
    if (nouse % 30 ==0) {blinker(500); blinker(500);} //every 30 seconds blinks two times quickly
  }
  loggerln("Back to work!");
  needtime=true;  //need to connect and take time after sleep
}
