/**************************************************** 
 * --------- WRITE TO OLED screen functions --------- 
****************************************************/

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

void OLEDlms(String what) { //show a short message on the OLED screen until a new message erase it.
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
    delay(5000);
    display.clearDisplay();
    display.display();  
}

void OLEDsetup() { //initialize the OLED screen, once per startup
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.print("OLED SSD1306 allocation failed, --> not good!");
  }
  display.clearDisplay(); // Clear the buffer
  display.setTextColor(WHITE);
  OLEDsms("REF_POND - "+versioncode);
}

void OLEDallInOne(String head, String texttodisplay) {
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
