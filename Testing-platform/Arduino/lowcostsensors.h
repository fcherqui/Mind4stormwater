/* All about the low-cost sensors:
 *  - JSN_SRT04: Ultrasonic sensor
 *  - HC_SR04: Ultrasonic sensor ("two eyes")
 *  - VL53l0x: Laser "time of light" (infrared) ==> check parameters below to choose the mode
 *  - MS5803-1A: Pressure sensor ==> two sensors needed (in the air and in the water)
 */

//////////////////////////////////////////////
///       US SENSOR JSN_SRT04              ///
//////////////////////////////////////////////
void InitJSNSRT04() {
  pinMode(trigPin, OUTPUT); //define pin for US sensor trigger
  pinMode(echoPin, INPUT);  //define pin for US sensor echo
}

double measJSNSRT04(){
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(20);
  digitalWrite(trigPin, LOW);
  double duration = pulseIn(echoPin, HIGH);
  delay(50);
  return duration;
}

//////////////////////////////////////////////
///        US SENSOR HC_SR04               ///
//////////////////////////////////////////////
void InitHCSR04() {
  pinMode(trigPin, OUTPUT); //define pin for US sensor trigger
  pinMode(echoPin, INPUT);  //define pin for US sensor echo
}

double measHCSR04(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);
  double duration = pulseIn(echoPin, HIGH);
  return duration;
}

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

//////////////////////////////////////////////
///       PRESSURE SENSOR MS5803-1A        ///
//////////////////////////////////////////////
#include <SparkFun_MS5803_I2C.h>  //version=1.1.2 from here: https://github.com/sparkfun/SparkFun_MS5803-14BA_Breakout_Arduino_Library (http://librarymanager/All#SparkFun_MS5803-14BA)
//#include <Wire.h>               //required from I2C communication, already activated for the laser sensor
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

//////////////////////////////////////////////
///   NEXT SENSOR CODE TO COPY AND ADAPT   ///
//////////////////////////////////////////////
void InitSENSOR_NAME() {
 
}

double measSENSOR_NAME(){

  double duration = 99999999999999;
  return duration;
}
