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
 * - MS5803_1A (to be confirmed) - both sensors on the same pin, the resistor on one sensor change the adress
 *     - Vin: 3.3v (red)
 *     - GND: GND (blue)
 *     - SCL: SCL (yellow)
 *     - SDA: SDA (green)
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
#define DHTPIN   2              //DHT sensor pin
#define DHTTYPE DHT22           //Type of DHT sensor
