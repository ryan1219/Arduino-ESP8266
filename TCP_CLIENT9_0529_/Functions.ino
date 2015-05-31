void ESP_setup()
{
  //CommLaunch("AT+CIOBAUD?", 2000,0);
  CommLaunch("AT", 1000, 0);
  CommLaunch("AT+RST", 2000, 0);
  CommLaunch("AT+CWMODE=3", 1000, 0);
  //CommLaunch("AT+CIPMUX=0", 1000,0);
  CommLaunch("AT+CIFSR", 2 * 1000, 0);
  CommLaunch("AT+CWLAP", 4 * 1000, 0);
}

void CommLaunch(String cmd, int timeDelay, int keyword)
{
  Serial1.println(cmd);    //send command to ESP8266

  Serial.println("ESP8266 RESPONSE:");
  String tmp = "";
  String local_storage="";
  
  long deadline = millis() + timeDelay;
  
  while (millis() < deadline)
  {
    while (Serial1.available()>0)
    {
      tmp += char(Serial1.read());
      
    }
    
    if (tmp.length()>0)
    {
      Serial.print(tmp);
      if (keyword == 1)
      {
        local_storage=local_storage+tmp; // if the keyword is triggered, copy the string
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
    CommLaunch(cmd, 8 * 1000, 1);

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
  String cmd = "AT+CIPSTART=\"TCP\",\"" + IP + "\"," +  port_num ;

	
  while (true)
  {
    CommLaunch(cmd, 2*1000, 1);

    if (Storage.Contains("CONNECT") == true)
    {
      Storage.ClearString();
      Serial.println("TCP CONNECTED!");
      Serial.println("------------------------------");
      
      //set to transmission mode
      CommLaunch("AT+CIPMODE=1", 2*1000, 0);
      //
      CommLaunch("AT+CIPSTATUS\r", 2 * 1000, 0);
      Serial.println("READY TO SEND DATA!");
      Serial.println("------------------------------");
      
      return;
     }
    else
    {
      Storage.ClearString();
      Serial.println("[ERROR0] : cannot set TCP connection, restart connection after 5s.");
      Serial.println("------------------------------");
      CommLaunch("AT+CIPCLOSE", 5 * 1000, 0);
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
  String         cmd = "AT+CIPSEND";
  //messaage_length
  //int            message_length;
  
  
 // message_length = message.length(); // +1 since EOL
  //cmd = cmd + (String)message_length;
  
  //1000*((8*message_length)/SUSTAINABLE_SERIAL1_BAUD_RATE)      
  message_interarrival_time = 2;
        
  //get the TCP ready to send the messsage of length X
  Serial1.println(cmd);  //launch command "AT+CIPSEND=X"
  
  //keep waiting until ESP is ready (i.e., until ">" is rx from serial1)
  //ResponseCheck function can echo the TCP command to the PC for checking
  result=ResponseCheck(">");
  
  //save sending message
  //current_message=message;
  
  //now, send the message using TCP       
  Serial1.print(message);

  //wait for next messsage to be sent
  //delay(message_interarrival_time);
  
  //result=ResponseCheck("SEND");
  
  //clear saved value
  //current_message="";

}

boolean ResponseCheck(String keywords)
{
  /* Store the important information from ESP reponse during setup. 
     This variable is only used in ResponseCheck */
  StringModule analysis = StringModule("");
  String           tmp;
  
  while (true)
  {
    while (Serial1.available()>0)
    {
      tmp += char(Serial1.read());
      //delay(2);
    }
    
    if (tmp.length()>0)
    {
      //Serial.print(tmp);
      analysis.setString(analysis.getString() + tmp); 
      tmp = "";
    }
    // if send ok, continue to send next message
    if (analysis.Contains(keywords) == true)
    {
      analysis.ClearString();
      return true;
    }
    
    //if send error, check AP connection, check TCP status, and rebuild the TCP
    if (analysis.Contains("ERROR") == true)
    { 
      analysis.ClearString();
      //check if client is connected to the AP
      ESP_join_AP("ESP8266SERVER_YAN","12345");
      
      //check TCP connection, if TCP CLOSED, rebuild
      CommLaunch("AT+CIPSTATUS\r", 2 * 1000, 1);
      if (Storage.Contains("STATUS:3") == true)
      {
        Storage.ClearString();
        ESP_send_message(current_message);
        return true;
      }
      else
      {
        Storage.ClearString();
        Serial.println("TCP CLIENT CLOSED, RESTARTING IN 4s...");
        Serial.println("------------------------------");
        delay(4*1000);
        ESP_TCP_connection(TCP_IP, TCP_port);
          
        //send the message again
        ESP_send_message(current_message);
        return true;
      }
    }  
  }
}

//close TCP, disconnect from AP
void ESP_finish()
{
  CommLaunch("AT+CIPCLOSE", 2 * 1000, 0);
  CommLaunch("AT+CWQAP", 2 * 1000, 0);
}

//back up code
/*
boolean ResponseCheck(String keywords)
{
  String           tmp;
  
  while (true)
  {
    while (Serial1.available()>0)
    {
      tmp += char(Serial1.read());
      delay(1);
    }
    
    if (tmp.length()>0)
    {
      Serial.println(tmp);
      analysis.setString(analysis.getString() + tmp); 
      tmp = "";
    }
    
    // if send ok, continue to send next message
    if (analysis.Contains(keywords) == true)
    {
      analysis.ClearString();
      return true;
    }
  }
}
*/
