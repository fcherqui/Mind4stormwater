 /* PROGRAM FOR THE TESTING BENCH (to test low cost sensor for water level monitoring) using an Arduino UNO
 * Mind4Stormwater project: http://mind4stormwater.org, , feel free to use, reuse, modify, share!!
 * University of Melbourne / INSA Lyon
 * Frédéric Cherqui - fcherqui@gmail.com
 * Thanks to Theo Perigaud and Kai James
 * ----- last version June 2020 ----
 * 
 * This code will fill up the calibration bench (PVC pipe), and then empty it while taking measurements at set increments
 * Once it has emptied it will then repeat the cycle while filling.
 * A full cycle will empty and then refill the pipe while taking measurements.
 * Please ensure to set the lower and upper limits properly so as not to dry out the pump, or overflow the storage containers
 * 
 * NEED TO DO:
 * - add safety to shutdown the experiement if the pump is running but the level is not changing
 * - measure again DS18B20 temp if value is -127 (means problem)
 * 
 * 
 //==> all comments to check!!
*/ 

String Arduinocodeversion="v04_01"; //version of the code
String LCsensor="ADS1115"; //choose the sensor to test: "JSN_SRT04" or "HC_SR04" or "VL53l0x" or "MS5803_1A" or "INA219" or "H710B" or "ADS1115"
String LCsensorbis="_ALS"; //complementary name of the sensor
String REFsensor="PLS_OTT";  //what is the reference sensor?

//////////////////////////////////////////////
///               LIBRARIES                ///
//////////////////////////////////////////////
#include "_variables.h"
#include "wiring.h"
#include "refsensor.h"
#include "lowcostsensors.h"
#include "pumpcontrol.h"
#include "tempsensors.h"
#include "_supportfunctions.h"

//////////////////////////////////////////////
///              SETUP PHASE               ///
//////////////////////////////////////////////
void setup(){
  initall(); // (in "_supportfunctions.h")
  measREFsensor();
  leakCheck_TMP=LevelCheck; //initial measure for the first level (used to check potential leaks)
  Serial.println(F("GOprocessing")); //trigger the save for processing (save in csv file)
  Serial.print(F("Id,Direction,Level_Num,Sample_Num,ref_sensor_name,"));
  Serial.print(F("ref_level_m,REF_Offset_m,Water_temp_C,DS_airtemp_C,DHT_airtemp_C,DHT_airrelhum,"));
  if (LCsensor=="JSN_SRT04") {Serial.print(F("duration_us,approx_dist_m"));}
  if (LCsensor=="HC_SR04") {Serial.print(F("duration_us"));}
  if (LCsensor=="VL53l0x") {Serial.print(F("distance_mm"));}
  if (LCsensor=="MS5803_1A") {Serial.print(F("Pression1_hPa,Temp1_C,Pression2_hPa,Temp2_C"));}
  if (LCsensor=="INA219") {Serial.print(F("Shunt Voltage,Load Voltage,Current,Power"));}
  if (LCsensor=="H710B") {Serial.print(F("Value1, Average12, Value2, Average22, gain"));}
  if (LCsensor=="ADS1115") {Serial.print(F("raw_number"));}
  Serial.println(F(",LCsensor_name,Arduino_Code"));
  LevelNum=0;
  SampleNum=0;
  SampleNum_per_level=0;
}

//////////////////////////////////////////////
///               LOOP PHASE               ///
//////////////////////////////////////////////
void loop(){
  ///////////////////
  // Update measures
  ///////////////////
  if(SampleNum_per_level%REF_PER_LEVEL==0||SampleNum_per_level==0){ //first tick or every xx ticks)
    measREFsensor();
    delay(1000); //wait one second if the reference sensor has been used in order to avoid any perturbation (voltage) to the low-cost sensor (it seems that there is a perturbation!!!)
    lowcostmeasure(); //one blank low-cost measure after the reference measure (that may perturb the low-cost measure)
  }

  if(SampleNum_per_level%LC_PER_LEVEL==0||SampleNum_per_level==0){ //first tick or every xx ticks)
    lowcostmeasure(); 
  }
  
  if(SampleNum_per_level%DS_PER_LEVEL==0||SampleNum_per_level==0){ //first tick or every xx ticks)
    TempDS_TMP=meastempDS();  
  }
   
  if(SampleNum_per_level%DHT_PER_LEVEL==0||SampleNum_per_level==0){ //first tick or every xx ticks)
   DHTHumidity_TMP=dht.readHumidity();
   DHTTemp_TMP=dht.readTemperature();
   delay(500); //wait one second or less if the reference sensor has been used in order to avoid any perturbation (voltage) to the low-cost sensor (it seems that there is a perturbation!!!)
  }
  
  ///////////////////
  // Print measures
  ///////////////////
  Serial.print(String(SampleNum)+",");
  if(goingUpsaid==false){Serial.print(F("Down,"));}     //To tell that the level is going DOWN
  else {Serial.print(F("_Up_,"));}                      //To tell that the level is going UP
  Serial.print(String(LevelNum)+",");
  Serial.print(String(SampleNum_per_level)+",");
  Serial.print(REFsensor+",");
  Serial.print(String(LevelCheck,3)+",");
  Serial.print(String(Offset,3)+",");
  Serial.print(String(Watertemp,2)+",");
  Serial.print(String(TempDS_TMP,2)+",");
  Serial.print(String(DHTTemp_TMP,2)+",");
  Serial.print(String(DHTHumidity_TMP,2)+",");
  Serial.print(LCsensor_TMP+",");
  Serial.print(LCsensor+LCsensorbis+",");
  Serial.print(Arduinocodeversion+",");
  Serial.println(LCsensor); //==> temporary change oto add "inside_tube"
  
  ///////////////////////////////
  // Update sample / lev & sample
  ///////////////////////////////
   SampleNum_per_level++;
   SampleNum++;
   
  /////////////////////////
  // Inform if going up now
  /////////////////////////
  if (SampleNum_per_level%NB_TICK_PER_LEVEL==0&&goingUp&&!goingUpsaid) { //this condition delays the println of going up after the next set of measures (to write at the good time "Going up")
    Serial.println(F("GOING_UP")); //Used to tell processing that switch has been made
    goingUpsaid=true;
  }  
  
  //////////////////////////////////////
  // Check for the end of the experiment
  /////////////////////////////////////
  if(goingUp&&LevelCheck>UPPER_LIM&&SampleNum_per_level%NB_TICK_PER_LEVEL==0){ //Checks for end of test (go up, reach the limit, do all measures at the step) & sends end code to processing
    digitalWrite(PumpStop, LOW);                    //Stops the pump
    Serial.println(F("Experiment_DONE"));           //enters infinite loop (Arduino dont like to exit)
    while(true){
      delay(1000);
    }
  }
  
  /////////////////////////////
  // Check & change water level
  /////////////////////////////
  if(SampleNum_per_level%NB_TICK_PER_LEVEL==0){       //If statement to change water level
    measREFsensor();                                  //Updates LevelCheck
    float levelbeforepumping=LevelCheck;              //save the level before running the pump
    if(abs(leakCheck_TMP-LevelCheck)>0.002){          //Checks for change in water level since starting on this level
      Serial.println(F("Problem_LEAK"));              //Tells processing that there is a leak, saves and exits
      digitalWrite(PumpStop, LOW);                    //Stops the pump
      while(true){
        delay(1000);
      } 
    }
    LevelNum++;                                       //Increase level count
    SampleNum_per_level=0;                            //Reset sample per level count
    if(goingUp==false){ pump("EMPTY",TIME_PUMP);}     //Pump out water if on downwards cycle
    else if(goingUp==true){ pump("FILL",TIME_PUMP); } //Pump in water if on upwards cycle
    delay(STABILIZE_TIME);                            //Stabilize water level
  
    measREFsensor();
    if(LevelCheck<LOWER_LIM){                         //switches outflow to inflow when reaches the bottom
      goingUp=true;
    }
    leakCheck_TMP=LevelCheck;                         //Save the level for the next leak test
    
    if(abs(levelbeforepumping-LevelCheck)<0.005){     //Checks for change in water level since the pump run
      Serial.println(F("Problem_PUMP"));              //Tells processing that there is a problem with the pump (level not changing or less than 5mm), saves and exits
      digitalWrite(PumpStop, LOW);                    //Stops the pump
      while(true){
        delay(1000);
      } 
    }    
  }
  delay(500); //to avoid some loss of data during the serial communication???
}
