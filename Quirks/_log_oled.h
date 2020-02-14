/*
 * LOG EVERY ACTIONS (message) on serial (if activate) and in a log file on the SD card 
 */

/*
 * OLED
 * message
 * oled:0 --> nothing on the OLED screen
 * oled:1 --> display measures all in one OLED screen
 * oled:2 --> scroll measures on the OLED screen
 * oled:3 --> display parameters all in one OLED screen
 * oled:4 --> scroll parameters on the OLED screen
 *  
 *  parameter: oledon: epoch time corresponding to the start of the OLED screen
 *  ==> OLED screen stopped after 'periodOLED' minutes = 30*60 seconds
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin) - was 4 before (but 4 is for the SD card)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void blinker(int howlong) { //blink for howlong seconds
  digitalWrite(LED_BUILTIN, HIGH); delay(howlong);  // turn the LED on and wait
  digitalWrite(LED_BUILTIN, LOW); delay(howlong);   // turn the LED off and wait
}

String print2digits(int number) { //print correctly dates
  String numback="";
  if (number < 10) { numback="0";} // print a 0 before if the number is < than 10
  numback=numback+String(number);
  return numback;
}

String nowthatIcanread() {
  String datetime=print2digits(rtc.getDay())+"/"+print2digits(rtc.getMonth())+"/"+print2digits(rtc.getYear())+" "+print2digits(rtc.getHours())+":"+print2digits(rtc.getMinutes())+":"+print2digits(rtc.getSeconds());
  return datetime;
}

void OLEDlms(String what) { //show a short message on the OLED screen until a new message erase it.
    lastaction="OLEDlms";
    display.clearDisplay();
    display.setTextWrap(true);
    if (what.length()<21) {display.setTextSize(2);display.setCursor(0, 20);}
    else if (what.length()<40) {display.setTextSize(2);display.setCursor(0, 0);}
    else {display.setTextSize(1);display.setCursor(0, 20);}
    display.println(what);
    display.display();
    display.setTextSize(1);
}

void OLEDsms(String what) { //show a short message on the OLED screen for a short time (5 seconds)
    OLEDlms(what);
    lastaction="OLEDsms";
    delay(5000);
    display.clearDisplay();
    display.display();  
}

void OLEDsetup() { //initialize the OLED screen, once per startup
  lastaction="OLEDsetup";
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.print("OLED SSD1306 allocation failed, --> not good!");
    oled=0; // prevent using the oled screen
  }
  display.clearDisplay(); // Clear the buffer
  display.setTextColor(WHITE);
  OLEDsms("Restarted - "+versioncode);
}

void loggerln(String whattolog) { //log all message on SD card and serial (if needed)
  lastaction="loggerln";
  String filename =print2digits(rtc.getMonth())+print2digits(rtc.getDay())+LOGGERFILE;
  if (logoff==0) {
    lastaction="logSD";
    myFile = SD.open(filename, FILE_WRITE);
    if (myFile) {
      myFile.println(nowthatIcanread()+","+ whattolog);
      myFile.close();
    }  
  }
  if (withserial) {lastaction="logSerial"; Serial.println(nowthatIcanread()+": "+ whattolog);}
  if (oled==1 || oled==2) {
    lastaction="logOLED";
    //take only the string before the coma "," to have short message on screen//
    int p=0;
    for (int z=0 ; z<whattolog.length() ; z++)  {
      char u = whattolog.charAt(z);
      if (u == ',') {p = z; z=whattolog.length();} //if one coma is found, save the position and quit the loop
      delay (50); //==> usefull???
    }
    if (!p==0) {whattolog = whattolog.substring(0,p);} //keep only the first part of the log
    OLEDlms(whattolog); //show the current log on the OLED screen
 }
 lastaction="logEND";
}
  
void OLEDoff() { //clear the display and stop any other display
  lastaction="OLEDoff";
    OLEDsms("display is OFF");
    oled=0;   //stop OLED screen
}

void OLEDallInOne(String head, String texttodisplay) {
  lastaction="OLEDallInOne";
  display.clearDisplay();
  display.setTextWrap(true);
  display.setTextSize(1);                     // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);                     // Start at top-left corner
  display.println(head);
  display.println(texttodisplay);
  display.display();
  delay(5000);
}

void OLEDscroll(String head, String scroll) {
  lastaction="OLEDscroll";
  const char* messhead = head.c_str();
  const char* messscroll = scroll.c_str();
  int  x, minX;
  display.setTextWrap(false);
  x = display.width();
  minX = -12 * strlen(messscroll); // 12 = 6 pixels/character * text size 2
  while (x>minX) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(messhead);
    display.setTextSize(2);
    display.setCursor(x, 20);
    display.print(messscroll);
    display.display();
    if(--x < minX) x = display.width(); // check if x=1 (true) then do x=x-1
  }
}

void showthedata() { //show the measures on the OLED screen (from main programme if oled=1 or oled=2)
  lastaction="showthedata";
  String dat="";
  if (oled==1) { //6 pixels/character and 128 pixels / line ==> 21 characters / line!
    dat=dat+"#"+String(counter,4)+" & ";
    dat=dat+"bat="+String(batlevel,2)+" & ";
    dat=dat+"water level="+String(wlev,3)+" & ";
    dat=dat+"head level="+String(hlev,3)+" & ";
    dat=dat+"tem="+String(otttemp,2)+" & ";
    dat=dat+"flow="+String(flow,4)+" & ";
    dat=dat+"underwater:"+underwater+" & ";
    dat=dat+"cumul vol="+String(vcumul,3);
    OLEDallInOne(nowthatIcanread(), dat);
    delay(25000);
  }

  if (oled==2) {
    dat=dat+"#"+String(counter,4);
    dat=dat+", bat="+String(batlevel,2);
    dat=dat+", water level="+String(wlev,3);
    dat=dat+", head level="+String(hlev,3);
    dat=dat+", temp="+String(otttemp,2);
    dat=dat+", flow="+String(flow,4);
    dat=dat+", underwater:"+underwater;
    dat=dat+", cumul vol="+String(vcumul,3);
    OLEDscroll(nowthatIcanread(), dat);
    display.clearDisplay();
    display.display();
  }
  if ((rtc.getEpoch() - oledon.toInt()) >= periodOLED*60) { //test whether the time the OLED screen is on is more than autorized
    OLEDoff();
  }  
}
