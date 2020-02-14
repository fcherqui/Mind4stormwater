/// INITIALISATION PROCEDURES

void Initchecks() {
  if (watch!="0" && connected==true) {PubToGS("\"TYP\":\"Order\",\"FRO\":\"WATCH\",\"MSG\":\"Watchdog_"+watch+"\"}");watch="0";} //Inform online that the system has rebooted because of the watchdog...
  if (oled==1 || oled==2) {OLEDsms(sitenam+ " - " + myphone);} //show the measurement on the OLED screen
  if (myphone==default_myphone && connected==true) {PubToGS("\"TYP\":\"Order\",\"FRO\":\""+myphone+"\",\"MSG\":\"Board_phone_number_needed!\"}");} //Inform online that the system has rebooted because of the watchdog...
  while (myphone == default_myphone) {MyWatchDoggy.clear(); blinker(100); Readsms();}
  if (sitenam == default_sitenam && connected==true) {PubToGS("\"TYP\":\"Order\",\"FRO\":\""+myphone+"\",\"MSG\":\"Site_NAME_needed!\"}");} //Inform online that the system has rebooted because of the watchdog...
  while (sitenam == default_sitenam) {MyWatchDoggy.clear(); blinker(100); Readsms();}
  if (sitenam.length()<8) {logFile=sitenam+".csv";}
  loggerln("Saving in "+logFile+", on the SD card");
}

void InitPins() {//Init all necessary pins {
  lastaction="InitPins";
  USB->DEVICE.CTRLA.reg |= USB_CTRLA_ENABLE; // Enable USB (end of sleep) if stop during ultrasleep
  pinMode(BATTERY_PIN, INPUT); //set pin to read battery voltage
  pinMode(LED_BUILTIN, OUTPUT); //set LED pin to output
  digitalWrite(LED_BUILTIN, LOW); //led off
  pinMode(RELAYprobe_PIN, OUTPUT); //PIN linked to the relay to power of the probe
  pinMode(RELAYsampler_PIN, OUTPUT); //PIN linked to the relay to send a pulse to the sampler
  pinMode(SWITCH1_PINOUT, OUTPUT); //PIN to power the switch 1
  pinMode(SWITCH2_PINOUT, OUTPUT); //PIN to power the switch 2
  pinMode(SWITCH1_PINread, INPUT); //set pin to read battery voltage from the switch 1 and check the weir is not underwater
  pinMode(SWITCH2_PINread, INPUT); //set pin to read battery voltage from the switch 2 and check the weir is not underwater
  pinMode(timer_PIN, OUTPUT); //PIN connected to the timer relay
}

/*RELAY MKR PROTO SHIELD
 *  NO: Normally Open terminal
 *  COM: Common terminal
 *  NC: Normally Closed terminal
 *  http://www.omron.com.au/service_support/FAQ/FAQ03206/index.asp
 */
