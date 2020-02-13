/**************************************************** 
 * ---------- SEND TO LoRaWAN TTN functions --------- 
****************************************************/
//librairy & object
#include <MKRWAN.h>
LoRaModem modem;


/**************************************************** 
 * ---------- LoRaWAN TTN parameters -------------- * 
****************************************************/
const String devAddr= "??????????";
const String nwkSKey= "????????????????????????????????";
const String appSKey= "????????????????????????????????";

/**************************************************** 
 * --------- CONVERT TO BYTE function ------------- * 
****************************************************/
void Bytezer() {
  Serial.println("Converting the measures into bytes:");
  meas_bytes[0]=measures[0]/256;
  meas_bytes[1]=measures[0]%256;
  meas_bytes[2]=measures[1]/256;
  meas_bytes[3]=measures[1]%256;
  meas_bytes[4]=measures[2];
  Serial.println("--Measures: | "+String(measures[0])+" | "+String(measures[1])+" | "+String(measures[2])+" | ");
}

void JoinLoRaWAN() {
  Serial.println("Connecting to LoRaWAN...");
  OLEDlms("Connect   LoRaWAN");
  digitalWrite(LED_BUILTIN, HIGH);
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(AS923)) { Serial.println("Failed to start module"); while (1) {} };
  Serial.println("--Lora modem started.\n--Your module version is: " + String(modem.version())+"\n--Your device EUI is: "+String(modem.deviceEUI()));
  Serial.println("--Connecting via ABP with:\n----Device Address: "+devAddr+"\n----NWS KEY: "+nwkSKey+"\n----APP SKEY: "+appSKey);

  int is_connected = modem.joinABP(devAddr, nwkSKey, appSKey);
  if (!is_connected) {
    Serial.println("--Something went wrong; are you indoor? Move near a window and retry");
    while (1) {     //== if cannot connect to LoRawan, will keep blinking fast until the watchdog reset all
      //MyWatchDoggy.clear(); // Reset watchdog to stay in this stupid loop FOREVER!
      digitalWrite(LED_BUILTIN, LOW); delay(150);   // turn the LED off and wait
      digitalWrite(LED_BUILTIN, HIGH); delay(150);  // turn the LED on and wait
    }
  }
  Serial.println("--Done, connected to LoRaWAN, ready to send!");
  digitalWrite(LED_BUILTIN, LOW);
}

void Publish() {
  Serial.println("Sending the measures through LoRaWAN:");
  digitalWrite(LED_BUILTIN, HIGH);
  modem.setPort(1);
  modem.beginPacket();
  modem.write(meas_bytes[0]); // send each byte one by one
  modem.write(meas_bytes[1]); // send each byte one by one
  modem.write(meas_bytes[2]); // send each byte one by one
  modem.write(meas_bytes[3]); // send each byte one by one
  modem.write(meas_bytes[4]); // send each byte one by one
  Serial.print("--Sendind: |"+ String(meas_bytes[0])+ " | "+String(meas_bytes[1])+ " | "+String(meas_bytes[2])+ " | "+String(meas_bytes[3])+ " | "+String(meas_bytes[4])+ " | ");
  Serial.println();
  int err = modem.endPacket(false); //put 'true' to have a feedback from TTN (err>0 if message sent correctly)
  digitalWrite(LED_BUILTIN, LOW);
}
