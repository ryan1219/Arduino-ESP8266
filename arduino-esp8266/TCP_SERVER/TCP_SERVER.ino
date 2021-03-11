#include <SoftwareSerial.h>
#include "StringModule.h"

String Storage;  /* Store the important information from ESP reponse */
String Command;  /* Keep track of the command */
int    Decision; /* Decide which kind of behavior of ESP should have */
int   Count;

int dummyData = 40;
const String  API_KEY = "GPVP0E6QQVWU47LZ";
StringModule STR("");

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
  
  /* Define the global variables */
  Storage = "";
  Command = "";
  Decision = -1;
  Count=0;
  
  ESPWIFIsetup(); /* set up the configuration of ESP chip */
  //ESPWIFIassociate();
  //ESPTCPconnection();
  ESPsetServer();
}

void loop() {
    char character;
    if (Serial.available())
    {
      character = Serial.read();
      Command += character;
    }
    else if(Command.length())
    {
      Serial.println(Command);
      if(Command.equals("FINISH"))
      {
         ESPFinish(); /* Break the TCP and association*/
      }else if(Command.equals("UPLOAD"))
      {
         Decision = 0;
      }else if(Command.equals("DOWNLOAD"))
      {
         Decision = 1;
      }else if(Command.equals("PAUSE"))
      {
         Decision = -1;
      }else{
         Serial.println("[ERROR0] : no matching command!!");
      }
      Command =""; /* Clean the memory of the command */
    }
    else
    {
      switch(Decision)
      {
       case 0:
         if(Count==0)
         {
           dummyData +=5;
           ESPUpload(dummyData);
         }
         else
           Count = (Count+1)%60;
         delay(1000);
         break;
       case 1:
         Decision = -1;
         ESPDownload();
         break;
       default:
         delay(1000);
         break;
      } 
    }
}



