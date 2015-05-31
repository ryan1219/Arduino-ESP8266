void ESP_setup()
{
  //CommLaunch("AT+CIOBAUD?\r", 2000,0);
  delay(1000);
  CommLaunch("AT\r", 1000, 0);
  CommLaunch("AT+RST\r", 2000, 0);
  CommLaunch("AT+CWMODE=1\r", 1000, 0);
  //CommLaunch("AT+CIPMUX=0\r", 1000,0);
  CommLaunch("AT+CIFSR\r", 2 * 1000, 0);
  CommLaunch("AT+CWLAP\r", 6 * 1000, 0);
}

void CommLaunch(String cmd, int timeDelay, int keyword)
{
  Serial1.println(cmd);    //send command to ESP8266

  Serial.println("ESP8266 RESPONSE:");
  String tmp = "";
  long deadline = millis() + timeDelay;
  //delay(timeDelay);
  while (millis() < deadline)
  {
    while (Serial1.available()>0)
    {
      tmp += char(Serial1.read());
      delay(2);
    }
    
    if (tmp.length()>0)
    {
      Serial.println(tmp);
        if (keyword == 1)
        {
	  Storage.setString(Storage.getString() + tmp); // if the keyword is triggered, copy the string
        }
	tmp = "";
     }
   }
  Serial.println("\n------------------------------");
}

void ESP_join_AP(String usr, String pw)
{
  //CommLaunch("AT+CWJAP=\"ESP8266SERVER\",\"12345\"\r", 10000,0);
  String           cmd;
  
  cmd = "AT+CWJAP=\"" + usr + "\",\"" + pw + "\"";

  while (true)
  {
    CommLaunch(cmd, 4 * 1000, 1);

    if (Storage.Contains("OK") == true)
    {
      Storage.ClearString();
      Serial.println("FINISH WIFI ASSOCIATION!");
      Serial.println("------------------------------");
      CommLaunch("AT+CIFSR\r", 2 * 1000, 0);
      Storage.ClearString();
      break;
     }
     else
     {
       Storage.ClearString();
       Serial.println("[ERROR0] : Cannot associate with WIFI: " + usr + ", closing current connection and restart again");
       Serial.println("------------------------------");
       CommLaunch("AT+CWQAP", 8 * 1000, 0);
      }
  }
}

void ESP_TCP_connection(String IP, String port_num)
{
  String cmd = "AT+CIPSTART=\"TCP\","+ IP + "," + port_num;

	
  while (true)
  {
    CommLaunch(cmd, 1000, 1);

    if (Storage.Contains("CONNECT") == true)
    {
      Storage.ClearString();
      Serial.println("FINISH TCP CONNECTION!");
      Serial.println("------------------------------");
      CommLaunch("AT+CIPSTATUS\r", 2 * 1000, 0);
      Serial.println("READY TO SEND DATA!");
      Serial.println("------------------------------");
      break;
     }
    else
    {
      Storage.ClearString();
      Serial.println("[ERROR0] : cannot set TCP connection, restart connection after 5s.");
      Serial.println("------------------------------");
      CommLaunch("AT+CIPCLOSE", 8 * 1000, 0);
     }

    }
}

void ESP_send_message(String message)
{
  //
  unsigned long  message_interarrival_time;
  
  //
  boolean        result;
  
  //TCP command
  String         cmd = "AT+CIPSEND=";
  //messaage_length
  int            message_length;
  
  
  message_length = message.length(); // +1 since EOL
  cmd = cmd + (String)message_length;
        
  message_interarrival_time = 1000*((8*message_length/SERIAL1_BAUD_RATE)/SERIAL1_BAUD_RATE_SCALE);
        
  //get the TCP ready to send the messsage of length X
  Serial1.println(cmd);  //launch command "AT+CIPSEND=X"
  //echo the TCP command to the PC for checking
  Serial.println(cmd);
   
  //keep waiting until ESP is ready (i.e., until ">" is rx from serial1)
  result=ResponseCheck(">");

  //now, send the message using TCP       
  Serial1.println(message);
  
  //wait for next messsage to be sent
  delay(message_interarrival_time);

}

boolean ResponseCheck(String keyWords)
{
  while (true)
  {
    String tmp = "";
    while (Serial1.available()>0)
    {
      tmp += char(Serial1.read());
    }
    if (tmp.length()>0)
    {
      STR.setString(STR.getString() + tmp); 
      tmp = "";
    }
    if (STR.Contains(keyWords) == true)
    {
      STR.ClearString();
      return true;
    }
  }
}

void ESPFinish()
{
	/*close TCP, disconnect from AP
	*/
	CommLaunch("AT+CIPCLOSE", 2 * 1000, 0);
	CommLaunch("AT+CWQAP", 2 * 1000, 0);
}
boolean launchComWithCheck(String cmd, String keyWords)
{
  Serial1.println(cmd);
  while (true){
		String tmp = "";
		while (Serial1.available()>0)  {
			tmp += char(Serial1.read());
		}
		if (tmp.length()>0){
			STR.setString(STR.getString() + tmp); // if the keyword is triggered, copy the string
			tmp = "";
		}

		if (STR.Contains(keyWords) == true){
			STR.ClearString();
			return true;
		}
	}
  
  
}

void ESPTCPcheck()
{

}
