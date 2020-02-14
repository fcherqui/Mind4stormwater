/*************************************** 
 * ----------- SMS FUNCTIONS --------- 
****************************************/
String whosnumber(char* phonum) { //give name associate to phone number
  lastaction="whosnumber-1";
  String num=String(phonum); // convert characters to string
  String who="n.a."; //by default unknown personn...
  int i=0;
  while (phonename[i] != "n.a.") {
    if (phonenum[i]==num) {who=phonename[i];}
    i=i+1;
  }
  lastaction="whosnumber-2";
  if (who=="n.a.") {return num;}
  else {return who;}
  lastaction="whosnumber-3";
}

void SendSMS (String message) { //send a sms to someone
  lastaction="SendSMS-1";
  String msg="Sms send, message=" + message + ", to " + whosnumber(sendernumber)+"..., ";
  if (nosms==0 || samon==1) {
    char txtMsg[200];
    message.toCharArray(txtMsg, 200);
    sms.beginSMS(sendernumber);
    sms.print(txtMsg);
    sms.endSMS();
    msg=msg+"Done!";
  } else {
    msg=msg+"NO because sms have been DESACTIVATED";
  }
  loggerln(msg);
  lastaction="SendSMS-2";
}

void diplayparam() { //display all important parameters on the OLED screen
  lastaction="diplayparam-1";
  InitParameters();
  String param="";
  param=param+"batcoeff="+String(batco,4)+" & ";
  param=param+"off1="+String(off1,4)+" & ";
  param=param+"off2="+String(off2,4)+" & ";
  param=param+"period="+String(period)+" & ";
  param=param+"samnum"+String(samnum)+" & ";
  param=param+"sleepm="+String(sleepm)+" & ";
  param=param+"water pulse="+String(wpulse,4)+" & ";
  param=param+"water band="+String(wband,4)+" & ";
  param=param+"vol pulse="+String(vpulse,4);
  lastaction="diplayparam-2";
  OLEDallInOne(nowthatIcanread(), param);
  lastaction="diplayparam-3";
}

void scrollparam() { //scroll all important parameters on the OLED screen
  lastaction="scrollparam-1";
  InitParameters();
  Serial.print("init oled screen");
  String param="";
  param=param+"batcoeff="+String(batco,4);
  param=param+", off1="+String(off1,4);
  param=param+", off2="+String(off2,4);
  param=param+", period="+String(period);
  param=param+", samnum"+String(samnum);
  param=param+", sleepm="+String(sleepm);
  param=param+", water pulse="+String(wpulse,4);
  param=param+", water band="+String(wband,4);
  param=param+", vol pulse="+String(vpulse,4);
  lastaction="scrollparam-2";
  OLEDscroll(nowthatIcanread(), param);
  lastaction="scrollparam-3";
}

void Readsms() { //check if a sms is received
  lastaction="Readsms-1a";
  MyWatchDoggy.clear();
  digitalWrite(LED_BUILTIN, LOW); delay(2000);   // turn the LED off and wait
  digitalWrite(LED_BUILTIN, HIGH); delay(500);  // turn the LED on and wait
  String whatyousay =""; //will be the content of the sms
  if (smsloop==true) {loggerln("Waiting   for sms");}
  lastaction="Readsms-1b";
  smsloop=false;
  lastaction="Readsms-1c";
  if (sms.available()) {
    lastaction="Readsms-2";
    int c;
    sms.remoteNumber(sendernumber, 20); // Get remote number
    while ((c = sms.read()) != -1) { whatyousay=whatyousay+(char)c; } // Read message bytes and save them 
    loggerln("SMS received: " + whatyousay + " from: "+ whosnumber(sendernumber));
    sms.flush(); // Delete message from modem memory
  
    //cutting the string in pieces... ;-)
    lastaction="Readsms-3a";
    int p=0;
    int pos[] = {0,0};
    for (int z=0 ; z<whatyousay.length() ; z++)  {
      char u = whatyousay.charAt(z);
      if (u == ':') {pos[p] = z; p++;} 
      delay (50);//==> usefull???
    }
    lastaction="Readsms-3b";
    String key = whatyousay.substring(0,pos[0]);
    String parameter = whatyousay.substring(pos[0]+1,pos[1]);
    String smsmessage=(whatyousay.substring(pos[1]+1,whatyousay.length()));
    lastaction="Readsms-3c";
    float valueP = smsmessage.toFloat();

    //loggerln("'--,key="+key+ ", parameter="+parameter+", value="+ String(valueP,4));
    if (key=="amour") {//action depending on the message
      lastaction="Readsms-4";
      String nameP = parameter + ".txt";
      if (parameter=="version" && smsmessage=="?") {
      lastaction="Readsms-4a";        
      PubToGS("\"TYP\":\"Order\",\"FRO\":\""+myphone+"\",\"MSG\":\"version="+versioncode+"\"}");
      }
      else if (SD.exists(nameP) && smsmessage=="?") {
        lastaction="Readsms-4b";
        //String GSorder=String("\"TYP\":\"Order\",\"FRO\":\"" + whosnumber(sendernumber) + "\",\"MSG\":\"" + whatyousay  + "\"}"); PubToGS(GSorder);
        //delay(1000);
        InitParameters();
        PubToGS("\"TYP\":\"Order\",\"FRO\":\""+myphone+"\",\"MSG\":\""+parameter+"="+readFile(parameter)+"\"}");
      }
      else if (SD.exists(nameP) && parameter!="stamp" && !((parameter=="period" || parameter=="vpulse" || parameter=="coeffb" || parameter=="coeffc") && valueP==0) && !(parameter=="mesnow" && valueP==1) && !(parameter=="samtest" && valueP==1) && !(parameter=="oled" && (valueP==3 || valueP==4))) { //prevent to change the timestamp or put period or vpulse or coeffc = 0
            lastaction="Sms_chgt_param";
            if (parameter=="sitenam" || parameter=="myphone") {ChangeParameter(parameter, smsmessage); }
            else { ChangeParameter(parameter, String(valueP,4));}
            if (parameter=="nosms" && valueP==0) {SendSMS("Back to normal: SMS will be sent if needed");} //at the end to allow sms back before trying to send one
            if (parameter=="oled" && (valueP==1 || valueP==2)) {
              ChangeParameter("oledon", String(rtc.getEpoch())); //at the end to allow sms back before trying to send one
              OLEDsms("OLED is ON for "+String(periodOLED)+" mn");
            }
            if (parameter=="oled" && valueP==0) {OLEDoff();} //stop OLED display
            String GSorder=String("\"TYP\":\"Order\",\"FRO\":\"" + whosnumber(sendernumber) + "\",\"MSG\":\"" + whatyousay  + "\"}"); PubToGS(GSorder);
            InitParameters();
      }
      else if ((parameter=="mesnow") && (valueP==1)) {
            lastaction="Sms_meas";
            mesnow=true;
            String GSorder=String("\"TYP\":\"Order\",\"FRO\":\"" + whosnumber(sendernumber) + "\",\"MSG\":\"" + whatyousay  + "\"}"); PubToGS(GSorder);
      }
      else if ((parameter=="connect") && (valueP==1)) {
            lastaction="Sms_connect";
            ConnecToTheWorld();
            String GSorder=String("\"TYP\":\"Order\",\"FRO\":\"" + whosnumber(sendernumber) + "\",\"MSG\":\"" + whatyousay  + "\"}"); PubToGS(GSorder);
      }
      else if ((parameter=="samtest") && (valueP==1)) {
            lastaction="Sms_samtest";
            String GSorder=String("\"TYP\":\"Order\",\"FRO\":\"" + whosnumber(sendernumber) + "\",\"MSG\":\"" + whatyousay  + "\"}"); PubToGS(GSorder);
            StartSampling();
            delay(10000); //wait 10 seconds for the second message to google...
            PubToGS("\"TYP\":\"Order\",\"FRO\":\""+myphone+"\",\"MSG\":\"Sampling...done!\"}");
      }
      else if ((parameter=="oled") && (valueP==3)) {
            lastaction="Sms_oled3";
            String GSorder=String("\"TYP\":\"Order\",\"FRO\":\"" + whosnumber(sendernumber) + "\",\"MSG\":\"" + whatyousay  + "\"}"); PubToGS(GSorder);
            loggerln("oled 3: displaying parameters");
            diplayparam();
      }
      else if ((parameter=="oled") && (valueP==4)) {
            lastaction="Sms_oled4";
            String GSorder=String("\"TYP\":\"Order\",\"FRO\":\"" + whosnumber(sendernumber) + "\",\"MSG\":\"" + whatyousay  + "\"}"); PubToGS(GSorder);
            loggerln("oled 4: displaying parameters");
            scrollparam();
      }
      else if ((parameter=="reset") && (smsmessage=="test")) {
          lastaction="Sms_reboot";
          loggerln("Board_reboot_as_asked...");
          PubToGS("\"TYP\":\"Order\",\"FRO\":\""+myphone+"\",\"MSG\":\"Board_reboot_as_asked...\"}");
          resetFunc(); //call reset
      }
      else {loggerln("Parameter doesn't exist or not allowed for message: "+ whatyousay); SendSMS("parameter doesn't exist or not allowed");}
      lastaction="Sms_wrongparameter";
    }
    //else { loggerln("Wrong format sms: '" + whatyousay +"'"); SendSMS("wrong format, req: 'key:parameter:value'");} send sms if wrong format (disable)
  }
  lastaction="ReadsmsEnd";
}
