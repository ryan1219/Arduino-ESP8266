void ESPWIFIsetup()
{
	//CommLaunch("AT+CIOBAUD?\r", 2000,0);
	delay(1000);
	CommLaunch("AT\r", 1000, 0);
	CommLaunch("AT+RST\r", 2000, 0);
	CommLaunch("AT+CWMODE=3\r", 1000, 0);
	//CommLaunch("AT+CIPMUX=0\r", 1000,0);
	CommLaunch("AT+CIFSR\r", 2*1000, 0);
        CommLaunch("AT+CWLAP\r", 6*1000, 0);
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
		while (Serial1.available()>0)  {
			tmp += char(Serial1.read());
			delay(2);
		}
		if (tmp.length()>0){
			Serial.println(tmp);
			if (keyword == 1){
				Storage.setString(Storage.getString() + tmp); // if the keyword is triggered, copy the string
			}
			tmp = "";
		}
	}
	Serial.println("\n------------------------------");
}

void ESPWIFIassociate()
{
	//CommLaunch("AT+CWJAP=\"ESP8266SERVER\",\"12345\"\r", 10000,0);
	String usr, pw, asso;
	
        usr = "ESP8266SERVER";
	pw = "12345";
	asso = "AT+CWJAP=\"" + usr + "\",\"" + pw + "\"";

	while (true)
	{
		CommLaunch(asso, 4 * 1000, 1);

		if (Storage.Contains("OK") == true)
		{
			Storage.ClearString();
                        Serial.println("FINISH WIFI ASSOCIATION!");
			Serial.println("------------------------------");
                        CommLaunch("AT+CIFSR\r", 2*1000, 0);
			Storage.ClearString();
			break;
		}
		else{
                        Storage.ClearString();
			Serial.println("[ERROR0] : Cannot associate with WIFI: " + usr+ ", closing current connection and restart again");
                        Serial.println("------------------------------");
                        CommLaunch("AT+CWQAP", 8 * 1000, 0);
		    }
	}
}

void ESPTCPconnection()
{
	String URL = "AT+CIPSTART=\"TCP\",\"192.168.6.1\",80";

	//URL = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80";
	while (true)
	{
		CommLaunch(URL,1000, 1);

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
		else{
                        Storage.ClearString();
			Serial.println("[ERROR0] : cannot set TCP connection, restart connection after 5s.");
                        Serial.println("------------------------------");
                        CommLaunch("AT+CIPCLOSE", 8 * 1000, 0);
		    }
		
	}
}

void ESPUpload(String input)
{
    //String content="";
    //content += input;
    int SIZE;
    SIZE = input.length()+1; // 1 is character return
    ESPTCPsend(SIZE);
    
    while(true){
        String tmp;
        while (Serial1.available()>0)  {
	     tmp += char(Serial1.read());
          }
        if (tmp.length()>0){
             STR.setString(STR.getString() + tmp); // if the keyword is triggered, copy the string
	     tmp = "";
           }
	     
        if(STR.Contains(">")==true){
                 
                 CommLaunch(input, 8*SIZE/9600, 0);
                 STR.ClearString();
                 break;
              }
        }
    
    CommLaunch(input, 8*SIZE/9600, 0);
//ESPTCPcheck();
}
void ESPTCPsend(int SIZE)
{
	String message = "AT+CIPSEND=";
	message+=SIZE;
        
	CommLaunch(message+"\r", 1000, 0);
}

void ESPFinish()
{
	/*close TCP, disconnect from AP
	*/
	CommLaunch("AT+CIPCLOSE", 2 * 1000, 0);
	CommLaunch("AT+CWQAP", 2 * 1000, 0);
}
void ESPTCPcheck()
{
 
}
