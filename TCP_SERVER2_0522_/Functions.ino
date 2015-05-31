void ESPWIFIsetup()
{
  //CommLaunch("AT+CIOBAUD?\r", 2000,0);
  delay(2000);
  CommLaunch("AT\r", 1000,0);
  CommLaunch("AT+RST\r", 2000,0);
  CommLaunch("AT+CWMODE=3\r", 1000,0);
  CommLaunch("AT+CIPAP=\"192.168.5.1\"\r", 1000,0);
  CommLaunch("AT+CIPMUX=1\r", 1000,0);
  CommLaunch("AT+CIFSR\r", 1000,0);
  CommLaunch("AT+CWSAP=\"ESP8266SERVER\",\"12345\",1,0", 2000,0);
  /*
    CommLaunch("AT+CWJAP=\"BCRLovs\",\"23456\"\r", 10000,0);
    CommLaunch("AT+CIFSR\r", 2000,0);
   */
  //CommLaunch("AT+CWLAP\r", 6*1000, true, 1);
}
void CommLaunch(String cmd, int timeDelay, int keyword)
{
  Serial1.println(cmd);    //send command to ESP8266

  Serial.println("ESP8266 RESPONSE:");
  String tmp="";
  long deadline = millis()+timeDelay;
  //delay(timeDelay);
  while(millis() < deadline)
  {
  while (Serial1.available()>0)  {
      tmp += char(Serial1.read());
      delay(2);
      }
  if (tmp.length()>0){
         Serial.println(tmp);
         if(keyword==1){
                Storage.setString(Storage.getString()+tmp); // if the keyword is triggered, copy the string
               }
               tmp = "";
      }
  }
  Serial.println("\n------------------------------");
}
void ESPsetServer(){
          while(true){
	  CommLaunch("AT+CIPSERVER=1,80\r",2000, 1);
          //Serial.println(Storage.getString());
          if(Storage.Contains("OK")==true){
	       Serial.println("Server is ready. Waiting for client to connected...");
	       Storage.ClearString();
               break;
	     }
	  else{
               Storage.ClearString();
	       Serial.println("Building server is failed. Rebuilding after 5s...");
               delay(5000);
	      } 
	}
}
void ESPUpload(int chal, String input)
{
    //String content="";
    //content += input;
    int SIZE;
    SIZE = input.length()+2;
    ESPTCPsend(chal,SIZE);
    CommLaunch(input, SIZE*1, 0);
//ESPTCPcheck();
}
void ESPTCPsend(int chal, int length)
{
	String message = "AT+CIPSEND="+String(chal)+",";
	message+=length;
	CommLaunch(message+"\r", length*1 , 0);
}

void ESPFinish()
{        //close TCP, disconnect from AP
    CommLaunch("AT+CIPCLOSE\r", 5 * 1000, 0);
    CommLaunch("AT+CWQAP\r", 2 * 1000, 0);
}
void ESPTCPcheck()
{
    
    
}


