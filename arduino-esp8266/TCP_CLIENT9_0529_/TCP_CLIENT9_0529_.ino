/*********************************************************/
#include <SoftwareSerial.h>
#include "StringModule.h"


/**********************************************************/
//
#define  SERIAL1_BAUD_RATE                  57600
//
#define  SERIAL1_BAUD_RATE_SCALE            10
//
#define  SUSTAINABLE_SERIAL1_BAUD_RATE      (SERIAL1_BAUD_RATE/SERIAL1_BAUD_RATE_SCALE)      
//
/*********************************************************/
/* Define the global variables */
StringModule Storage = StringModule("");  /* Store the important information from ESP reponse during setup. 
                                             This variable is only used in CommLaunch function when keyword =1 */
//long from -2,147,483,648 to  2,147,483,647                                                       
long int MESSAGE_NUM = 1;

String dataToBeSent = "welcome"; 

//store current sending message, if sending fails, after fixing the connection problems, using this storage to resend. 
String current_message="";
String TCP_IP="192.168.6.1";
String TCP_port="80";
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
  ESP_join_AP("ESP8266SERVER_YAN","12345");
  
  //Open TCP connection to "192.168.6.1" at port 80
  ESP_TCP_connection(TCP_IP,TCP_port);
}

/*********************************************************/
void loop() 
{
  
  
  long int    i;
  
  for(i = MESSAGE_NUM; i < MESSAGE_NUM+MESSAGE_NUM; i++)
  {
    //send the TCP message to the server
    ESP_send_message(dataToBeSent+(String)i);
   }
   
//close TCP connection to the TCP server
  ESP_finish();
  
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
