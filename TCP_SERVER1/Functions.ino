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
	       Serial.println("Building server is failed. Rebuilding after 10s...");
               Serial.println(Storage.getString());
	       delay(10000);
	      } 
	}
}

/*
void ESPTCPsend(int length)
{
  String message = "AT+CIPSEND="; 
  message+=length;
  CommLaunch(message, 2*1000, true, 0); 
}
*/
/*
void ESPUpload(int input)
{
  String content;
  int SIZE;
  content = "GET /update?key=";
  content += API_KEY;
  content += "&field1=";
  content += input;
  SIZE = content.length()+1;
  
  ESPTCPsend(SIZE);
  CommLaunch(content, 15*1000, true, 0); 
  
  ESPTCPcheck();
}
*/
/*
void ESPTCPcheck()
{
  while(true)
  {
    CommLaunch("AT+CIPCLOSE", 5*1000, true, 1);
    if(ESPCommandSucess("OK+ERROR",2))
        break;
  }
  ESPTCPconnection();
}
*/
/*
void ESPDownload()
{
  String content;
  int SIZE;
  content = "GET /channels/26355/feeds.json?results=1\r\n";
  SIZE = content.length()-1;

  ESPTCPsend(SIZE);
  CommLaunch(content, 15*1000, true, 0); 
  
  ESPTCPcheck(); 
}
*/
/*
void ESPFinish()
{
    CommLaunch("AT+CIPCLOSE", 6*1000, true, 0);
    CommLaunch("AT+CWQAP", 2*1000, true, 0);
}
*/

/*boolean ESPCommandSucess(String KEY, int index)
{
  int iteration;
  STR.ReplaceBase(Storage);
  Storage = "";
  
  if(index==1)
    return STR.Contains(KEY);
  
  boolean result = false;
  String  component;
  STR.StoreKey(KEY);
  for(iteration = 0; iteration < index; iteration++)
  {
    component = STR.Delimitation('+');

    result = result || STR.Contains(component);
  }
  
  if(STR.Contains("busy"))
  {
    Serial.println("ENTER REST");
    delay(1000);
    Serial.println("EXIT REST");  
}
  
  return result;
}
*/
