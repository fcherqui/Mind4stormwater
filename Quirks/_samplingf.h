/********************************************* 
 * ----------- Sampling functions ----------- 
*********************************************/
void StartSampling() { //start the sampling procedure
  lastaction="StartSampling-1";
  digitalWrite(RELAYsampler_PIN, HIGH); //start sampling with a pulse to the sampler
  delay(pulse_duration); //pulse in milliseconds
  digitalWrite(RELAYsampler_PIN, LOW); //stop sampling
  delay(1000); //wait a little, why not?
  samnum+=1;
  loggerln("Start sampling #"+String(samnum));
  lastaction="StartSampling-2";
  String GSorder=String("\"TYP\":\"Order\",\"FRO\":\""+myphone+"\",\"MSG\":\"Sample-"+String(samnum)+"\"}");
  PubToGS(GSorder);
  lastaction="StartSampling-3";
}

void TestSampling() { //test if sampling if required
  lastaction="TestSampling-1";
  String msg="Test sampling...";
  int dayoftheweek=0;
  if (samweek==1) {dayoftheweek=Weekday(stamp);} //Day of the week, 0=Sunday...6=Saturday
  msg=msg+", Day if the week:"+String(dayoftheweek)+" (0:Sunday or desactivate)";
  msg=msg+", wlev="+String(wlev,3)+", wpulse="+String(wpulse,3)+", wband="+String(wband,3)+", samon="+String(samon);
  if (wlev>wpulse && samon==0 && dayoftheweek!=5 && dayoftheweek!=6) {// if level reached + day ISNOT Friday (5) or Saturday (6) in UTC time (Melbourne -10 hours or -8 hours)
      msg=msg+",!!Water level reached, start the sampling procedure";
      if (!connected) {ConnecToTheWorld();} // connect to internet if not already to send all measures and receive commands
      StartSampling(); //start the sampler
      samon=1;  //sampling process started
      vevent=0; //ready to monitor the cumulated volume of the event
  }
  lastaction="TestSampling-2";
  while (samon==1 && (wlev>(wpulse-wband)) && vevent>vpulse) {
    StartSampling();
    vevent=vevent-vpulse;
    msg=msg+", Water level still high and volume triggered: start the sampler, volume after sampling:" + String(vevent,4) + " m3";
    PublishData(); //publish the data online, same counter but new cumulated volume
    SaveData(); //save the data on the SD card, same counter but new cumulated volume
    if (vevent>vpulse) {delay(5000);} //wait 5 seconds because there will be another pulse for sampling
  }
  lastaction="TestSampling-3";
  if (samon==1 && (wlev<=(wpulse-wband))) {
    msg=msg+", END of sampling procedure";
    samon=0;
    vevent=0; //ready to monitor the cumulated volume of the next event
    PubToGS("\"TYP\":\"Order\",\"FRO\":\""+myphone+"\",\"MSG\":\"Sampling...done!\"}");
  }
  lastaction="TestSampling-4";
  loggerln(msg);
  lastaction="TestSampling-5";
}
