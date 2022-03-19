 /* PROGRAM FOR THE TESTING BENCH (to test low cost sensor for water level monitoring) using an Arduino UNO
 * Mind4Stormwater project: http://mind4stormwater.org, , feel free to use, reuse, modify, share!!
 * University of Melbourne / INSA Lyon
 * Frédéric Cherqui - fcherqui@gmail.com
 Thanks to Theo Perigaud and Kai James
 * ----- last version June 20 ----
 * 
 * This code connect with the Arduino UNO board through the serial * 
 * Added: possibility to communicate back through serial to input a value to the board
 * 
 //==> all comments to check!!
*/ 
String ProcessingCodeVersion="v03_01";
String filepath="C:\\Users\\nerdguy\\Dropbox\\LOW_COST_SENSORS_Testing-Bench\\91_Live-tests-results\\"; //where to save the results, default: "C:\\Users\\nerdguy\\Dropbox\\LOW_COST_SENSORS_Testing-Bench\\91_Live-tests-results\\"

//CONFIG//////////
String comPort="COM8"; //Change for arduino com port (From arduino IDE or device manager) - default "COM3" for the Sony computer and "COM8" for the EliteBook
import processing.serial.*;
import javax.swing.JOptionPane; //use for dialog box
Serial myPort;
Table dataTable = new Table();
String fileName = "Test-results_"+str(year())+"-"+str(month())+"-"+str(day())+"_"+str(hour())+"h"+str(minute())+"mn_";
String fileNameTable;
String fileNameLog=filepath+fileName+".txt";
PrintWriter output;
String phase="NoTable"; //phase of processing: "NoTable" when the serial data is only diplayed, "TableHeader" to save the headers in the table (only once), "TableData" to save data in the table
int num_column;//number of column of the table
String[] headers;

//////////////////
/// SETUP FUNCTION
//////////////////
void setup(){
  output = createWriter (fileNameLog); //create the file to save the log
  output.println("LOG FILE FOR THE TESTING BENCH"); //write in the output (need to be flush to the file at the end)
  output.println("Date= "+str(year())+"/"+str(month())+"/"+str(day()));
  output.println("Time= "+str(hour())+":"+str(minute()));

  loggerln("'*******************************************************************************");
  loggerln("'**                  PROCESSING FOR THE TESTING BENCH                         **");
  loggerln("'**                              "+ProcessingCodeVersion+"                                       **");
  loggerln("'*******************************************************************************");
  loggerln(" ");
  loggerln(" Press x to leave at any time");
  loggerln(" ");
  loggerln(" Starting serial communication, wait 30 to 60 seconds please...");
  myPort = new Serial(this, comPort, 9600);
}

//////////////////
/// SERIAL EVENT 
//////////////////
//void serialEvent(Serial myPort){
//}

/// "log" on screen & file 
//////////////////////////
void logger(String whattolog) {
  String message="["+str(year())+"/"+str(month())+"/"+str(day())+" "+str(hour())+":"+str(minute())+"] "+whattolog;
  output.print(message);
  print(message);
}

/// "log" on screen & file (line return) 
////////////////////////////////////////
void loggerln(String whattolog) {
  String message="["+str(year())+"/"+str(month())+"/"+str(day())+" "+str(hour())+":"+str(minute())+"] "+whattolog;
  output.println(message);
  println(message);
}

/// user end the program 
//////////////////////////
void keyPressed(){
  if(key=='x'){
//    key='0'; //==> useful?
    loggerln("Force exit on key pressed, ignore last data line. PLEASE WAIT FOR DATA SAVE");
//    phase="Exit";
    CloseAll();
  }
}

/// always on function (may not be needed)
//////////////////////////
void draw(){ //draw function is executed every frame of the program 
  if ( myPort.available() > 0) {
    String val=myPort.readStringUntil('\n');
      /// unexpected problem 
      //////////////////////
    if (val!=null) { //CONDITION message not null
      /// Manual reading 
      //////////////////////
      if (val.contains("Manual_reading_in_mm_?")){ // specific part to manage the manual readings (input to the Arduino board through the serial
        int manualread = 0;
        String r = JOptionPane.showInputDialog(null, "What number?", "Decide", JOptionPane.QUESTION_MESSAGE);
        try {
          manualread = Integer.parseInt(r);
        } catch(NumberFormatException e) {
          r = JOptionPane.showInputDialog(null, "LAST TRIAL What number?", "digit only", JOptionPane.QUESTION_MESSAGE);
        }
        try {
          manualread = Integer.parseInt(r);
        } catch(NumberFormatException e) {
          r = JOptionPane.showInputDialog(null, "REALLY LAST TRIAL What number?", "digit only", JOptionPane.QUESTION_MESSAGE);
        }
        try {
          manualread = Integer.parseInt(r);
        } catch(NumberFormatException e) {
          manualread=0000;
        }
        loggerln("Manual reading entered: "+manualread+" mm");
        myPort.write(manualread+"\n"); //send the value back to the board through serial
      }
      /// unexpected error 
      //////////////////////
      else if (val.contains("Something_wrong")){
        logger(val);
        phase="Exit";
        loggerln(" Forced exit but PROBLEM unknow. PLEASE WAIT FOR DATA SAVE");
      }
      /// leak problem 
      //////////////////////
      else if (val.contains("Problem_LEAK")){
        logger(val);
        phase="Exit";
        loggerln(" Forced exit because there is a leak PROBLEM! PLEASE WAIT FOR DATA SAVE");
      }
      /// pump problem 
      //////////////////////
      else if (val.contains("Problem_PUMP")){
        logger(val);
        phase="Exit";
        loggerln(" Forced exit because there is a PROBLEM with the pump! PLEASE WAIT FOR DATA SAVE");
      }
      /// end of experiment 
      //////////////////////
      else if (val.contains("Experiment_DONE")){
        logger(val);
        loggerln(" Normal exit, experiment finished. All good! PLEASE WAIT FOR DATA SAVE");
        phase="Exit";
      }
      /// change of punping direction 
      ///////////////////////////////
      else if (val.contains("GOING_UP")){
        //logger(val);
        loggerln("=================================================================================");
        loggerln("============================= INVERSING PUMP TO GO UP ===========================");
        loggerln("=================================================================================");
      }
      /// Fetch sensor name 
      ///////////////////////////////
      else if (val.contains("Low_cost_sensor_to_test")){
        logger(val);
        String Temparray[] = split(val, ':'); //places strings in array.
        String sensorname=Temparray[1]; //second value in the table (message is something like: "'** Low_cost_sensor_to_test: JSN-SRT04", so only "JSN-SRT04" will be kept
        loggerln("==> Sensor name is:"+sensorname);
        fileName = fileName+sensorname;
        loggerln("==> New file name is:"+fileName);
      }
      /// start saving data 
      //////////////////////
      else if (phase=="NoTable"){ //CONDITION message contains "GOprocessing" ==> all the next data will go in the table
        logger(val);
        if (val.contains("GOprocessing")){
          phase="TableHeader";
          loggerln(" Start to save the data in the table: header first");
        }
      }
      /// header creation 
      //////////////////////
      else if (phase=="TableHeader"){ //CONDITION all the next data will go in the table and it is the first time
        logger(val);
        val=trim(val);
        String Vals[] = split(val, ','); //places values in array. Cant process this as a double in processing
        num_column=Vals.length;
        loggerln("Number of headers: "+str(num_column));
        headers=new String[num_column+5];
        loggerln(" Creating the headers of the data table");   
        dataTable.addColumn("year");
        dataTable.addColumn("month");
        dataTable.addColumn("day");
        dataTable.addColumn("hour");
        dataTable.addColumn("minute");
        dataTable.addColumn("second");
        for (int i=0; i < num_column; i++) {
          headers[i] = Vals[i]; //places values in array
          loggerln(" New header: "+headers[i]);
          dataTable.addColumn(headers[i]);
        }
        dataTable.addColumn("Processing_code");
        phase="TableData";
        loggerln("Start to save the data in the table: all the data!");
        loggerln(val);
      }
      /// save measurements
      //////////////////////
      else if (phase=="TableData"){ //CONDITION all the next data will go in the table and it is NOT the first time
        print("["+str(year())+"/"+str(month())+"/"+str(day())+" "+str(hour())+":"+str(minute())+"] "+val);
        val=trim(val);
        String sensorVals[] = split(val, ','); //places values in array. Cant process this as a double in processing, use this if it's only float: float sensorVals[] = float(split(val, ','));
        TableRow newRow = dataTable.addRow(); //add row for new reading
        newRow.setInt("year", year());
        newRow.setInt("month", month());
        newRow.setInt("day", day());
        newRow.setInt("hour", hour());
        newRow.setInt("minute", minute());
        newRow.setInt("second", second());
        for (int i=0; i < num_column; i++) { //all float numbers except the last one (arduino code version)
          newRow.setString(headers[i], sensorVals[i]);
        }
        newRow.setString("Processing_code", ProcessingCodeVersion);
      }
    }
  }
  if(phase=="Exit"){ CloseAll();}      //end all and exit
}

/// end of experiment, write files 
//////////////////////////////////
void CloseAll() {
  loggerln("'*******************************************************************************");
  loggerln("'**                  IT IS TIME TO END THE EXPERIMENT                         **");
  loggerln("'*******************************************************************************");
  fileNameTable=filepath+fileName+".csv";
  saveTable(dataTable, fileNameTable);
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  println("Data saved in "+filepath+fileName+", you can close the programm");
  exit(); // Stops the program
}
