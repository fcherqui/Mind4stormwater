/* All about the low-cost sensors:
 *  - JSN_SRT04: Ultrasonic sensor
 *  - HC_SR04: Ultrasonic sensor ("two eyes")
 *  - VL53l0x: Laser "time of light" (infrared) ==> check parameters below to choose the mode
 *  - MS5803-1A: Pressure sensor ==> two sensors needed (in the air and in the water)
 *  - INA219: Current sensor to use with the ALS pressure sensor
 *  - ADS115: voltage sensor ADC converter
 */

//////////////////////////////////////////////
///       US SENSOR JSN_SRT04              ///
//////////////////////////////////////////////
void InitJSNSRT04() {
  pinMode(trigPin, OUTPUT); //define pin for US sensor trigger
  pinMode(echoPin, INPUT);  //define pin for US sensor echo
}

String measJSNSRT04(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); //was delay(2) until 25/11/2020 but should have been delayMicroseconds(2)...strange
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20); //was delay(20) until 25/11/2020 but should have been delayMicroseconds(20)...strange
  digitalWrite(trigPin, LOW);
  double duration = pulseIn(echoPin, HIGH);
  float dist=duration/2*0.000343;
  delay(50);
  String answer=String(duration)+","+String(dist,3);
  return answer;
}

/* comment for memory limit problems
//////////////////////////////////////////////
///        US SENSOR HC_SR04               ///
//////////////////////////////////////////////
void InitHCSR04() {
  pinMode(trigPin, OUTPUT); //define pin for US sensor trigger
  pinMode(echoPin, INPUT);  //define pin for US sensor echo
}

double measHCSR04(){
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(20);
  digitalWrite(trigPin, LOW);
  double duration = pulseIn(echoPin, HIGH);
  return duration;
}
*/

/* comment for memory limit problems
//////////////////////////////////////////////
///       LASER SENSOR VL53l0x             ///
//////////////////////////////////////////////
#include <Wire.h>           //required from I2C communication 
#include <VL53L0X.h>        //Version: 1.2.0 from here: https://github.com/pololu/vl53l0x-arduino
VL53L0X vl53;
#define LONG_RANGE       true
#define HIGH_ACCURACY    true
#define HIGH_SPEED       false

void InitVL53l0x() {
  Wire.begin();
  vl53.setTimeout(500);
  if (!vl53.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  #if defined LONG_RANGE    
    vl53.setSignalRateLimit(0.1);                               // lower the return signal rate limit (default is 0.25 MCPS)
    vl53.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18); // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    vl53.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  #endif
  
  #if defined HIGH_ACCURACY
    vl53.setMeasurementTimingBudget(200000);                    // increase timing budget to 200 ms
  #endif
  
  #if defined HIGH_SPEED
    vl53.setMeasurementTimingBudget(20000);                     // reduce timing budget to 20 ms (default is about 33 ms)
  #endif
}

unsigned int measVL53l0x(){
  unsigned int distance = vl53.readRangeSingleMillimeters(); //distance in millimeters
  if (distance<3000) {return distance;}
  else {return 0;}
}
*/

//////////////////////////////////////////////
///       PRESSURE SENSOR MS5803-1A        ///
//////////////////////////////////////////////
#include <SparkFun_MS5803_I2C.h>  //version=1.1.2 from here: https://github.com/sparkfun/SparkFun_MS5803-14BA_Breakout_Arduino_Library (http://librarymanager/All#SparkFun_MS5803-14BA)
#include <Wire.h>               //required from I2C communication, already activated for the laser sensor
MS5803 sensor1(ADDRESS_HIGH);     //pressure sensor with ??? connected to the ground / Vcc???
MS5803 sensor2(ADDRESS_LOW);      //pressure sensor with ??? connected to the ground / Vcc???

void InitMS5803_1A() {
  Wire.begin(); //communication with the sensors
  sensor1.reset();
  sensor1.begin();
  sensor2.reset();
  sensor2.begin(); 
}

String measMS5803_1A(){
  float press1= float(sensor1.getPressure(ADC_4096))/10; //pressure in mbar = hPa (= 10.19716 mm H2O), ADC_4096 = resolution of 0.012 mbar (= 0.122 mmH20!)
  float press2= float(sensor2.getPressure(ADC_4096))/10; //pressure in 10*mbar = hPa (= 10.19716 mm H2O), ADC_512, resolution of 0.008°C
  float temp1= sensor1.getTemperature(CELSIUS, ADC_512); //temperature in °C
  float temp2= sensor2.getTemperature(CELSIUS, ADC_512); //temperature in °C
  String answer = String(press1)+", "+String(temp1)+", "+String(press2)+", "+String(temp2);
  return answer;
}

//////////////////////////////////////////////////////////////////
///   INA219 CONNECTED TO A PRESSURE SENSOR (CURRENT SENSOR)   ///
//////////////////////////////////////////////////////////////////
//#include <Wire.h>                 //required from I2C communication
#include <Adafruit_INA219.h>      //version 1.0.9 from https://github.com/adafruit/Adafruit_INA219
// this library requires the library Adafruit_BusIO version 1.3.2 from https://github.com/adafruit/Adafruit_BusIO

Adafruit_INA219 ina219; //to declare the sensor

void InitINA219() {
  uint32_t currentFrequency;
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  ina219.setCalibration_16V_400mA();
}

String measINA219(){
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();
  float power_mW = ina219.getPower_mW();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);

  String answer = String(shuntvoltage)+", "+String(loadvoltage)+", "+String(current_mA)+", "+String(power_mW);
  return answer;
}

//////////////////////////////////////////////////
/// PRESSURE SENSOR CONNECTED TO INA219        ///
//////////////////////////////////////////////////
#include "HX711.h" // version 0.7.4 from https://github.com/bogde/HX711
HX711 scale1;
HX711 scale2;
byte gain1=128; //gain, can be 32, 64 and 128
byte gain2=32; //gain, can be 32, 64 and 128

void InitH710B() {
  scale1.begin(DOUT_PIN1, SCK_PIN1); // Initializing the scale
  scale2.begin(DOUT_PIN2, SCK_PIN2); // Initializing the scale
  scale1.set_gain(gain1); //options are 128, 64 and 32
  scale2.set_gain(gain2); //options are 128, 64 and 32
  scale1.power_up();
  scale2.power_up();
//  scale1.power_down();
//  scale2.power_down(); 
  delay(500); //required before any measure
}

String measH710B(){
//  delay(500); //required if scale.read is used and after startup
  String Value1;
  String Average12;
  if (scale1.is_ready()) {
    Value1= String(scale1.read());          // print a raw reading from the ADC
    Average12=scale1.read_average(2);     // print the average of 2 readings from the ADC
  } else {
    Value1= "not found";
    Average12="error";
  }
  delay(500); //required if scale.read is used
  String Value2;
  String Average22;
  String result;
  if (scale2.is_ready()) {
    Value2= String(scale2.read());          // print a raw reading from the ADC
    Average22=scale2.read_average(2);     // print the average of 2 readings from the ADC
  } else {
    Value2= "not found";
    Average22="error";
  }
  result=Value1+","+Average12+","+Value2+","+Average22+","+gain1+"/"+gain2;
  return result;
}

//////////////////////////////////////////////
///   ADC 1115 voltage coverter   ///
//////////////////////////////////////////////
//#include <Wire.h> //for I2c
#include <Adafruit_ADS1015.h> // fro; https://learn.adafruit.com/adafruit-4-channel-adc-breakouts/arduino-code
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
//Adafruit_ADS1115 ads1115(0x49);  // construct an ads1115 at address 0x49
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
void InitADS1115() {
 ads.begin();
}

int measADS1115(){
//  int16_t adc0;
//  adc0 = ads.readADC_SingleEnded(0);
  return ads.readADC_SingleEnded(0);
}

//////////////////////////////////////////////
///   NEXT SENSOR CODE TO COPY AND ADAPT   ///
//////////////////////////////////////////////
void InitSENSOR_NAME() {
 
}

double measSENSOR_NAME(){

  double duration = 99999999999999;
  return duration;
}

///////////////////////////////////////////
///       LOW COST INITIALISATION       ///
///////////////////////////////////////////
void lowcosinit() {
  //Start the low-cost sensor choosen for the test
  if (LCsensor=="JSN_SRT04") {InitJSNSRT04();}
//  if (LCsensor=="HC_SR04") {InitHCSR04();}
//  if (LCsensor=="VL53l0x") {InitVL53l0x();}
  if (LCsensor=="MS5803_1A") {InitMS5803_1A();}
  if (LCsensor=="INA219") {InitINA219();}
  if (LCsensor=="H710B") {InitH710B();}
  if (LCsensor=="ADS1115") {InitADS1115();}
}


///////////////////////////////////////////
///       LOW COST MEASURE              ///
///////////////////////////////////////////
void lowcostmeasure() {
    if (LCsensor=="JSN_SRT04") {LCsensor_TMP=measJSNSRT04();}
//    if (LCsensor=="HC_SR04") {LCsensor_TMP=String(measHCSR04());}
//    if (LCsensor=="VL53l0x") {LCsensor_TMP=String(measVL53l0x());}
    if (LCsensor=="MS5803_1A") {LCsensor_TMP=measMS5803_1A();}
    if (LCsensor=="INA219") {LCsensor_TMP=measINA219();}
    if (LCsensor=="H710B") {LCsensor_TMP=measH710B();}
    if (LCsensor=="ADS1115") {LCsensor_TMP=measADS1115();}
}
