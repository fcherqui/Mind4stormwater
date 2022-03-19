/* All about the PLS OTT sensor
 *  (could also work with a Cambell Scientific CS451)
 *  Based on the SDI 12 protocol
 */
#include <SDI12.h> //Library
SDI12 mySDI12(DATA_PIN); // Define the SDI-12 bus

String readsensor(){ //SDI 12 readings from the sensors
  String sdiResponse = "";
  delay(30);
  while(mySDI12.available()){    // write the response to the screen
    char c = mySDI12.read();
    if ((c != '\n') && (c != '\r')) { sdiResponse += c; delay(5); }
  }
  return sdiResponse;
}

void measREFsensor(){
  delay(500);
  mySDI12.sendCommand("0M!"); //ask to start a measurement
  while(mySDI12.available()){ mySDI12.read();}    // write the response of the sensor
  delay(1000);
  mySDI12.clearBuffer();
  delay(500);
  mySDI12.sendCommand("0D0!"); //ask to send the measure
  delay(50);
  String rawdata = readsensor();
  mySDI12.clearBuffer();
  delay(50); 

  //Decoding string sent from probe
  int p=0;
  int pos[] = {0,0};
  for (int z=0 ; z<rawdata.length() ; z++)  {
    char u = rawdata.charAt(z);
    if (u == '+' || u == '-') {
      pos[p] = z ;
      p++; 
    } 
    delay (50);
  }    

  float RealRefLevel = (rawdata.substring(pos[0],pos[1])).toFloat();  //Water level from the reference sensor (without correction)
  Watertemp = (rawdata.substring(pos[1],rawdata.length())).toFloat(); //Water temperature from the reference sensor
  LevelCheck = RealRefLevel-Offset;                                   //include offset
}

void StartRefSensor(){
  mySDI12.begin();
  delay(2000); //start SDI communication and wait a little for the probe to be ready
  Serial.print(F("'**** Identification of the sensor: "));
  mySDI12.sendCommand("0I!");
  delay(300);
  Serial.println(readsensor());
  delay(500);
  mySDI12.clearBuffer();
  
  Serial.print(F("'**** Adress of the sensor: "));
  mySDI12.sendCommand("?!"); //ask the adress of the probe
  delay(300);
  Serial.println(readsensor());
  delay(500);
  mySDI12.clearBuffer();
  Serial.print(F("'**** Unit 0+(0=m, 1=cm, 2=ft, 3=mbar, 4=psi): "));
  mySDI12.sendCommand("0OSU!"); //ask for the unit of the measure, +0=m, +1=cm, +2=ft, +3=mbar, +4=psi
  delay(300);
  Serial.println(readsensor());
  delay(500);
  mySDI12.clearBuffer();
}
