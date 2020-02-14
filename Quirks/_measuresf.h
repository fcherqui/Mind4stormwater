/**************************************************** 
 * ----------- MEASURES functions --------- 
****************************************************/
// libraries
#include <SDI12.h>              //to use SDI-12 communication protocol
SDI12 mySDI12(data_PIN);  //initialization of the SDI12

//SDI 12 commands
#define myComAdress   "?!"    //ask the adress of the probe
#define myComId       "0I!"   //ask the identification
#define myComMeasure  "0M!"  //ask to start a measurement
#define myComSend     "0D0!"  //ask to send the measure
#define myComUnit     "0OSU!" //ask for the unit of the measure, +0=m, +1=cm, +2=ft, +3=mbar, +4=psi

String switch1() {//test if switch 1 is underwater
  lastaction="switch1-1";
  digitalWrite(SWITCH1_PINOUT, HIGH); //power switch 1 line
  delay(500);
  float sum = 0;
  float r = analogRead(SWITCH1_PINread); //first measure not saved, it is often wrong...
  for (int k = 0 ; k<10 ; k++) {
     float r = analogRead(SWITCH1_PINread);
     sum = sum + r/310; //division to give a value in volts... (1023 = 3.3 volts)
     delay(100);
   }
  sum = sum / 10.0 ;
  digitalWrite(SWITCH1_PINOUT, LOW); //unpower switch 1 line
  loggerln("Testing switch 1, sum="+String(sum,2)+" and volt limit="+String(default_switch,2));
  lastaction="switch1-2";
  if (sum>default_switch) {return "1";}
  else {return "0";}
}

String switch2() {//test if switch 1 is underwater
  lastaction="switch2-1";
  digitalWrite(SWITCH2_PINOUT, HIGH); //power switch 1 line
  delay(500);
  float sum = 0;
  float r = analogRead(SWITCH2_PINread); //first measure not saved, it is often wrong...
  for (int k = 0 ; k<10 ; k++) {
     float r = analogRead(SWITCH2_PINread);
     sum = sum + r /310; //division to give a value in volts... (1023 = 3.3 volts)
     delay(100);
   }
  sum = sum / 10.0 ;
  digitalWrite(SWITCH2_PINOUT, LOW); //unpower switch 1 line
  loggerln("Testing switch 2, sum="+String(sum,2)+" and volt limit="+String(default_switch,2));
  lastaction="switch2-2";
  if (sum>default_switch) {return "1";}
  else {return "0";}
}

void Underwatertest() {
  lastaction="Underwatertest-1";
  underwater=switch1();
  lastaction="Underwatertest-2";
  if (switch2enable) {underwater=underwater+switch2();} //if two switchs installed
  else {underwater=underwater+switch1();} //if only one switch installed
  lastaction="Underwatertest-3";
  if (underwater=="00") {
    loggerln("NO switch underwater, --> bottom of weir is above water ");
  }
  else if (underwater=="11") {    
    loggerln("BOTH switch underwater, --> bottom of weir is UNDER water ");
  }
  else {
    loggerln("switch UNCLEAR ");
  }
  lastaction="Underwatertest-4";
}

void BatMeasure() { //measure the battery voltage
  lastaction="BatMeasure-1";
  float sum = 0;
  float r = analogRead(BATTERY_PIN); //first measure not saved, it is often wrong...
  for (int k = 0 ; k<10 ; k++) {
     r = analogRead(BATTERY_PIN);
     sum = sum + r / batco;
     delay(100);
   }
   batlevel = sum / 10.0 ;
   loggerln("Bat meas., "+String(batlevel));
  lastaction="BatMeasure-2";
}

void VolumeFlow() { //calculation of the volume
  lastaction="VolumeFlow-1";
  loggerln("Calculation of flow & volume");
  if (hlev>0) {flow=coeff1*coeffc*coeffb*pow(hlev,1.5);} else {flow=0;}
  //loggerln("'----flow=" + String(flow,4) + " m3/s, stamp dif: " + stampold +" - "+ stamp);
  lastaction="VolumeFlow-2";
  if (stampold.toInt()!=0) {
    String msg="'----vcumul:" + String(vcumul,4) + " --> ";
    vcumul=vcumul+(stamp.toInt()-stampold.toInt())*flow;
    //loggerln(msg+String(vcumul,4) + " m3");
    msg="'----vevent: " + String(vevent,4) + " --> ";
    if (samon==1) {vevent=vevent+(stamp.toInt()-stampold.toInt())*flow;} //increase cumulated volume for event if sampling in progress
    //loggerln(msg+String(vevent,4) + " m3");
  }
  lastaction="VolumeFlow-3";
}

String readsensor(){ //SDI 12 readings from the sensors
  lastaction="readsensor-1";
  String sdiResponse = "";
  delay(30);
  lastaction="readsensor-2";
  while(mySDI12.available()){    // write the response to the screen
    char c = mySDI12.read();
    if ((c != '\n') && (c != '\r')) { sdiResponse += c; delay(5); }
  }
  lastaction="readsensor-3";
  return sdiResponse;
}

void OttMeasure() { //do the measurement with the OTT probe
  lastaction="OttMeasure-1";
  mySDI12.begin(); delay(1000); //start SDI communication and wait a little for the probe to be ready
  
/*PART BELOW TO DO ONLY ONCE to check address and unit of the probe
  Serial.print("----Identification of the sensor: ");
  mySDI12.sendCommand(myComId); delay(300);loggerln(readsensor());
  delay(500);mySDI12.clearBuffer();
  Serial.print("----Adress of the sensor: ");
  mySDI12.sendCommand(myComAdress); delay(300);loggerln(readsensor());
  delay(500);mySDI12.clearBuffer();
  Serial.print("----Unit 0+(0=m, 1=cm, 2=ft, 3=mbar, 4=psi): ");
  mySDI12.sendCommand(myComUnit); delay(300); loggerln(readsensor());
  delay(500); mySDI12.clearBuffer();
//END OF PART BELOW */

   mySDI12.sendCommand(myComMeasure); delay(300); //loggerln("'----Start measure, "+readsensor());
   delay(1300); mySDI12.clearBuffer(); //time necessary to wait for the measure
   mySDI12.sendCommand(myComSend); delay(300);
   String rawdata = readsensor();
   loggerln("OTT measure, Ask values: "+rawdata);
   delay(500); mySDI12.clearBuffer();
  lastaction="OttMeasure-2";

   int p=0; int pos[] = {0,0};
   for (int z=0 ; z<rawdata.length() ; z++)  {
     char u = rawdata.charAt(z);
     if (u == '+' || u == '-') { pos[p] = z ; p++; } 
     delay (50);
   }
   lastaction="OttMeasure-3";
   ottlev = (rawdata.substring(pos[0],pos[1])).toFloat(); //in meters
   otttemp = (rawdata.substring(pos[1],rawdata.length())).toFloat(); //in degrees C
   hlev=max(0,ottlev-off1); //head level: height of water from the bottom of the weir in meters, minimum is zero!!
   wlev=ottlev-off1+off2;   //water level in meters
  lastaction="OttMeasure-4";
   //loggerln("'----,[m]ottlev = "+ String(ottlev,3) + ", [m]hlev = "+String(hlev,3)+", [m]wlev = "+String(wlev,3)+", [C]temp= "+ String(otttemp));  
}

void Measures() { //do the different measurements
  lastaction="Measures";
  digitalWrite(LED_BUILTIN, HIGH); //LED on: start of the measure
  counter += 1; // increment the counter (count the number of measure)
  String msg="Measure #"+String(counter)+" done";
  digitalWrite(RELAYprobe_PIN, HIGH); //start of the probe
  needtime=true;  //new counter, need to check if time is needed
  stampold=stamp; //keep in memory the last timestamp of the previous measure
  stamp = String(rtc.getEpoch()); // timestamp
  long int stamptimeSTART=rtc.getEpoch();
  
  BatMeasure();
  OttMeasure();
  msg=msg+", [m]ottlev = "+ String(ottlev,3) + ", [C]temp= "+ String(otttemp);  
  Underwatertest();
  VolumeFlow();
  digitalWrite(RELAYprobe_PIN, LOW); //shutdown of the probe
  digitalWrite(LED_BUILTIN, LOW); //led off because end of the measure
  long int stamptimeFINISH=rtc.getEpoch();
  long int meastime=(stamptimeSTART-stamptimeFINISH)/1;
  loggerln(msg+",End of measure, measurement time="+String(meastime)+" seconds."); //==> to correct!!
}
