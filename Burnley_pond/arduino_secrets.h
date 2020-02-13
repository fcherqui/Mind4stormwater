//DON'T CHANGE parameters
#define RELAYprobe_PIN    2       //PIN linked to the relay to power of the probe  (relay1 = pin1 & relay2 = pin2)
#define data_PIN          A1      //PIN for SDI-12 communication with the probe

//INTERNAL VARIABLES
unsigned long startMillis;  // last start of the "thing"
unsigned long currentMillis; // that is now!
long int counter = 0;               //counter of measure
bool isitthestart= true;             //to know that it's the first loop
float ottlev = 0;                   //will be used for the water level from the OTT [m]
float waterlevel=0;                 //will be used for the real water level [m}
float otttemp = 0;                  //will be used for the water temperature from the OTT [C]

int measures[3]; // array of measures in the following order: [0]: counter (max 65000) & [1] = 1000*water level & [2] = 5*temperature
byte meas_bytes[5]={0,0,0,0,0}; //array of bytes (conversion from int measures): [0] & [1] = counter & [2] & [3] = pressure, [4] = temperature 1

void InitPins() {//Init all necessary pins {
  pinMode(LED_BUILTIN, OUTPUT); //set LED pin to output
  digitalWrite(LED_BUILTIN, LOW); //led off
  pinMode(RELAYprobe_PIN, OUTPUT); //PIN linked to the relay to power of the probe
}

void blinker(int howlong) { //blink for howlong seconds
  digitalWrite(LED_BUILTIN, HIGH); delay(howlong);  // turn the LED on and wait
  digitalWrite(LED_BUILTIN, LOW); delay(howlong);   // turn the LED off and wait
}

/*RELAY MKR PROTO SHIELD
 *  NO: Normally Open terminal
 *  COM: Common terminal
 *  NC: Normally Closed terminal
 *  http://www.omron.com.au/service_support/FAQ/FAQ03206/index.asp
 */
