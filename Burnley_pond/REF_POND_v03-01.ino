/*
 * PROGRAM TO RECEIVE DATA FROM PLS OTT probe and sent it online through LoRaWAN using a MKR 1300 board
 * Mind4Stormwater project: http://mind4stormwater.org, , feel free to use, reuse, modify, share!!
 * University of Melbourne / INSA Lyon
 * Frédéric Cherqui - fcherqui@gmail.com
 ----- last version 01/01/2020 ----
 * 
*/ 
const float period = 5;   // measurement period in minutes
const float offset = 0.015; //offset to correct sensor level to water level
const String versioncode =" v03-01"; //version of the code!  
#include "libs.h"               //useful libraries & co, easier to put there!

void setup() {
  Serial.begin(9600); // initialize serial communications and wait for port to open
  startMillis = millis();  //initial start time
  //while(!Serial);
  InitPins();   //Init all necessary pins
  Serial.println("'*******************************************************************************");
  Serial.println("'**                  OTT/CS READER / LoRaWAN sender                           **");
  Serial.println("'**                            prog REF_POND                                  **");
  Serial.println("'**                           "+versioncode+"                                       **");
  Serial.println("'*******************************************************************************");
  OLEDsetup();
  JoinLoRaWAN();
//  digitalWrite(RELAYprobe_PIN, HIGH); //start of the probe
}

void loop() {
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  //-- MEASUREMENT STEP --//
  if ((currentMillis - startMillis >= period*60*1000) || (isitthestart==true))  //test whether the period has elapsed
  {
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
    Measures();         //measure all (level, temp, battery, flow, volume) !!
    Bytezer();          //convert the measures into 8 bytes (2 bytes for each measure)
    Publish();          //publish the values to TTN (LoRaWAN)
  }
}
