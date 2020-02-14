//parameters
#define server            "script.google.com" //GS URL
const String LINKtoURL = "/macros/s/????????????????????????????/exec"; // replace with the url to the correct google spreadsheet
 /*GOOGLE RESPONSE CODE
  * 1: all good!
  * 0: not transmitted
  * 200: ?
  * 302: Moved Temporarily...but transmitted so ok!
  */

/**************************************************** 
 * ----------- Google Spreadsheet functions --------- 
****************************************************/
void PubToGS(String jsonObject) {  //***SEND MESSAGE TO GOOGLE SPREADSHEET
  lastaction="PubToGS0";
  MyWatchDoggy.clear();
  if ((nogs!=2 || samon==1) && connected==true) { //publish to GS only if silent mode is off
    lastaction="PubToGS1";
    sigstrength = scanner.getSignalStrength();
    jsonObject= String("{\"SIT\":\"" + sitenam +  "\",\"UTC\":\"" + stamp + "\",\"GSM\":\"" + sigstrength + "\",") + jsonObject;
    String msg="Sending to GS, "+ jsonObject;
    if (client.connect(server, 443)) { //port 443 is the default for HTTPS
      lastaction="PubToGS2";
      client.println("POST "+ LINKtoURL +"?{" + " HTTP/1.1\nHost: " + String(server));
      client.println("Content-Length: "+String(jsonObject.length())+"\n");
      client.println(jsonObject);
      delay(350); //Wait to receive the response
      lastaction="PubToGS3";
      client.parseFloat();
      gsresp = String(client.parseInt());
      loggerln(msg);
      lastaction="PubToGS4";
      //loggerln("Response code, " + gsresp); // Print the response code. 202 indicates that the server has accepted the response
      for (int i=0; i<3; i++) {blinker(300);} //blink very fast = data send online
    }
    else {loggerln("Connection failed");} // if you didn't get a connection to the server:
    /* verbose desactivate because it's always the same message from google */
    /*if (!(gsresp=="1" || gsresp=="302")) { //verbose response from google if problem to publish...
      loggerln("\n\n====================================================================================");
      while (client.available()) { char c = client.read(); Serial.print(c);}
      loggerln("====================================================================================\n");
    }*/
    lastaction="PubToGS5";
    client.stop(); // Disconnect
  }
}

void PublishData() { //publish the data online
  lastaction="PublishData";
  gsmessage = String("\"TYP\":\"OTT\",\"NUM\":\"" + String(counter) + "\",\"BAT\":\"" + String(batlevel) + "\",\"HLE\":\"" + String(hlev,3) + "\",\"WLE\":\"" + String(wlev,3) + "\",\"TEM\":\"" + String(otttemp) + "\",\"FLO\":\""+String(flow,4) + "\",\"VEV\":\""+String(vevent,3)+ "\",\"SWI\":\""+underwater+ "\",\"VCU\":\""+String (vcumul,3)+"\"}");
  PubToGS(gsmessage); //publish data to google spreadsheet
}
