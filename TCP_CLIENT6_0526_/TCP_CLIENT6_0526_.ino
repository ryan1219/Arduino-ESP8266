/*********************************************************/
#include <SoftwareSerial.h>
#include "StringModule.h"


/*********************************************************/
//
#define  MESSAGE_NUM        100

//
#define  SERIAL1_BAUD_RATE            9600
//
#define  SERIAL1_BAUD_RATE_SCALE      2

/*********************************************************/
/* Define the global variables */
StringModule Storage = StringModule("");  /* Store the important information from ESP reponse
                                             during setup.
                                           */
StringModule Analysis=StringModule(""); //use this objects to analysis data received from client.            
String dataToBeSent = "welcome"; 
StringModule STR = StringModule("");

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
  ESP_setup();
  
  //join the AP "ESP8266SERVER", password "12345"
  ESP_join_AP("ESP8266SERVER","12345");
  
  //Open TCP connection to "192.168.6.1" at port 80
  ESP_TCP_connection("192.168.6.1","80");
}

/*********************************************************/
void loop() 
{
  int    i;
  
  for(i = 0; i < MESSAGE_NUM; i++)
  {
    //send the TCP message to the server
    ESP_send_message(dataToBeSent);
   }
   
   //close TCP connection to the TCP server
        
   delay(100*1000);
}

/*********************************************************/

//receiving module, this module will receive any information from client
/*    String rx="";
    while (Serial1.available() > 0)  {
        rx += char(Serial1.read());
        delay(2);
     }

    if(rx.length() > 0) {
       Serial.println(rx);
       rx = "";
    }
//sending module, this module will send anything read from serial monitor
/*String tx="";
    while (Serial.available()){
      tx+=char(Serial.read());
      delay(2);
    }
    if (tx.length()>0){
        ESPUpload(tx);
        tx="";
    }
 */   
