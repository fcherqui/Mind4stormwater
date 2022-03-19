/* All about the control of the pump
 *  
 *  Relay shield: HiLetgo 5V 4 Channel Relay Shield for Arduino UNO R3
 *  - relay 3 (NO) or pin 5 safety to power or not the pump (need to be high to power the pump)
 *  - Relay 1 is controlled by digital pin 7
 *  - Relay 2 is controlled by digital pin 6
 *  - Relay 3 is controlled by digital pin 5
 *  - (Relay 4 is controlled by digital pin 4)
 */

//////////////////////////////////////////////
///         PUMP INITIALISATION            ///
//////////////////////////////////////////////
void InitPUMP() {
  pinMode(PumpIn, OUTPUT);
  pinMode(PumpOut,OUTPUT);
  pinMode(PumpStop, OUTPUT);
  digitalWrite(PumpIn, HIGH);   // PumpIn LOW to fill the column and HIGH to empty it
  digitalWrite(PumpOut, LOW);   // PumpOut LOW to fill the column and HIGH to empty it
  digitalWrite(PumpStop, HIGH); // PumpStop HIGH to start the pump
}

//////////////////////////////////////////////
///            PUMP CONTROL                ///
//////////////////////////////////////////////
void pump(String Flow, int Time){                                         //Flow= FILL --> to fill the column | EMPTY --> to empty the column
  measREFsensor();                                                        //Run this to grab a updated LevelCheck value
  if((LevelCheck>1.9&&Flow=="FILL")||(LevelCheck<0&&Flow=="EMPTY")){      //This is a secondary safety check, system should already be protected by UPPER_LIM and LOWER_LIM
    digitalWrite(PumpStop, LOW);                                          //Stops the pump
    Serial.println(F("Something_wrong"));
    while(1){ delay(1000); }
  }

  if((Flow=="FILL"&&LevelCheck<1.9)||(LevelCheck<0)){
    digitalWrite(PumpIn, LOW);
    digitalWrite(PumpOut, LOW);
    delay(Time);
    digitalWrite(PumpIn, HIGH);
    digitalWrite(PumpOut,LOW);
  } else if((Flow=="EMPTY"&&LevelCheck>0)||LevelCheck>1.9){
    digitalWrite(PumpIn, HIGH);
    digitalWrite(PumpOut, HIGH);
    delay(Time);
    digitalWrite(PumpIn, HIGH);
    digitalWrite(PumpOut,LOW);
  }
}

//////////////////////////////////////////////
///           INITIAL FILL                 ///
//////////////////////////////////////////////
void initialFillFunc(){             //Fill up the pipe before beginning
  measREFsensor();                  //Update LevelCheck
  while(LevelCheck<UPPER_LIM-0.05){  //If not already full, will fill
    digitalWrite(PumpIn, LOW);      //Start pumping up
    digitalWrite(PumpOut, LOW);     //Start pumping up
    delay(4000);
    measREFsensor();                //Update LevelCheck
    Serial.print(F("'**** (Fast mode)Water level was approx. "));
    Serial.print(String(LevelCheck,3));
    Serial.print(F(" m & objective= "));
    Serial.print(String(UPPER_LIM-0.05,3));
    Serial.println(F(" m"));
  }
  digitalWrite(PumpIn, HIGH);       //stop the pump
  digitalWrite(PumpOut,LOW);        //stop the pump
  delay(3000);                      //time to start to be a little precise!
  measREFsensor();                  //Update LevelCheck
  while(LevelCheck<UPPER_LIM){      //If not already full, will fill
      Serial.print(F("'**** (Slow mode)Water level was approx. "));
      Serial.print(String(LevelCheck,3));
      Serial.println(F(" m & pumping water for 5 seconds"));
      pump("FILL",5000);
      measREFsensor();                //Update LevelCheck
  }    
  delay(STABILIZE_TIME);          //Stabilize water level
  measREFsensor();                //Update LevelCheck
  Serial.print(F("'**** Level is now "));
  Serial.print(String(LevelCheck,3));
  Serial.println(F(" m."));
}
