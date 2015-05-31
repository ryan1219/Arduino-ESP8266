/*********************************************************/
#include <SoftwareSerial.h>
#include "StringModule.h"
/*********************************************************/
//
#define  SERIAL1_BAUD_RATE             57600

#define  WIFI_AP_SSID                 "ESP8266SERVER_YAN"
#define  WIFI_AP_PASSW                "12345"
#define  AP_IP                        "192.168.6.1"
/*********************************************************/
/* Define the global variables */
StringModule storage=StringModule("");  /* Store the important information from ESP reponse
                                           when launch every AT command
                                        */
StringModule analysis=StringModule(""); //use this objects to analysis data received from client.
long int count=0;                   //count how many massage receive    
String dataToBeSent = "welcome"; 
unsigned long time;                //measure the time for sending data
/*********************************************************/
void setup()
{
  // Arduino serial Monitor
  Serial.begin(115200);    
  
  //
  while (!Serial)
  {
    // wait for serial port to connect. Needed for Leonardo only
  }
  
  //ESP8826 has a baud rate of SERIAL1_BAUD_RATE
  Serial1.begin(SERIAL1_BAUD_RATE);
  
  //enable esp8266
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  //set up the configuration of ESP chip
  ESP_setup(WIFI_AP_SSID, WIFI_AP_PASSW, AP_IP);
  
  //set up the TCP server, port 80
  ESP_set_server("80");
}



/*********************************************************/
void loop() 
{
  //receiving module, this module will receive any information from client
  String    rx  =  "";
  
  while (Serial1.available() > 0)  
  {
    rx += char(Serial1.read());
    //delay(2);
  }//while
  
  //if rx catch anything from buffer, save it to "analysis" 
  if(rx.length() > 0) 
  {
    Serial.print(rx);
    
    //store "rx" into "analysis" for checking
    analysis.setString(analysis.getString()+rx);
    //clear rx
    rx = "";
    
    //if 1 TCP message is rx-ed from ESP
    if(analysis.Contains("+IPD,0"))
    {
      //number of rx-ed TCP messages
      count++;
      //clear status 
      analysis.ClearString();
    }
    
    if(count==1)
    {
      time=millis();
    }
    
    //if TCP client requests to close the TCP connection
    if(analysis.Contains("CLOSED"))
    {
      analysis.ClearString();
      time=millis()-time;
      Serial.print("total number of messages received: ");
      Serial.println(count);
      Serial.print("time for sending data (millis): ");
      Serial.println(time);
      //clear counter
      count=0;     
    }//if
    
  }//if rx catch anything from buffer, save it to "analysis"
  
}//void loop()     
    
    
    
    
    
    
    
    
    
    
    
    /*
    String rx="";
    while (Serial1.available() > 0)  {
        rx += char(Serial1.read());
        delay(2);
     }

    if(rx.length() > 0) {
       analysis.setString(analysis.getString()+rx);
       if(analysis.Contains("OK")==true){
            analysis.ClearString();
            count++;
            Serial.println(rx);
       }
       rx = "";
    }
    Serial.println(count);
    */
    //sending module, this module will send anything read from serial monitor.
    //send through channal 0 by default.
    


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
