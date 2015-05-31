void ESP_set_baud_rate(long int baud_rate)
{
  CommLaunch("AT+UART="+(String) baud_rate+ ",8,1,0,3", 2000,0);
}
void ESP_setup(String wifi_name, String passw, String IP_address)
{
  CommLaunch("AT+CIOBAUD?", 2000,0);
  CommLaunch("AT+RST", 2000,0);
  CommLaunch("AT+CWMODE=3", 1000,0);
  CommLaunch("AT+CIPAP=\""+IP_address+"\"", 1000,0);
  CommLaunch("AT+CIPMUX=1", 1000,0);
  CommLaunch("AT+CIFSR", 2000,0);
  CommLaunch("AT+CWSAP=\"" + wifi_name + "\",\"" + passw + "\",5,0", 3000, 0);
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
      //when serial baud rate is 9600, need this delay time
      //delay(2);
    }
    
    if (tmp.length()>0)
    {
      Serial.print(tmp);
        if (keyword == 1)
        {
	  storage.setString(storage.getString() + tmp); // if the keyword is triggered, copy the string
        }
	tmp = "";
     }
   }
  Serial.println("\n------------------------------");
}

void ESP_set_server(String port)
{
  while(true)
  {
    CommLaunch("AT+CIPSERVER=1,"+port, 3000, 1);
          
    if(storage.Contains("OK")==true)
    {
      //set TCP server timeout, if AT+CIPSTO=0, it will never timeout.
      CommLaunch("AT+CIPSTO=0",2000,0);
      
      Serial.println("Server is ready. Waiting for client to connect...");
      storage.ClearString();
      break;
    }
    else
    {
      storage.ClearString();
      Serial.println("Building server is failed. Rebuilding after 5s...");
      delay(5000);
    } 
  }
}

//following code is for server to send message to client. need to be rewritten.
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

//close TCP, disconnect from AP
void ESPFinish()
{        //close TCP, disconnect from AP
  CommLaunch("AT+CIPCLOSE\r", 5 * 1000, 0);
  CommLaunch("AT+CWQAP\r", 2 * 1000, 0);
}

void ESPTCPcheck()
{
    
}


