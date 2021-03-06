#include <SoftwareSerial.h>
#include "StringModule.h"

/* Define the global variables */
StringModule Storage=StringModule("");  /* Store the important information from ESP reponse
                                           when launch every AT command
                                        */
StringModule Analysis=StringModule(""); //use this objects to analysis data received from client.
String dataToBeSent = "welcome"; 
int chal=0;//default channal for server to send data

void setup()
{
  Serial.begin(9600);    // Arduino serial Monitor
  while (!Serial) {
     // wait for serial port to connect. Needed for Leonardo only
  }
  Serial1.begin(9600);   // The ESP8826 has a baud rate of 9600
//enable esp8266
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  ESPWIFIsetup(); /* set up the configuration of ESP chip */
  //ESPWIFIassociate();
  ESPsetServer(); //set up a server, port 80
}

void loop() {
   //receiving module, this module will receive any information from client
    String rx="";
    while (Serial1.available() > 0)  {
        rx += char(Serial1.read());
        delay(2);
     }

    if(rx.length() > 0) {
       Serial.println(rx);
       
       rx = "";
    }
    
    //sending module, this module will send anything read from serial monitor.
    //send through channal 0 by default.
    
}

/*
String tx="";
    
    while (Serial.available()){
      tx+=char(Serial.read());
      delay(2);
    }
    if (tx.length()>0){
        ESPUpload(chal,tx);
        tx="";
    }
   */
