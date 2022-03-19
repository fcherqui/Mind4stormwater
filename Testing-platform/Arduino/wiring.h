/*
 * Wiring of the system
 * - JSN_SRT04 or HC_SR04
 *     - Vin: 3.3v or 5v
 *     - GND: GND
 *     - Trig: Pin 8
 *     - Echo: Pin 9
 * 
 * - VL53L0X
 *     - Vin: 3.3v or 5V 
 *     - GND: GND 
 *     - SCL: SCL 
 *     - SDA: SDA
 * 
 * - MS5803_1A - both sensors on the same pin, the resistor on one sensor change the adress
 *     - Vin: 3.3v (red)
 *     - GND: GND (blue)
 *     - SCL: SCL (yellow)
 *     - SDA: SDA (green)
 *     
 * - INA219 (with a pressure based on current)
 *     - Vin- & Vin+: same wire of the pressure sensor (current monitoring)
 *     - Vcc: 3.3v or 5V
 *     - GND: GND
 *     - SCL: SCL
 *     - SDA: SDA 
 *     
 *  - H710B pressure sensor (two sensor for a differential pressure)
 *     - Vcc on 3 or 5V
 *     - GND... 
 *     sensor 1
 *     - OUT: A1
 *     - SCK: A0
 *     sensor 2
 *     - OUT: A2
 *     - SCK: A3
 */

//////////////////PINS////////////////////////////////////////
// Laser sensor and pressure sensor to the I2C pins //
#define tempSignalPin 13        //Air temp pin
#define DATA_PIN 12             //The pin of the SDI-12 data bus
#define echoPin  9              //US low cost sensor echo pin (Input)   
#define trigPin  8              //US low cost sensor trigger pin (Output)
#define PumpIn   6              //Pump relay control (Relay 2)
#define PumpOut  7              //Pump relay control (Relay 1)
#define PumpStop 5              //Relay pin disconnecting pump when not in use (Relay 3) put to low to stop the pump
#define DOUT_PIN1 A1            //H710B sensor 1 out pin
#define SCK_PIN1 A0             //H710B sensor 1 sck pin
#define DOUT_PIN2 A2            //H710B sensor 2 out pin
#define SCK_PIN2 A3             //H710B sensor 2 sck pin
#define DHTPIN   2              //DHT sensor pin
#define DHTTYPE DHT22           //Type of DHT sensor
