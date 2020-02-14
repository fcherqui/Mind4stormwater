//libraries and library instance (objects), already in libs.h
//#include <SD.h>                 //to manage files on SD Card
//File myFile;                        //specific class to use files

/*************************************** 
 * --------- SD CARD FUNCTIONS --------- 
****************************************/
void SDinit() { //start the SD card
  lastaction="SDinit";
  Serial.print("Initializing SD card...");
  if (SD.begin(SD_CHIP_SELECT)) { Serial.println("Success!");}
  else {
    Serial.println("Card failed, or not present, or?");
    OLEDlms("SD Card   failed or not in?");
    while (!SD.begin(SD_CHIP_SELECT)) {
      MyWatchDoggy.clear(); // Reset watchdog with every loop to make sure the sketch keeps running.
      for (int i=0;i<50;i++) {
        blinker(100);
      }
    }
  }
  display.clearDisplay();
  display.display();  
}

String readFile(String nameFile) { //read the content of a file from the SD card
//  String msg="'--Reading file " + nameFile +".txt: ";
  String result;
  nameFile = nameFile + ".txt";
  if (SD.exists(nameFile)) {
//    msg=msg+"file exists! ";
    myFile = SD.open(nameFile);
    result = "";
    while (myFile.available()) {
      char c = myFile.read();
      if ((c != '\n') && (c != '\r')) { result += c; delay(5);}
    }
  }
  else {
    result="0"; //default value to the parameter if there is no value in the SD card
//    msg=msg+"no file? now good! ";
    myFile = SD.open(nameFile, FILE_WRITE);
    if (myFile) {
      if (nameFile=="batco.txt") {result=default_batco;}    //defaut oefficient to calculate the battery voltage
      if (nameFile=="coeffb.txt") {result=default_coeffb;}  //coefficient B by defaut
      if (nameFile=="coeffc.txt") {result=default_coeffc;}  //coefficient C by defaut
      if (nameFile=="myphone.txt") {result=default_myphone;}  //board phone number by defaut
      if (nameFile=="period.txt") {result=default_period;}  //defaut time between measurements
      if (nameFile=="sitenam.txt") {result=default_sitenam;}    //defaut oefficient to calculate the battery voltage      
      if (nameFile=="timeot.txt") {result=default_timeot;}  //defaut timeout time for the gsm connection
      if (nameFile=="vpulse.txt") {result=default_vpulse;}  //defaut cumulative volume needed to start the sampling
      if (nameFile=="wband.txt")  {result=default_wband;}   //defaut height diff to stop the sampling
      if (nameFile=="wpulse.txt") {result=default_wpulse;}  //defaut water level needed to start the sampling
      myFile.println(result);
      myFile.close();
    }
  }
//  loggerln(msg);
  return result;
}

void SaveData() { //save all data on the SD card
  lastaction="SaveData";
  String msg="Saving data, ";
  String csvObject = "";
  if (connected) {sigstrength = scanner.getSignalStrength();} //GSM signal strengh (max seems to be 31 ?)
  if (!SD.exists(logFile)) {
    csvObject = "Site_name,DateTime [DDMMYY HH:MM:SS],Timestamp[s],Signal_strength[/31],Measure_number[-],Battery_level[V],Head_level[m],Water_level[m],Water_temp[C],Flow [m3/s], Vol_cumul_event[m3],Vol_cumul_total[m3],Weir_under_water [1=yes/0=no]\n";
  }
  myFile = SD.open(logFile, FILE_WRITE);
  csvObject = csvObject + sitenam + "," + nowthatIcanread() + "," + stamp + "," + sigstrength + "," + String(counter) + "," + String(batlevel) + "," + String(hlev, 3) + "," + String(wlev, 3) + "," + String(otttemp) + "," + String(flow, 4) + "," + String(vevent,4) + "," + String(vcumul,4) + "," + underwater;
  if (myFile) {
    myFile.println(csvObject);
    msg=msg+"done!";
    myFile.close();
  }
  else { msg=msg+"error opening file."; }
  loggerln(msg);
}

void ChangeParameter (String nameparameter , String value) { //all values should be given as string
  String msg="'--, Updating parameter,  '"+nameparameter+"' = "+value;
  String name = nameparameter + ".txt";
  if (SD.exists(name)) {
    SD.remove(name);
  }
  myFile = SD.open(name, FILE_WRITE);
  //msg=msg+", success = "+String(myFile);
  if (myFile) {
    myFile.println(value);
    msg=msg+", value saved: "+value;
    myFile.close();
  }
  //loggerln(msg);
}
