/****************************************************** 
 * ----------- Paramters saved on the SD Card --------- 
******************************************************/
void InitParameters() { //**SHOULD have the same parameters as function SaveParameters ()**
  lastaction="InitParameters-1";
  String msg="Reading   parameters,";
  batco=readFile("batco").toFloat(); //loggerln("'----batco="+String(batco,4));
  coeffb=readFile("coeffb").toFloat(); //loggerln("'----coeffb="+String(coeffb,4));
  coeffc=readFile("coeffc").toFloat(); //loggerln("'----coeffc="+String(coeffc,4));
  counter = readFile("counter").toInt(); //loggerln("'----counter="+String(counter));
  logoff = readFile("logoff").toInt(); //loggerln("'----logoff="+String(logoff));
  myphone =readFile("myphone"); //loggerln("'----myphone="+myphone);          //phone number of the board
  nogs = readFile("nogs").toInt(); //loggerln("'----nogs="+String(nogs));
  nosms = readFile("nosms").toInt(); //loggerln("'----nosms="+String(nosms));
  off1=readFile("off1").toFloat(); //loggerln("'----off1="+String(off1,4));
  off2=readFile("off2").toFloat(); //loggerln("'----off2="+String(off2,4));
  oled = readFile("oled").toInt(); //loggerln("'----oled="+String(oled));  //Oled 1, 2, 3 or 4 to display measures or paramters on the OLED screen
  oledon =readFile("oledon"); //loggerln("'----oledon="+oledon);          //OLED timestamp to now since when the OLED is on
  period = readFile("period").toFloat(); //loggerln("'----period="+String(period));
  samnum = readFile("samnum").toInt(); //loggerln("'----samnum="+String(samnum));
  samon = readFile("samon").toInt(); //loggerln("'----samon="+String(samon));
  samweek = readFile("samweek").toInt(); //loggerln("'----samweek="+String(samweek));
  sitenam =readFile("sitenam"); //loggerln("'----sitenam="+sitenam);
  sleepm = readFile("sleepm").toInt(); //loggerln("'----sleepm="+String(sleepm));
  stamp =readFile("stamp"); //loggerln("'----stamp="+stamp);
  timeot = readFile("timeot").toInt(); //loggerln("'----timeot="+String(timeot));
  vcumul=readFile("vcumul").toFloat(); //loggerln("'----vcumul="+String(vcumul,1));
  vevent=readFile("vevent").toFloat(); //loggerln("'----vevent="+String(vevent,4));
  vpulse=readFile("vpulse").toFloat(); //loggerln("'----vpulse="+String(vpulse,4));
  watch = readFile("watch"); //loggerln("'----watch="+String(watch));
  wband=readFile("wband").toFloat(); //loggerln("'----wband="+String(wband,4));
  wpulse=readFile("wpulse").toFloat(); //loggerln("'----wpulse="+String(wpulse,4));
  loggerln(msg+"done!");
  lastaction="InitParameters-2";
  smsloop=true;       //will write "waiting sms for the first next time in the SMS loop
}

void SaveParameters() { //**SHOULD have the same parameters as function InitParameters()**
  lastaction="SaveParameters-1";
  String msg="Saving parameters, ";
  ChangeParameter ("batco" , String(batco,4));
  ChangeParameter ("coeffb" , String(coeffb,4));
  ChangeParameter ("coeffc" , String(coeffc,4));
  ChangeParameter ("counter" , String(counter));
  ChangeParameter ("logoff" , String(logoff));
  ChangeParameter ("myphone" , myphone);
  ChangeParameter ("nogs" , String(nogs));
  ChangeParameter ("nosms" , String(nosms));
  ChangeParameter ("off1" , String(off1,4));
  ChangeParameter ("off2" , String(off2,4));
  ChangeParameter ("oled" , String(oled));
  ChangeParameter ("oledon" , oledon);
  ChangeParameter ("period" , String(period));
  ChangeParameter ("samnum" , String(samnum));
  ChangeParameter ("samon" , String(samon));
  ChangeParameter ("samweek" , String(samweek));
  ChangeParameter ("sitenam" , sitenam);
  ChangeParameter ("sleepm" , String(sleepm));
  ChangeParameter ("stamp" , stamp);
  ChangeParameter ("timeot" , String(timeot));
  ChangeParameter ("vcumul" , String(vcumul,4));
  ChangeParameter ("vevent" , String(vevent,4));
  ChangeParameter ("vpulse" , String(vpulse,4));
  ChangeParameter ("watch" , watch);
  ChangeParameter ("wband" , String(wband,4));
  ChangeParameter ("wpulse" , String(wpulse,4));
  loggerln(msg+"done!");
  lastaction="SaveParameters-2";
  smsloop=true;       //will write "waiting sms for the first next time in the SMS loop
}
