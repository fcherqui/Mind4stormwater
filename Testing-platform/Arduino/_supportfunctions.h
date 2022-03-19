/*
 * SUPPORT FUNCTIONS
 * 
 * initall() to initialise the system, do initial checks and then trigger processing save
 */
void initall() {
  Serial.begin(9600);
  delay(15000); // wait if a new code needs to be uploaded to the board (default 30000)
  while(!Serial);
  delay(1000); // allow time to start processing
  Serial.println(F(""));
  Serial.println(F("'*******************************************************************************")); //Serial.println(F("...")) to put the constant char arrays in progmem/flash and save global variable memory, see https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
  Serial.println(F("'**                                                                           **"));
  Serial.println(F("'**                  TESTING BENCH HAS STARTED                                **"));
  Serial.print(F("'**                           "));
  Serial.print(Arduinocodeversion);
  Serial.println(F("                                          **"));
  Serial.println(F("'*******************************************************************************"));
  Serial.println(F("'**EXPERIMENT PARAMETERS"));
  Serial.println("'** Low_cost_sensor_to_test:"+LCsensor+":"); //==> temporary change oto add "inside_tube"
//  if (LCsensor=="VL53l0x") {
//    Serial.print(F("'**** VL53l0x Long Range: "));
//    Serial.println(LONG_RANGE);
//    Serial.print(F("'**** VL53l0x High Accuracy: "));
//    Serial.println(HIGH_ACCURACY);
//    Serial.print(F("'**** VL53l0x High Speed: "));
//    Serial.println(HIGH_SPEED);
//  }
  Serial.print(F("'** Reference sensor: "));
  Serial.println(REFsensor);
  Serial.print(F("'** Stabilisation time for each level: "));
  Serial.print(String(STABILIZE_TIME/1000));
  Serial.print(F(" seconds\n'** Pump working time between each level : "));
  Serial.print(String(TIME_PUMP/1000));
  Serial.println(F(" seconds"));
  LOWER_LIM = max(0,LOWER_LIM);             //safety on the min lower limit
  UPPER_LIM = min(1.90, UPPER_LIM);         //safety on the max upper limit
  Serial.print(F("'** UPPER LIMIT="));
  Serial.print(String(UPPER_LIM));
  Serial.println(F(" m (max=1.9 m)"));
  Serial.print(F("'** LOWER LIMIT="));
  Serial.print(String(LOWER_LIM));
  Serial.println(F(" m (min=0.0 m)"));
  Serial.println("'** "+String(NB_TICK_PER_LEVEL)+ " ticks/level");
  Serial.println("'** "+String(NB_REF_PER_LEVEL)+ " reference measures/level ==> A reference measure every "+String(REF_PER_LEVEL)+" ticks");
  Serial.println("'** "+String(NB_LC_PER_LEVEL)+ " low-cost measures/level ==> A reference measure every "+String(LC_PER_LEVEL)+" ticks");
  Serial.println("'** "+String(NB_DS_PER_LEVEL)+ " Dallas (black) temperature measures/level ==> A reference measure every "+String(DS_PER_LEVEL)+" ticks");
  Serial.println("'** "+String(NB_DHT_PER_LEVEL)+ " DHT (white) temp/air relat humid measures/level ==> A reference measure every "+String(DHT_PER_LEVEL)+" ticks");
  Serial.println(F("'**"));
  Serial.println(F("'** Approximate duration of the experiment = "));
  Serial.print(F("'** "));
  long expduration=(2000*(UPPER_LIM-LOWER_LIM)/(5.1*TIME_PUMP/1000))*(15+255*(NB_TICK_PER_LEVEL/1000)+(TIME_PUMP+STABILIZE_TIME)/1000)/60+5; //PUMP=5.1mm/second, 15 sec measures for a new level, 255 measures /level (with 1000 measures); 2000 because way and back and in mm
  Serial.println(String(expduration/(24*60))+" days "+String((expduration%(24*60)/60))+ " hours "+String((expduration%(24*60)%60))+ " minutes              ("+String(expduration)+" mn in total)."); 
  Serial.println(F("'*******************************************************************************"));
  Serial.println(F("'**EXPERIMENT STARTED"));
  Serial.println(F("'*******************************************************************************"));

  Serial.println(F("'** Starting the reference sensor..."));
  StartRefSensor(); // start reference sensor (default reference sensor is PLS OTT)
  Serial.println(F("'** Starting the reference sensor...ok"));
  Serial.println(F("'** First measure:"));
  measREFsensor();
  Serial.println(F("**"));
  Serial.println(F("****************************************"));
  Serial.println("**           Level="+String(LevelCheck,3)+" m                     , Offset incl. ("+String(Offset,3)+" m), water temperature = "+Watertemp+" C");
  Serial.println(F("****************************************"));
  Serial.println(F("**"));
  Serial.println(F("'**"));
  Serial.println(F(" ==> You have five seconds to check that the level is correct... <=="));
  Serial.println(F("'**"));
  delay(5000);

  Serial.print(F("'** Initialisation of the pump..."));
  InitPUMP();
  Serial.println(F("ok!"));
  Serial.print(F("'** Starting the temperature sensors..."));
  dht.begin();      // start the DHT sensor
  int trials=0;
  TempDS_TMP=85;
  while (TempDS_TMP==85&&trials<50) {
    trials++;
    TempDS_TMP=meastempDS();
    delay(500);       
  }
  if (trials<50) {Serial.println(F("ok!"));}
  else {
    Serial.println(F("problem with the Dallas scientific temperature sensor (black), check connections"));
    while (true) {delay(1000);}
  }

  Serial.print(F("'** Starting the low cost sensors..."));
  lowcosinit();
  Serial.println(F("ok!"));

  Serial.println("'** Filling the column until "+String(UPPER_LIM,3)+" m...");
  initialFillFunc();
  Serial.println(F("'**** Column ready!"));
  delay(1000);
}
  
