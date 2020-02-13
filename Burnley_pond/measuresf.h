/**************************************************** 
 * ----------- MEASURES functions --------- 
****************************************************/
// libraries
#include <SDI12.h>              //to use SDI-12 communication protocol

// initialize the library instance (objects)
SDI12 mySDI12(data_PIN);  //initialization of the SDI12

/**************************************************** 
 * ---- probe COMMANDS (for CAMpabell or OTT) ----- * 
****************************************************/
//(default) SDI 12 commands for both OTT and Campbell probe
String SDI12Address = "?!";   //ask the adress of the probe
String SDI12AddTOzero = "A0!";   //force the adress of the probe to be zero (need to add the address in front (ex: 1A0! : change sensor adress from 1 to 0)
String SDI12Id      = "0I!";  //ask the identification
String SDI12Measure = "0M!";  //ask to start a measurement
String SDI12Send    = "0D0!"; //ask to send the measure

//(default) SDI 12 commands specific to a OTT probe (not available for CS)
String SDI12Unit    = "0OSU!";//ask for the unit of the measure, +0=m, +1=cm, +2=ft, +3=mbar, +4=psi
String SDI12ChgUnit = "0OSU0!";//change the unit of the measure to +0=m
String SDI12Offset = "0OAB!";//ask the offset
String SDI12ChgOffset = "0OAB0!";//ask the offset

/**************************************************** 
 *       ---- read answer from the probe  -----     * 
****************************************************/
String readsensor(){ //SDI 12 readings from the sensors
  String sdiResponse = "";
  delay(30);
  while(mySDI12.available()){    // write the response to the screen
    char c = mySDI12.read();
    if ((c != '\n') && (c != '\r')) { sdiResponse += c; delay(5); }
  }
  return sdiResponse;
}

/***************************** 
 * ---- Probe measure  ----- * 
*****************************/
void OttMeasure() { //do the measurement with the OTT probe
  mySDI12.begin(); delay(2000); //start SDI communication and wait a little for the probe to be ready

  if (isitthestart) {
    OLEDlms("Init...");
    Serial.println("Initialisation of the sensor");
    String msge="----Adress of the sensor: ";
    mySDI12.sendCommand(SDI12Address); delay(300);
    String sensoraddress=readsensor();
    delay(500);mySDI12.clearBuffer();
    if (sensoraddress!="0") { 
      Serial.println(" wrong address, was: "+sensoraddress+", will be change to 0...");
      mySDI12.sendCommand(sensoraddress+SDI12AddTOzero);
      delay(500);mySDI12.clearBuffer();
      mySDI12.sendCommand(SDI12Address);
      delay(300);
      sensoraddress=readsensor();
    }
    Serial.println(msge+sensoraddress);
    delay(500);mySDI12.clearBuffer();
    
    msge="----Identification of the sensor: ";
    mySDI12.sendCommand(SDI12Id);delay(500);
    String sensorid=readsensor();
    Serial.println(msge+sensorid);
    delay(500);mySDI12.clearBuffer();

    msge="(ONLY FOR OTT)Unit 0+(0=m, 1=cm, 2=ft, 3=mbar, 4=psi): ";
    mySDI12.sendCommand(SDI12Unit); delay(300);
    String sensorunit=readsensor();
    delay(500);mySDI12.clearBuffer();
    if (sensorunit!="0+0") { 
      Serial.print("(ONLY FOR OTT) wrong unit, was: "+sensorunit+", will be change to 0 (meters)...");
      mySDI12.sendCommand(SDI12ChgUnit);
      delay(300);Serial.println(readsensor());
      delay(500);mySDI12.clearBuffer();
      mySDI12.sendCommand(SDI12Unit);
      delay(300);sensorunit=readsensor();
    }
    Serial.println(msge+sensorunit);
    delay(500);mySDI12.clearBuffer();

    msge="(ONLY FOR OTT)Offset: ";
    mySDI12.sendCommand(SDI12Offset); delay(300);
    String sensoroffset=readsensor();
    delay(500);mySDI12.clearBuffer();
    if (sensoroffset!="0+0.000") { 
      Serial.print("(ONLY FOR OTT) wrong offset, was: "+sensoroffset+", will be change to 0 (meters)...");
      mySDI12.sendCommand(SDI12ChgOffset);
      delay(2000);Serial.print(readsensor());
      delay(500);mySDI12.clearBuffer();
      delay(300);Serial.println(readsensor());
      delay(500);mySDI12.clearBuffer();
      mySDI12.sendCommand(SDI12Unit);
      sensoroffset=readsensor();
    }
    Serial.println(msge+sensoroffset);
    delay(500);mySDI12.clearBuffer();

    display.clearDisplay();
    display.setTextWrap(true);
    display.setTextSize(1);                     // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);                     // Start at top-left corner
    display.println(sensorid);
    display.println("address:"+sensoraddress);
    display.println("unit:"+sensorunit+ "(OTT)");
    display.println("offset:"+sensoroffset+ "(OTT)");
    display.println("  Cf. manual");
    display.println("all zero, all good!");
    display.display();
    delay(5000);
  }
  isitthestart=false;
  
   mySDI12.sendCommand(SDI12Measure); delay(300);
   Serial.println("'----Start measure,0xxxy (xxx seconds to measure y values) "+readsensor());
   delay(3000); mySDI12.clearBuffer(); //time necessary to wait for the measure (3s for Campbell and 2 seconds for OTT)
   mySDI12.sendCommand(SDI12Send); delay(300);
   String rawdata = readsensor();
   Serial.println("'----Ask values: "+rawdata);
   delay(500); mySDI12.clearBuffer();

   int p=0; int pos[] = {0,0};
   for (int z=0 ; z<rawdata.length() ; z++)  {
     char u = rawdata.charAt(z);
     if (u == '+' || u == '-') { pos[p] = z ; p++; } 
     delay (50);
   }
   ottlev = (rawdata.substring(pos[0],pos[1])).toFloat(); //in meters
   otttemp = (rawdata.substring(pos[1],rawdata.length())).toFloat(); //in degrees C
   Serial.println("[m]ottlev = "+ String(ottlev,3) + ", [C]temp= "+ String(otttemp));  
}

/***************************** 
 * ---- Measure cycle  ----- * 
*****************************/
void Measures() { //do the different measurements
  digitalWrite(LED_BUILTIN, HIGH); //LED on: start of the measure
  counter += 1; // increment the counter (count the number of measure)
  if (counter>65000) {counter=1;}
  String msg="#"+String(counter/1000)+String((counter/100)%10)+String((counter/10)%10)+String(counter%10);
  OttMeasure();
  waterlevel=ottlev+offset;   //calcul the real water level
  digitalWrite(LED_BUILTIN, LOW); //led off because end of the measure
  display.clearDisplay();
  display.setTextWrap(true);
  display.setTextSize(2);                     // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);                     // Start at top-left corner
  display.println(msg);
  display.println(String(waterlevel,3)+" m");
  display.println("("+String(ottlev,3)+" m)");
  display.println(String(otttemp)+ " C");
  display.display();
  msg="Measure "+msg+", [m]water lev = "+ String(waterlevel,3) +", [m]ottlev = "+ String(ottlev,3) + ", [C]temp= "+ String(otttemp);  
  Serial.println(msg+", End of measure");
  measures[0]=counter;
  measures[1]=(waterlevel+0.0005)*1000; //0.0005 added to have correct round
  measures[2]=otttemp*5;
}
