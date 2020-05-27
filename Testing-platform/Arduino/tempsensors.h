/* All about the temperature sensors:
 *  - DHT22 (air temperature & relative humidity)
 *  - DS18B20 air temperature
 */

//////////////////////////////////////////////
///     DALLAS SCIENTIFIC DS18B20          ///
//////////////////////////////////////////////
#include <DallasTemperature.h>
#include <OneWire.h>
OneWire oneWire(tempSignalPin);
DallasTemperature sensors(&oneWire);
DeviceAddress thermometer;

double meastempDS(){
  sensors.requestTemperatures(); 
  double tempC = sensors.getTempCByIndex(0);
  delay(20);
  return tempC;
}

//////////////////////////////////////////////
///             DHT SENSOR                 ///
//////////////////////////////////////////////
#include "DHT.h" //DHT_sensor_library v 1.3.7 from https://github.com/adafruit/DHT-sensor-library, requiers also Adafruit_Unified_Sensor v 1.0.3 from https://github.com/adafruit/Adafruit_Sensor
DHT dht(DHTPIN, DHTTYPE);
