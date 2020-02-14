/*
MKR-OTT project
----------
 * v8: improve connection stability (avoid idle when trying to connect) + sampling mode desactivate synchro time + allow data to GS + desactivate sleep
 * v9: log all actions on serial AND on a logfile!!
 * v10: Improve sampling configuation + huge improvement in connection stability (no more watchdog shutdown?) + sms option to establish gsm connexion + sms option to stop logs on SD card + log file with date
 * v11: Add Low Power library (https://www.arduino.cc/en/Reference/ArduinoLowPower) to compare consumption with RTCZero library (https://github.com/arduino-libraries/RTCZero)
 * v12: huge simplification of the code, time sync during sampling, sleep with epoch time, lowpower mode removed...
 * v13: 2nd switch + power switch + OLED screen + MAJOR modification of the code (log on OLED, sms management, management of one or two switchs, update function list, etc...
 * v14: one code to rule them all: site name and phone within the SD card ==> same code for all sites!!
 * v15: code a little rewritten to separate functions by sheet (incl. libraries) + possible to change by sms sitename, phone number and coeffb + possible to ask parameter value by sms, soft reset when connection fails several times
 * 
----------
Functions:
.  BatMeasure();        //measure the level of the battery
.  blinker();            //blink LED for the choosen amount of seconds seconds
.  ChangeParameter();   //change the value of a parameter on the file on the SD card (called by SaveParameters)
.  ConnecToTheWorld();  //connect to the network
.  diplayparam();       //display all important parameters on the OLED screen
.  IdleMode();          //disconnect from the network to wait for the next measure
.  InitParameters();    //initialise all the parameters based on the SD card content (counter and other parameters)
.  InitPins();          //Init all necessary pins
.  ISR();               //interrupt service routine (ISR), called when interrupt is triggered  executes after MCU wakes up (needed but can be empty)
.  loggerln();          //save messagge on SD and show on Serial if activated (ln because use return like printLN)
.  Measures();          //fill in all parameters, i.e. measures + stamp + battery level + signal strength
.  Myshutdown();        //command executed before reset from the watchdog
.  nowthatIcanread()    //write a nice that easy to read
.  OLEDallInOne();      //display all the message on the OLED screen (small size)
.  OLEDoff();           //clear the display and stop any other display
.  OLEDlms();           //show a short message on the OLED screen until a new message erase it.
.  OLEDsetup();         //initialize the OLED screen, once per startup
.  OLEDsms();           //show a short message on the OLED screen for a short time (5 seconds)
.  OLEDscroll();        //scroll the message with a big font but slowly...
.  OttMeasure();        //start the measurement with the OTT probe
.  print2digits();      //print a 0 before if the number is < than 10 (for date, month...)
.  PublishData();       //prepare the data to be published online
.  PubToGS();           //send message to google spreadsheet
.  readFile();          //read the content of a file on the SD card and send back the content as a string
.  Readsms();           //read any incoming sms
.  readsensor();        //read string from the probe
.  Sampling();          //start the sampling...
.  SaveData();          //save the data on the SD card
.  SaveParameters();    //save the parameters on the SD Card
.  scrollparam();       //scroll all important parameters on the OLED screen
.  SDinit();            //initialize the SD card
.  SendSMS ();          //send an sms to a friend...
.  showthedata();       //show the measures on the OLED screen (from main programme if oled=1 or oled=2)
.  Sleep_Init();         //Init sleepmode
.  SleepNow();          //start and end of the sleeping period
.  Sleep_Time();        //set new alarm
.  StartSampling();     //Start the sampler!
.  SynchroTime();       //get the time from the network
.  switch1();           //test if switch 1 is underwater
.  switch2();           //test if switch 2 is underwater
.  TestSampling();      //test if the sampling is needed
.  UltraSleepMode();    //call SleepNow() function
.  Underwatertest();    //test if the weir is underwater (1) or above water (0), 2=unclear when the two switch do not agree
.  VolumeFlow();        //calcul the cumulated volume 
.  Watchdogstart();     //start the watchdog
.  Weekday();           //Calculate day of week using the epoch time, easier than Gaussian algorithm ;-)
.  whosnumber()         //give name associate to phone number
*/
