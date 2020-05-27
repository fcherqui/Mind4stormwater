/*
 * ALL VARIABLES
 * Variables to change are in experimental parameters
 */
//////////////////////////////////////////////
///      EXPERIMENT PARAMETERS             ///
//////////////////////////////////////////////
float LOWER_LIM = 0.000;             //Used to switch direction of pump (default = 0)
float UPPER_LIM = 1.90;          //Used to switch direction of pump and stop program (max=1.9, more than that and the floor is wet)

const int NB_TICK_PER_LEVEL=  1000;   //Number of ticks per water level (default=1000)
const int NB_REF_PER_LEVEL=   10;     //Number of reference water level measures per level (default=10)
const int NB_LC_PER_LEVEL=    1000;   //Number of low-cost measures per water level (default=1000)
const int NB_DS_PER_LEVEL=    100;    //Number of Dallas temperature measures per water level (default=100)
const int NB_DHT_PER_LEVEL=   100;    //Number of DHT temperature/air humidity measures per water level (default=100)

#define TIME_PUMP      5000    //time (in milliseconds) for the pump to work (at each step), default=5000, , i.e. 5 seconds (5 seconds = 0.03 m --> 6mm/second), min = 5s because of the computer memory
#define STABILIZE_TIME 20000   //Time to stabilize water level after pump off (defaut=15000, i.e. 15 seconds)
const float Offset PROGMEM =    0.150;  //Offset (distance OTT sensor to the 0, default=0.150, if you have to change this, it meas that the reference sensor has drifted...)
//PROGMEM to save the constant in the flash memory to save some space, see https://www.arduino.cc/reference/en/language/variables/utilities/progmem/

//////////////////////////////////////////////
///        INTERNAL VARIABLES              ///
//////////////////////////////////////////////
const byte REF_PER_LEVEL PROGMEM = max(1,NB_TICK_PER_LEVEL/NB_REF_PER_LEVEL); //for each level, ref measure every REF_PER_LEVEL measures
const byte LC_PER_LEVEL PROGMEM  = max(1,NB_TICK_PER_LEVEL/NB_LC_PER_LEVEL);  //for each level, low-cost measure every LC_PER_LEVEL measures
const byte DS_PER_LEVEL PROGMEM  = max(1,NB_TICK_PER_LEVEL/NB_DS_PER_LEVEL);  //for each level, DS temp measure every DS_PER_LEVEL measures
const byte DHT_PER_LEVEL PROGMEM = max(1,NB_TICK_PER_LEVEL/NB_DHT_PER_LEVEL); //for each level, DHT measure every DHT_PER_LEVEL measures

//////////////Initialising variables//////////////////
float TempDS_TMP;
float DHTHumidity_TMP;
float DHTTemp_TMP;
String LCsensor_TMP;

float Watertemp;     //Water temperature from the reference sensor
float LevelCheck; //level measured with the reference sensor
float leakCheck_TMP;
boolean goingUp=false;
boolean goingUpsaid=false;
byte LevelNum;
unsigned long SampleNum; //int is too small!
unsigned int SampleNum_per_level;
